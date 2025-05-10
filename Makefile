CC = gcc
CFLAGS = -Wall -std=c99 -O2
LDFLAGS = -lm -ldl -lpthread -lGL -lrt -lX11 -lraylib -lcjson

main: main.c cJSON.c
	$(CC) $(CFLAGS) -o main main.c cJSON.c $(LDFLAGS)

clean:
	rm -f main
