CC = gcc
CFLAGS = -Wall -Wextra
LIBS = -lpam -lpam_misc -lncurses

SRC = main.c seatd.c tui.c
OBJ = $(SRC:.c=.o)

all: waytui

waytui: $(OBJ)
	$(CC) $(CFLAGS) -o waytui $(OBJ) $(LIBS)

clean:
	rm -f waytui $(OBJ)

