
main: main.o discover_media.o
	gcc -o $@ $^ `pkg-config --cflags --libs gstreamer-1.0 gstreamer-pbutils-1.0`
#main: main.c discover_media.c
#	gcc -o $@ $^ `pkg-config --cflags --libs gstreamer-1.0 gstreamer-pbutils-1.0`

discover_media.o: discover_media.c
	gcc -c $^ `pkg-config --cflags --libs gstreamer-1.0 gstreamer-pbutils-1.0`

main.o: main.c 
	gcc -c $^ `pkg-config --cflags --libs gstreamer-1.0 gstreamer-pbutils-1.0`

media_play: media_play.c
	gcc -o $@ $^ `pkg-config --cflags --libs gstreamer-video-1.0 gtk+-3.0 gstreamer-1.0` -lbsd

player_name: player_name.c
	gcc -o $@ $^ `pkg-config --cflags --libs gstreamer-1.0` -lbsd

print_list: print_list.c
	gcc -o $@ $^ 

clean:
	rm *.o main
