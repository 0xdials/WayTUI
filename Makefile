CC = gcc
CFLAGS = -Wall -Wextra
LDFLAGS = -lncurses -lpam

BIN = waytui
PAM_DIR = /etc/pam.d/

all: $(BIN)

$(BIN): main.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

install:
	cp $(BIN) /usr/local/bin/
	cp pam/waytui $(PAM_DIR)

clean:
	rm -f $(BIN)
