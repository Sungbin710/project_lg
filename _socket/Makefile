CC = gcc
CFLAGS = -Wall
TARGET = play_media



play_media: play_media.c login.c connect.c
	$(CC) -o $@ $^ `pkg-config --cflags --libs gstreamer-video-1.0 gtk+-3.0 gstreamer-1.0` -lbsd
 
		
# play_media: play_media.o login.o connect.o
# 	$(CC) -o $@ $^

# play_media.o: play_media.c
# 	$(CC) -o $@ $^ `pkg-config --cflags --libs gstreamer-video-1.0 gtk+-3.0 gstreamer-1.0` -lbsd


# login.o: login.c
# 	$(CC) -o $@ $^

# connect.o: connect.c
# 	$(CC) -o $@ $^


# client: client.o login.o 
# 	$(CC) $(CFLAGS) -o $@ $^ 

# client.o: client.c
# 	$(CC) $(CFLAGS) -c $^ `pkg-config --cflags --libs gstreamer-video-1.0 gtk+-3.0 gstreamer-1.0` -lbsd
	

# play_media.o: play_media.c
# 	$(CC) $(CFLAGS) -c $^ `pkg-config --cflags --libs gstreamer-video-1.0 gtk+-3.0 gstreamer-1.0`

# login.o: login.c
# 	$(CC) $(CFLAGS) -c $^

# server: server.c
# 	$(CC) $(CFLAGS) -o $@ $^


clean:
	rm -f *.o 
	rm -f $(TARGET) 
