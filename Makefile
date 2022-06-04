CC = gcc
CFLAGS = -Wall
TARGET = client

all: client server

client: client.o login.o
	$(CC) $(CFLAGS) -o client client.o login.o

client.o: client.c
	gcc $(CFLAGS) -c client.c

login.o: login.c
	gcc $(CFLAGS) -c login.c

server:
	gcc $(CFLAGS) -o server server.c


clean:
	rm -f *.o 
	rm -f $(TARGET) 
