CC 		= gcc
CFLAGS 	= -Wall
LDFLAGS	= -lm -lpthread
SOURCES	= insuline.c server.c struct_for_client.c
OBJECTS	= $(SOURCES:.c=.o)
TARGET		= app

server: insuline.o server.o
	$(CC) $^ $(CFLAGS) $(LDFLAGS) -o $@
	
client: struct_for_client.o
	$(CC) struct_for_client.o -o client

clear:
	rm -rf $(OBJECTS) $(TARGET) *.o
