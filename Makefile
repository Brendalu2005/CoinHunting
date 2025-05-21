
EXEC = main

SRC_DIR = src
INCLUDE_DIR = include

SRCS = $(SRC_DIR)/main.c \
       $(SRC_DIR)/ghost.c \
       $(SRC_DIR)/menu.c \
       $(SRC_DIR)/cJSON.c \
       $(SRC_DIR)/game.c \
       $(SRC_DIR)/moeda.c \
       $(SRC_DIR)/selecao.c \
       $(SRC_DIR)/ranking.c \
       $(SRC_DIR)/audio.c \
       $(SRC_DIR)/fixo.c 

CC = gcc

INCLUDES = -I$(SRC_DIR) -I$(INCLUDE_DIR)

CFLAGS = -Wall -std=c99 -O2 $(INCLUDES) -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

all: $(EXEC)

$(EXEC): $(SRCS)
	$(CC) $(SRCS) -o $(EXEC) $(CFLAGS)

clean:
	rm -f $(EXEC)

