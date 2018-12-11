CC = gcc
CFLAGS = -c -Wall
EXECUTABLE = connector spammer

all: $(EXECUTABLE)

connector: connector.c
	$(CC) connector.c -o $@

spammer: spammer.c
	$(CC) spammer.c -o $@

.c.o:
	$(CC) $(CFLAGS) $< -o $@

clean: rm -rf spammer connector *.exe
