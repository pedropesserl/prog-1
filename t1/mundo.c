#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "libconjunto.h"
#include "libfila.h"
#include "liblef.h"

#define CHEGADA 0
#define SAIDA 1
#define MISSAO 2
#define FIM 3

typedef struct {
	int id;
	int paciencia;
	int idade;
	int exp;
	conjunto_t *habilidades_do_heroi;
} heroi_t;

typedef struct {
	int id;
	int lotacao_max;
	conjunto_t *herois_no_local;
	fila_t *fila;
	int localizacaox;
	int localizacaoy;
} local_t;

typedef struct {
	int tempo_atual;
	heroi_t *herois;
	local_t *locais;
	conjunto_t *habilidades;
	int n_herois;
	int n_locais;
	int n_tamanho_mundo; /* o mundo é um quadrado */
} mundo_t;

/* retorna um inteiro aleatório entre a e b (inclusive) */
int aleat(int a, int b) {
	return rand() % b + 1 + a;
}

/* retorna a distancia euclidiana entre duas coordenadas em double */
double distancia(coordenada_t loc1, coordenada_t loc2) { 
	int soma = (loc2.x - loc1.x)*(loc2.x - loc1.x) + (loc2.y - loc1.y)*(loc2.y - loc1.y);
	oreturn sqrt((double)soma);
}

heroi_t *inicializa_heroi(int id, conjunto_t *conjunto_de_habilidades) {
	heroi_t *h;
	if( !(h = malloc(sizeof(heroi_t))) )
		return NULL;

	if( !(h->habilidades_do_heroi = cria_subcjt_cjt(conjunto_de_habilidades, aleat(2, 5))) ) {
		free(h);
		return NULL;
	}

	h->id = id;
	h->paciencia = aleat(0, 100);
	h->idade = aleat(18, 100);
	h->exp = 0;

	return h;
}

local_t *inicializa_local(int id, int tamanho_mundo) {
	local_t *l;
	if( !(l = malloc(sizeof(local_t))) )
		return NULL;
	
	l->localizacaox = aleat(0, tamanho_mundo-1);
	l->localizacaoy = aleat(0, tamanho_mundo-1);
	/* TODO: terminar essa função */	
	return l;
}

mundo_t *inicializa_mundo() {
	mundo_t *m;
	if( !(m = malloc(sizeof(mundo_t))) )
		return NULL;

	m->tempo_atual = 0;
	if( !(m->habilidades = cria_cjt()) ) {
		free(m);
		return NULL;
	}
	/* TODO: terminar essa função */	

	return m;
}

int main() {
	srand(0);

	return 0;
}
