 #include <gst/gst.h>

 #include <stdio.h>
 #include <string.h>
 #include <stdlib.h>

 #include <unistd.h>

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

 int
 main (int argc, char *argv[])
 {
	GstElement *pipeline;
	GstBus *bus;
	GstMessage *msg;

	/* Initialize GStreamer */
	gst_init (&argc, &argv);

	/* real path */
	char *path = make_realpath();
	if(path == NULL){
		printf("file error\n");	
	}

	pipeline =
		gst_parse_launch
		(path, NULL);
	
	
	/* Start playing */
	gst_element_set_state (pipeline, GST_STATE_PLAYING);

	/* Wait until error or EOS */
	
	bus = gst_element_get_bus (pipeline);
	msg =
		gst_bus_timed_pop_filtered (bus, GST_CLOCK_TIME_NONE,
		GST_MESSAGE_ERROR | GST_MESSAGE_EOS);
	
	/* See next tutorial for proper error message handling/parsing */
	/*
	if (GST_MESSAGE_TYPE (msg) == GST_MESSAGE_ERROR) {
	  g_error ("An error occurred! Re-run with the GST_DEBUG=*:WARN environment "
		  "variable set for more details.");
	}
	*/
	/* Free resources */
	/*
	gst_message_unref (msg);
	gst_object_unref (bus);
	gst_element_set_state (pipeline, GST_STATE_NULL);
	gst_object_unref (pipeline);
	*/
	return 0;
 }
