# Makefile para o projeto racionais_2

OPCOES = -Wall -std=c90 -g
CC = gcc

tp2: tp2.o lib_racionais.o
	$(CC) -o tp2 tp2.o lib_racionais.o

tp2.o: tp2.c
	$(CC) -c $(OPCOES) tp2.c

lib_racionais.o: lib_racionais.c
	$(CC) -c $(OPCOES) lib_racionais.c

clean:
	rm -f ./*.o tp2
