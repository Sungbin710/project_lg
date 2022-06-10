
media_play: media_play.c
	gcc -o $@ $^ `pkg-config --cflags --libs gstreamer-video-1.0 gtk+-3.0 gstreamer-1.0` -lbsd

player_name: player_name.c
	gcc -o $@ $^ `pkg-config --cflags --libs gstreamer-1.0` -lbsd

print_list: print_list.c
	gcc -o $@ $^ 