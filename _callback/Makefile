all:
	gcc -o main main.c -pthread `pkg-config --cflags --libs glib-2.0 gobject-2.0`
	gcc -o server server.c