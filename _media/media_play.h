
#include <gst/gst.h>

/* Structure to contain all our information, so we can pass it around */
typedef struct _CustomData {
  GstElement *playbin;           /* Our one and only pipeline */

  GtkWidget *slider;              /* Slider widget to keep track of current position */
  GtkWidget *streams_list;        /* Text widget to display info about the streams */
  gulong slider_update_signal_id; /* Signal ID for the slider update signal */

  GstState state;                 /* Current state of the pipeline */
  gint64 duration;                /* Duration of the clip, in nanoseconds */
  gboolean is_start;
  gint64 last_playback;
} CustomData;



seek_to_time (GstElement *pipeline, gint64  time_nanoseconds);

static void realize_cb (GtkWidget *widget, CustomData *data);


static void play_cb (GtkButton *button, CustomData *data);
static void pause_cb (GtkButton *button, CustomData *data);
static void stop_cb (GtkButton *button, CustomData *data); 
static void delete_event_cb (GtkWidget *widget, GdkEvent *event, CustomData *data);
static gboolean draw_cb (GtkWidget *widget, cairo_t *cr, CustomData *data);
static void slider_cb (GtkRange *range, CustomData *data);
static void create_ui (CustomData *data);
static gboolean refresh_ui (CustomData *data);
static void tags_cb (GstElement *playbin, gint stream, CustomData *data);
static void error_cb (GstBus *bus, GstMessage *msg, CustomData *data);
static void eos_cb (GstBus *bus, GstMessage *msg, CustomData *data);
static void state_changed_cb (GstBus *bus, GstMessage *msg, CustomData *data);
static void analyze_streams (CustomData *data);
static void application_cb (GstBus *bus, GstMessage *msg, CustomData *data);

int play_media(){
    


}