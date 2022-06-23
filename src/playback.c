#include <string.h>

#include <gtk/gtk.h>
#include <gst/gst.h>
#include <gst/video/videooverlay.h>
#include <gst/pbutils/pbutils.h>

#include <gdk/gdk.h>
#if defined (GDK_WINDOWING_X11)
#include <gdk/gdkx.h>
#elif defined (GDK_WINDOWING_WIN32)
#include <gdk/gdkwin32.h>
#elif defined (GDK_WINDOWING_QUARTZ)
#include <gdk/gdkquartz.h>
#endif

#include <sys/types.h>
#include <dirent.h>

#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/stat.h>		/* stat -> file size */
#include "interface.h"
#include "playback.h"
#include "playlist.h"
#include "discover.h"
#include "account.h"

static GCond playback_cond;
static GMutex playback_lock;

static void seek_to_time(GstElement * playbin, gint64 seek_time)
{
	if (!gst_element_seek
	    (playbin, 1.0, GST_FORMAT_TIME, GST_SEEK_FLAG_FLUSH,
	     GST_SEEK_TYPE_SET, seek_time, GST_SEEK_TYPE_NONE,
	     GST_CLOCK_TIME_NONE)) {
		g_print("Seek failed!\n");
	}
	// gst_element_seek_simple (playbin, GST_FORMAT_TIME, GST_SEEK_FLAG_FLUSH | GST_SEEK_FLAG_KEY_UNIT, seek_time);

}

/* This function is called when the GUI toolkit creates the physical window that will hold the video.
 * At this point we can retrieve its handler (which has a different meaning depending on the windowing system)
 * and pass it to GStreamer through the VideoOverlay interface. */
static void realize_cb(GtkWidget * widget, CustomData * data)
{
	GdkWindow *window = gtk_widget_get_window(widget);
	guintptr window_handle;

	if (!gdk_window_ensure_native(window))
		g_error
		    ("Couldn't create native window needed for GstVideoOverlay!");

	/* Retrieve window handler from GDK */
#if defined (GDK_WINDOWING_WIN32)
	window_handle = (guintptr) GDK_WINDOW_HWND(window);
#elif defined (GDK_WINDOWING_QUARTZ)
	window_handle = gdk_quartz_window_get_nsview(window);
#elif defined (GDK_WINDOWING_X11)
	window_handle = GDK_WINDOW_XID(window);
#endif
	/* Pass it to playbin, which implements VideoOverlay and will forward it to the video sink */
	gst_video_overlay_set_window_handle(GST_VIDEO_OVERLAY(data->playbin),
					    window_handle);
}

/* This function is called when the PLAY button is clicked */
static void play_cb(GtkButton * button, CustomData * data)
{
	gst_element_set_state(data->playbin, GST_STATE_PLAYING);
}

/* This function is called when the PAUSE button is clicked */
static void pause_cb(GtkButton * button, CustomData * data)
{
	gst_element_set_state(data->playbin, GST_STATE_PAUSED);
}

/* This function is called when the STOP button is clicked */
static void stop_cb(GtkButton * button, CustomData * data)
{
	gst_element_set_state(data->playbin, GST_STATE_READY);
}

/* This function is called when the main window is closed */
static void
delete_event_cb(GtkWidget * widget, GdkEvent * event, CustomData * data)
{

	data->last_playback = GST_CLOCK_TIME_NONE;	// #define GST_CLOCK_TIME_NONE ((GstClockTime) -1)
	gst_element_query_position(data->playbin, GST_FORMAT_TIME,
				   &data->last_playback);
	g_print("Position %" GST_TIME_FORMAT " / %" GST_TIME_FORMAT "\n",
		GST_TIME_ARGS(data->last_playback),
		GST_TIME_ARGS(data->duration));

	/* eos 처리 -> eos인 경우 query시 -1값이 반환 되는 듯 */
	if (data->last_playback < 0)
		data->last_playback = 0;
	stop_cb(NULL, data);
	gtk_main_quit();
}

/* This function is called everytime the video window needs to be redrawn (due to damage/exposure,
 * rescaling, etc). GStreamer takes care of this in the PAUSED and PLAYING states, otherwise,
 * we simply draw a black rectangle to avoid garbage showing up. */
static gboolean draw_cb(GtkWidget * widget, cairo_t * cr, CustomData * data)
{
	if (data->state < GST_STATE_PAUSED) {
		GtkAllocation allocation;

		/* Cairo is a 2D graphics library which we use here to clean the video window.
		 * It is used by GStreamer for other reasons, so it will always be available to us. */
		gtk_widget_get_allocation(widget, &allocation);
		cairo_set_source_rgb(cr, 0, 0, 0);
		cairo_rectangle(cr, 0, 0, allocation.width, allocation.height);
		cairo_fill(cr);
	}

	return FALSE;
}

/* This function is called when the slider changes its position. We perform a seek to the
 * new position here. */
static void slider_cb(GtkRange * range, CustomData * data)
{
	gdouble value = gtk_range_get_value(GTK_RANGE(data->slider));
	gst_element_seek_simple(data->playbin, GST_FORMAT_TIME,
				GST_SEEK_FLAG_FLUSH | GST_SEEK_FLAG_KEY_UNIT,
				(gint64) (value * GST_SECOND));
}

/* This creates all the GTK+ widgets that compose our application, and registers the callbacks */
static void create_ui(CustomData * data, char *window_name)
{
	GtkWidget *main_window;	/* The uppermost window, containing all other windows */
	GtkWidget *video_window;	/* The drawing area where the video will be shown */
	GtkWidget *main_box;	/* VBox to hold main_hbox and the controls */
	GtkWidget *main_hbox;	/* HBox to hold the video_window and the stream info text widget */
	GtkWidget *controls;	/* HBox to hold the buttons and the slider */
	GtkWidget *play_button, *pause_button;	//*stop_button; /* Buttons */

	main_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	g_signal_connect(G_OBJECT(main_window), "delete-event",
			 G_CALLBACK(delete_event_cb), data);

	video_window = gtk_drawing_area_new();
	gtk_widget_set_double_buffered(video_window, FALSE);
	g_signal_connect(video_window, "realize", G_CALLBACK(realize_cb), data);
	g_signal_connect(video_window, "draw", G_CALLBACK(draw_cb), data);

	play_button =
	    gtk_button_new_from_icon_name("media-playback-start",
					  GTK_ICON_SIZE_SMALL_TOOLBAR);
	g_signal_connect(G_OBJECT(play_button), "clicked", G_CALLBACK(play_cb),
			 data);

	pause_button =
	    gtk_button_new_from_icon_name("media-playback-pause",
					  GTK_ICON_SIZE_SMALL_TOOLBAR);
	g_signal_connect(G_OBJECT(pause_button), "clicked",
			 G_CALLBACK(pause_cb), data);

/*
  stop_button = gtk_button_new_from_icon_name ("media-playback-stop", GTK_ICON_SIZE_SMALL_TOOLBAR);
  g_signal_connect (G_OBJECT (stop_button), "clicked", G_CALLBACK (stop_cb), data);
*/

	data->slider =
	    gtk_scale_new_with_range(GTK_ORIENTATION_HORIZONTAL, 0, 100, 1);
	gtk_scale_set_draw_value(GTK_SCALE(data->slider), 0);
	data->slider_update_signal_id =
	    g_signal_connect(G_OBJECT(data->slider), "value-changed",
			     G_CALLBACK(slider_cb), data);

	data->streams_list = gtk_text_view_new();
	gtk_text_view_set_editable(GTK_TEXT_VIEW(data->streams_list), FALSE);

	controls = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
	gtk_box_pack_start(GTK_BOX(controls), play_button, FALSE, FALSE, 2);
	gtk_box_pack_start(GTK_BOX(controls), pause_button, FALSE, FALSE, 2);
	//gtk_box_pack_start (GTK_BOX (controls), stop_button, FALSE, FALSE, 2);
	gtk_box_pack_start(GTK_BOX(controls), data->slider, TRUE, TRUE, 2);

	main_hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
	gtk_box_pack_start(GTK_BOX(main_hbox), video_window, TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(main_hbox), data->streams_list, FALSE, FALSE,
			   2);

	main_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
	gtk_box_pack_start(GTK_BOX(main_box), main_hbox, TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(main_box), controls, FALSE, FALSE, 0);
	gtk_container_add(GTK_CONTAINER(main_window), main_box);
	gtk_window_set_default_size(GTK_WINDOW(main_window), 640, 480);

	gtk_window_set_title(GTK_WINDOW(main_window), window_name);

	gtk_widget_show_all(main_window);
}

/* This function is called periodically to refresh the GUI */
static gboolean refresh_ui(CustomData * data)
{
	gint64 current = -1;

	/* We do not want to update anything unless we are in the PAUSED or PLAYING states */
	if (data->state < GST_STATE_PAUSED)
		return TRUE;

	/* If we didn't know it yet, query the stream duration */
	if (!GST_CLOCK_TIME_IS_VALID(data->duration)) {
		if (!gst_element_query_duration
		    (data->playbin, GST_FORMAT_TIME, &data->duration)) {
			g_printerr("Could not query current duration.\n");
		} else {
			/* Set the range of the slider to the clip duration, in SECONDS */
			gtk_range_set_range(GTK_RANGE(data->slider), 0,
					    (gdouble) data->duration /
					    GST_SECOND);
		}
	}

	if (data->is_start) {

		current = data->start_position;
		seek_to_time(data->playbin, current);
		g_signal_handler_block(data->slider,
				       data->slider_update_signal_id);
		/* Set the position of the slider to the current pipeline position, in SECONDS */
		gtk_range_set_value(GTK_RANGE(data->slider),
				    (gdouble) current / GST_SECOND);
		/* Re-enable the signal */
		g_signal_handler_unblock(data->slider,
					 data->slider_update_signal_id);
		data->is_start = FALSE;
		return TRUE;

	}

	if (gst_element_query_position
	    (data->playbin, GST_FORMAT_TIME, &current)) {
		/* Block the "value-changed" signal, so the slider_cb function is not called
		 * (which would trigger a seek the user has not requested) */
		g_signal_handler_block(data->slider,
				       data->slider_update_signal_id);
		/* Set the position of the slider to the current pipeline position, in SECONDS */
		gtk_range_set_value(GTK_RANGE(data->slider),
				    (gdouble) current / GST_SECOND);
		/* Re-enable the signal */
		g_signal_handler_unblock(data->slider,
					 data->slider_update_signal_id);
	}

	return TRUE;
}

/* This function is called when new metadata is discovered in the stream */
static void tags_cb(GstElement * playbin, gint stream, CustomData * data)
{
	/* We are possibly in a GStreamer working thread, so we notify the main
	 * thread of this event through a message in the bus */
	gst_element_post_message(playbin,
				 gst_message_new_application(GST_OBJECT
							     (playbin),
							     gst_structure_new_empty
							     ("tags-changed")));
}

/* This function is called when an error message is posted on the bus */
static void error_cb(GstBus * bus, GstMessage * msg, CustomData * data)
{
	GError *err;
	gchar *debug_info;

	/* Print error details on the screen */
	gst_message_parse_error(msg, &err, &debug_info);
	g_printerr("Error received from element %s: %s\n",
		   GST_OBJECT_NAME(msg->src), err->message);
	g_printerr("Debugging information: %s\n",
		   debug_info ? debug_info : "none");
	g_clear_error(&err);
	g_free(debug_info);

	/* Set the pipeline to READY (which stops playback) */
	gst_element_set_state(data->playbin, GST_STATE_READY);
}

/* This function is called when an End-Of-Stream message is posted on the bus.
 * We just set the pipeline to READY (which stops playback) */
static void eos_cb(GstBus * bus, GstMessage * msg, CustomData * data)
{
	g_print("End-Of-Stream reached.\n");
	gst_element_set_state(data->playbin, GST_STATE_READY);

}

/* This function is called when the pipeline changes states. We use it to
 * keep track of the current state. */
static void state_changed_cb(GstBus * bus, GstMessage * msg, CustomData * data)
{
	GstState old_state, new_state, pending_state;
	gst_message_parse_state_changed(msg, &old_state, &new_state,
					&pending_state);
	if (GST_MESSAGE_SRC(msg) == GST_OBJECT(data->playbin)) {
		data->state = new_state;
		g_print("State set to %s\n",
			gst_element_state_get_name(new_state));
		if (old_state == GST_STATE_READY
		    && new_state == GST_STATE_PAUSED) {
			/* For extra responsiveness, we refresh the GUI as soon as we reach the PAUSED state */
			refresh_ui(data);
		}
	}
}

/* Extract metadata from all the streams and write it to the text widget in the GUI */
static void analyze_streams(CustomData * data)
{
	gint i;
	GstTagList *tags;
	gchar *str, *total_str;
	guint rate;
	gint n_video, n_audio, n_text;
	GtkTextBuffer *text;

	/* Clean current contents of the widget */
	text = gtk_text_view_get_buffer(GTK_TEXT_VIEW(data->streams_list));
	gtk_text_buffer_set_text(text, "", -1);

	/* Read some properties */
	g_object_get(data->playbin, "n-video", &n_video, NULL);
	g_object_get(data->playbin, "n-audio", &n_audio, NULL);
	g_object_get(data->playbin, "n-text", &n_text, NULL);

	for (i = 0; i < n_video; i++) {
		tags = NULL;
		/* Retrieve the stream's video tags */
		g_signal_emit_by_name(data->playbin, "get-video-tags", i,
				      &tags);
		if (tags) {
			total_str = g_strdup_printf("video stream %d:\n", i);
			gtk_text_buffer_insert_at_cursor(text, total_str, -1);
			g_free(total_str);
			gst_tag_list_get_string(tags, GST_TAG_VIDEO_CODEC,
						&str);
			total_str =
			    g_strdup_printf("  codec: %s\n",
					    str ? str : "unknown");
			gtk_text_buffer_insert_at_cursor(text, total_str, -1);
			g_free(total_str);
			g_free(str);
			gst_tag_list_free(tags);
		}
	}

	for (i = 0; i < n_audio; i++) {
		tags = NULL;
		/* Retrieve the stream's audio tags */
		g_signal_emit_by_name(data->playbin, "get-audio-tags", i,
				      &tags);
		if (tags) {
			total_str = g_strdup_printf("\naudio stream %d:\n", i);
			gtk_text_buffer_insert_at_cursor(text, total_str, -1);
			g_free(total_str);
			if (gst_tag_list_get_string
			    (tags, GST_TAG_AUDIO_CODEC, &str)) {
				total_str =
				    g_strdup_printf("  codec: %s\n", str);
				gtk_text_buffer_insert_at_cursor(text,
								 total_str, -1);
				g_free(total_str);
				g_free(str);
			}
			if (gst_tag_list_get_string
			    (tags, GST_TAG_LANGUAGE_CODE, &str)) {
				total_str =
				    g_strdup_printf("  language: %s\n", str);
				gtk_text_buffer_insert_at_cursor(text,
								 total_str, -1);
				g_free(total_str);
				g_free(str);
			}
			if (gst_tag_list_get_uint(tags, GST_TAG_BITRATE, &rate)) {
				total_str =
				    g_strdup_printf("  bitrate: %d\n", rate);
				gtk_text_buffer_insert_at_cursor(text,
								 total_str, -1);
				g_free(total_str);
			}
			gst_tag_list_free(tags);
		}
	}

	for (i = 0; i < n_text; i++) {
		tags = NULL;
		/* Retrieve the stream's subtitle tags */
		g_signal_emit_by_name(data->playbin, "get-text-tags", i, &tags);
		if (tags) {
			total_str =
			    g_strdup_printf("\nsubtitle stream %d:\n", i);
			gtk_text_buffer_insert_at_cursor(text, total_str, -1);
			g_free(total_str);
			if (gst_tag_list_get_string
			    (tags, GST_TAG_LANGUAGE_CODE, &str)) {
				total_str =
				    g_strdup_printf("  language: %s\n", str);
				gtk_text_buffer_insert_at_cursor(text,
								 total_str, -1);
				g_free(total_str);
				g_free(str);
			}
			gst_tag_list_free(tags);
		}
	}
}

/* This function is called when an "application" message is posted on the bus.
 * Here we retrieve the message posted by the tags_cb callback */
static void application_cb(GstBus * bus, GstMessage * msg, CustomData * data)
{
	if (g_strcmp0
	    (gst_structure_get_name(gst_message_get_structure(msg)),
	     "tags-changed") == 0) {
		/* If the message is the "tags-changed" (only one we are currently issuing), update
		 * the stream info GUI */
		analyze_streams(data);
	}
}

static Playback_Callbacks playback_callbacks = { 0 };

static gboolean get_start_pos_cb(gint64 * start_pos, gint64 * receive_pos)
{
	*start_pos = *receive_pos;
	// printf("start_pos: %ld\n", *start_pos);
	g_mutex_lock(&playback_lock);
	g_cond_signal(&playback_cond);
	g_mutex_unlock(&playback_lock);

	return FALSE;
}

static void playback_media_init(gint64 * start_pos)
{

	playback_callbacks.get_start_pos = get_start_pos_cb;
	playback_install_callbacks(&playback_callbacks, start_pos);
}

int playback_media(int clnt_sock)
{

	gint64 start_position;
	gint64 last_position;
	struct stat file_info;
	char uri[512] = URI_PROTO;
	char file_path[512];
	char file_name[256];
	long file_size;

	playback_media_init(&start_position);

	if (playback_media_choice
	    (file_path, sizeof(file_path), file_name, sizeof(file_name)) == 1) {
		return 1;
	}

	strcat(uri, file_path);

	if (is_playable(uri)) {
		printf("재생할 수 없는 파일입니다.\n");
		return 1;
	}
	if (stat(file_path, &file_info)) {
		printf
		    ("파일 정보를 불러오는데 실패했습니다.\n");
		return 1;
	}
	file_size = file_info.st_size;

	/* 서버에 전송하기 전에 lock을 걸어 */
	/* g_cond_wait()보다 callback 함수의 g_cond_signal()이 먼저 호출되는 경우를 방지한다. */
	g_mutex_lock(&playback_lock);

	request_last_playback_to_server(clnt_sock, get_user_id(), file_name,
					file_size);

	/* start_position에 값 받아오는 것을 기다림 */
	g_cond_wait(&playback_cond, &playback_lock);
	g_mutex_unlock(&playback_lock);

	/* 실제 재생 모듈 */
	play_module(uri, file_name, &start_position, &last_position);

	/* 재생이 종료되고 마지막 재생 위치 전송 */
	update_last_playback_to_server(clnt_sock, get_user_id(), file_name,
				       file_size, last_position);

	return 0;
}

static int
play_module(gchar * uri, gchar * file_name, gint64 * start_position,
	    gint64 * last_position)
{

	CustomData data;
	GstStateChangeReturn ret;
	GstBus *bus;
	guint cancel;

	/* Initialize our data structure */
	memset(&data, 0, sizeof(data));
	data.duration = GST_CLOCK_TIME_NONE;
	data.is_start = TRUE;
	data.start_position = *start_position;

	/* Initialize GTK */
	gtk_init(NULL, NULL);

	/* Initialize GStreamer */
	gst_init(NULL, NULL);

	/* Create the elements */
	data.playbin = gst_element_factory_make("playbin", "playbin");

	if (!data.playbin) {
		g_printerr("Not all elements could be created.\n");
		return -1;
	}

	/* Set the URI to play */
	g_object_set(data.playbin, "uri", uri, NULL);

	/* Connect to interesting signals in playbin */
	g_signal_connect(G_OBJECT(data.playbin), "video-tags-changed",
			 (GCallback) tags_cb, &data);
	g_signal_connect(G_OBJECT(data.playbin), "audio-tags-changed",
			 (GCallback) tags_cb, &data);
	g_signal_connect(G_OBJECT(data.playbin), "text-tags-changed",
			 (GCallback) tags_cb, &data);

	/* Create the GUI */
	create_ui(&data, file_name);

	/* Instruct the bus to emit signals for each received message, and connect to the interesting signals */
	bus = gst_element_get_bus(data.playbin);
	gst_bus_add_signal_watch(bus);
	g_signal_connect(G_OBJECT(bus), "message::error", (GCallback) error_cb,
			 &data);
	g_signal_connect(G_OBJECT(bus), "message::eos", (GCallback) eos_cb,
			 &data);
	g_signal_connect(G_OBJECT(bus), "message::state-changed",
			 (GCallback) state_changed_cb, &data);
	g_signal_connect(G_OBJECT(bus), "message::application",
			 (GCallback) application_cb, &data);

	gst_object_unref(bus);

	/* Start playing */
	ret = gst_element_set_state(data.playbin, GST_STATE_PLAYING);
	gst_element_set_state(data.playbin, GST_STATE_PAUSED);

	if (ret == GST_STATE_CHANGE_FAILURE) {
		g_printerr
		    ("Unable to set the pipeline to the playing state.\n");
		gst_object_unref(data.playbin);
		return -1;
	}



	/* Register a function that GLib will call every second */
	cancel = g_timeout_add_seconds(1, (GSourceFunc) refresh_ui, &data);

	/* Start the GTK main loop. We will not regain control until gtk_main_quit is called. */
	gtk_main();

	/* 값 넘겨주기 */
	*last_position = data.last_playback;
	g_source_remove(cancel);

	/* Free resources */
	gst_element_set_state(data.playbin, GST_STATE_NULL);
	gst_object_unref(data.playbin);
	printf("the end\n");
	return 0;
}
