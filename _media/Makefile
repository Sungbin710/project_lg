init: init.c
	gcc -o $@ $^ `pkg-config --cflags --libs gstreamer-1.0`


main: main.c
	gcc main.c -o main `pkg-config --cflags --libs gstreamer-1.0`

basic5: basic5.c
	gcc -o basic5 basic5.c `pkg-config --cflags --libs gstreamer-video-1.0 gtk+-3.0 gstreamer-1.0`

basic4: basic4.c
	gcc -o basic4 basic4.c `pkg-config --cflags --libs gstreamer-1.0`

test: test.c
	gcc -o test test.c `pkg-config --cflags --libs gstreamer-1.0`

clean:
	rm basic5
