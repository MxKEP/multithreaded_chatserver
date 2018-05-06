CC = gcc
FLAGS = -pthread
COMPILE = $(CC) $(FLAGS)

all: client server serverlib clientlib

client: client.c serverlib.o clientlib.o
	$(COMPILE) -o client client.c serverlib.o clientlib.o

server: server.c serverlib.o clientlib.o
	$(COMPILE) -o server server.c serverlib.o clientlib.o

serverlib: serverlib.c server.h client.h
	$(COMPILE) -c serverlib.c

clientlib: clientlib.c client.h server.h
	$(COMPILE) -c clientlib.c

clean:
	rm -f client server *.o
