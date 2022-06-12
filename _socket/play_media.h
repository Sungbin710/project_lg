#ifndef _PLAY_H
#define _PLAY_H
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

//int print_playlist(char *playlist[], int *list_len);
int play_media(int sockfd);  

#endif /* _PLAY_H */
