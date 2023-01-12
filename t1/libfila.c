#include <stdio.h>
#include <stdlib.h>
#include "libfila.h"

fila_t *cria_fila() {
	fila_t *f;
	if ( !(f = malloc(sizeof(fila_t))) )
		return NULL;
	
	f->tamanho = 0;
	f->ini = NULL;
	f->fim = NULL;
	return f;
}

fila_t *destroi_fila(fila_t *f) {
	int e = 0;

	while (f->ini != NULL) {
		retira_fila(f, &e);
	}

	free(f);
	return NULL;
}

int vazia_fila(fila_t *f) {
	return f->tamanho == 0;
}

int tamanho_fila(fila_t *f) {
	return f->tamanho;
}

int insere_fila(fila_t *f, int elemento) {
	nodo_f_t *novo;
	if ( !(novo = malloc(sizeof(nodo_f_t))) )
		return 0;
	novo->elem = elemento;
	novo->prox = NULL;

	(f->tamanho)++;

	if (tamanho_fila(f) == 1) {
		f->ini = novo;
		f->fim = novo;
		return 1;
	}

	f->fim->prox = novo;
	f->fim = novo;
	return 1;
}

int retira_fila(fila_t *f, int *elemento) {
	if (vazia_fila(f))
		return 0;
	
	*elemento = f->ini->elem;
	nodo_f_t *temp = f->ini->prox;
	free(f->ini);
	f->ini = temp;
	(f->tamanho)--;
	return 1;
}

void imprime_fila(fila_t *f) {
	if (vazia_fila(f))
		return;

	nodo_f_t *atual = f->ini;
	printf("%d", atual->elem);

	int i;
	for (i = 1; i < tamanho_fila(f); i++) {
		atual = atual->prox;
		printf(" %d", atual->elem);
	}
	
	printf("\n");
}
