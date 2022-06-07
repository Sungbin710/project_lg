#include <gst/gst.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <unistd.h>

static gboolean cb_print_position (GstElement *pipeline);
char *make_realpath();

int main(int argc, char *argv[])
{
	GstElement *pipeline, *playbin;
	GstBus *bus;
	GstMessage *msg;

	gst_init (&argc, &argv);

	char *path = make_realpath();
	if(path == NULL)
		printf("File error\n");

	playbin = gst_element_factory_make ("playbin", "playbin");
	if (!playbin) {
		g_printerr ("Not all elements could be created.\n");
		return -1;
	}

	g_object_set (playbin, "uri", path, NULL);
	gst_element_set_state (playbin, GST_STATE_PLAYING);
	
	//pipeline = gst_parse_launch(path, NULL);
	//gst_element_set_state (pipeline, GST_STATE_PLAYING);
	//bus = gst_element_get_bus (pipeline);
	
	bus = gst_element_get_bus (playbin);
	  
	msg = gst_bus_timed_pop_filtered (bus, GST_CLOCK_TIME_NONE,
	GST_MESSAGE_ERROR | GST_MESSAGE_EOS);
	
//	gst_element_seek_simple (playbin, GST_FORMAT_TIME,
//	GST_SEEK_FLAG_FLUSH | GST_SEEK_FLAG_KEY_UNIT, 3 * GST_SECOND);
	
	//cb_print_position(pipeline);
	//gst_element_query_position(pipeline, GST_FORMAT_TIME, &current);
	//g_print("Position %" GST_TIME_FORMAT, GST_TIME_ARGS (current));



	gst_message_unref (msg);
	gst_object_unref (bus);
	//gst_element_set_state (pipeline, GST_STATE_NULL);
	//gst_object_unref (pipeline);
	gst_element_set_state (playbin, GST_STATE_NULL);
	gst_object_unref (playbin);

	return 0;
}


static gboolean
cb_print_position (GstElement *pipeline)
{
  gint64 pos, len;

  if (gst_element_query_position (pipeline, GST_FORMAT_TIME, &pos)
    && gst_element_query_duration (pipeline, GST_FORMAT_TIME, &len)) {
    g_print ("Time: %" GST_TIME_FORMAT " / %" GST_TIME_FORMAT "\r",
         GST_TIME_ARGS (pos), GST_TIME_ARGS (len));
  }

  /* call me again */
  return TRUE;
}

char *make_realpath(){
	
	char *new_path = (char *)malloc(sizeof(char) * 1024);
	strcpy(new_path, "playbin uri=file://");

	char video_name[256];
	char path[1024];
	
	/* find realpath */
	if(realpath(".", path) == NULL){
	  perror("realpath error");
	  return NULL;
	}

	printf("media name: ");
	scanf("%s", video_name);

	strcat(path,"/video/");
	strcat(path, video_name);

	if( access(path, F_OK) != -1){
		printf("file exist\n");
	}
	else{
		printf("file not exist\n");
		return NULL;
	}
	
	strcat(new_path, path);  

	return new_path;

}
