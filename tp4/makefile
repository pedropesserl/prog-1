# Makefile de exemplo (Manual do GNU Make)
     
CFLAGS = -Wall -std=c90 -g  # flags de compilacao
LDFLAGS = -lm

CC = gcc

# arquivos-objeto
	objects = testa_conjunto.o lib_conjunto.o 
     
all: testa_conjunto.o lib_conjunto.o
	$(CC) -o testa_conjunto testa_conjunto.o lib_conjunto.o $(LDFLAGS)

lib_conjunto.o: lib_conjunto.c
	$(CC) -c $(CFLAGS) lib_conjunto.c

tp4.o: tp4.c
	$(CC) -c $(CFLAGS) testa_conjunto.c

clean:
	rm -f $(objects)


