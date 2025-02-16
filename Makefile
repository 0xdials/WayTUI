CC = gcc
CFLAGS = -Wall -lncurses -lpam
SOURCES = main.c seatd.c tui.c
OBJECTS = $(SOURCES:.c=.o)
EXEC = waytui

$(EXEC): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(EXEC)

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

