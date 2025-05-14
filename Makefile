CC = gcc
CFLAGS = -Wall -std=c99 -O2
LDFLAGS = -lraylib -lm -ldl -lpthread -lGL -lrt -lX11

main: main.c cJSON.c ghost.c
	$(CC) $(CFLAGS) -o main main.c menu.c ghost.c cJSON.c $(LDFLAGS)

clean:
	rm -f main
