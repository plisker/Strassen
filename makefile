CC = gcc
CFLAGS=	-g -Wall -std=c99

strassen: 
	$(CC) $(CFLAGS) -o strassen strassen.c -lm

clean:
	rm strassen