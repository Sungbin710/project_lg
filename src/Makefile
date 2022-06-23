CC = gcc
CFLAGS = -Wall
TARGET = main 
OBJS = main.o\
	playback.o\
	discover.o\
	playlist.o\
	account.o\
	interface.o\


$(TARGET): $(OBJS) #main.o playback.o discover.o playlist.o account.o interface.o
	$(CC) $(GCOV) -o $@ $^  `pkg-config --cflags --libs gstreamer-video-1.0 gstreamer-pbutils-1.0 gtk+-3.0 gstreamer-1.0`

main.o: main.c
	$(CC) -c $^ `pkg-config --cflags --libs gstreamer-video-1.0 gstreamer-pbutils-1.0 gtk+-3.0 gstreamer-1.0`

interface.o: interface.c
	$(CC) -c $^ `pkg-config --cflags --libs glib-2.0`

account.o: account.c
	$(CC) -c $^ `pkg-config --cflags --libs glib-2.0`

playlist.o: playlist.c
	$(CC) -c $^

playback.o: playback.c
	$(CC) -c $^ `pkg-config --cflags --libs gstreamer-video-1.0 gtk+-3.0 gstreamer-1.0 gstreamer-pbutils-1.0` 

discover.o: discover.c
	$(CC) -c $^ `pkg-config --cflags --libs glib-2.0 gstreamer-video-1.0 gstreamer-pbutils-1.0`	

server: server.c
	$(CC) $(CFLAGS) -o $@ $^


clean:
	rm -f *.o 
	rm -f $(TARGET)
	rm -f server
