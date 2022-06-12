CC = gcc
CFLAGS = -Wall
TARGET = main 


$(TARGET): main.o play_media.o discover_media.o playlist.o login.o interface.o
	$(CC) -o $@ $^ `pkg-config --cflags --libs gstreamer-video-1.0 gstreamer-pbutils-1.0 gtk+-3.0 gstreamer-1.0` -lbsd

play_media: play_media.c login.c interface.c 
	$(CC) -o $@ $^ `pkg-config --cflags --libs gstreamer-video-1.0 gtk+-3.0 gstreamer-1.0` -lbsd

main.o: main.c
	$(CC) -c $^ `pkg-config --cflags --libs gstreamer-video-1.0 gstreamer-pbutils-1.0 gtk+-3.0 gstreamer-1.0` -lbsd

interface.o: interface.c
	$(CC) -c $^

login.o: login.c
	$(CC) -c $^ 

playlist.o: playlist.c
	$(CC) -c $^

play_media.o: play_media.c
	$(CC) -c $^ `pkg-config --cflags --libs gstreamer-video-1.0 gtk+-3.0 gstreamer-1.0 gstreamer-pbutils-1.0` 

discover_media.o: discover_media.c
	$(CC) -c $^ `pkg-config --cflags --libs gstreamer-video-1.0 gstreamer-pbutils-1.0`	
		


server: server.c
	$(CC) $(CFLAGS) -o $@ $^


clean:
	rm -f *.o 
	rm -f $(TARGET)
