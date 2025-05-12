CC = gcc
CFLAGS = -Wall -std=c99 -O2
LDFLAGS = -lm -ldl -lpthread -lGL -lrt -lX11

UNAME := $(shell uname -s)
ifeq ($(UNAME),Linux)
    DISTRO := $(shell lsb_release -i -s)
    ifeq ($(DISTRO),Fedora)
        LDFLAGS += -lraylib
        LIBRARY_PATH = /usr/local/lib64
    else
        LDFLAGS += $(shell pkg-config --libs raylib)
    endif
endif

SRC = main.c cJSON.c
OBJ = $(SRC:.c=.o)

%.o: %.c
    $(CC) $(CFLAGS) -c $< -o $@

jogo: $(OBJ)
    $(CC) $(CFLAGS) -o jogo $(OBJ) $(LDFLAGS)

clean:
    rm -f jogo $(OBJ)
