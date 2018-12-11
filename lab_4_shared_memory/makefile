CC = gcc
CFLAGS = -c -Wall
EXECUTABLE = receiver spammer
CONFIG_SOURCE = time_config.c
SPAM_SOURCES = $(CONFIG_SOURCE) time_spammer.c
SPAM_OBJECTS = $(SPAM_SOURCES:.c=.o)
RECV_SOURCES = $(CONFIG_SOURCE) receiver.c
RECV_OBJECTS = $(RECV_SOURCES:.c=.o)

all: $(EXECUTABLE)

receiver: $(RECV_OBJECTS)
	$(CC) $(RECV_OBJECTS) -o $@

spammer: $(SPAM_OBJECTS)
	$(CC) $(SPAM_OBJECTS) -o $@

.c.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf *.o receiver spammer *.exe
