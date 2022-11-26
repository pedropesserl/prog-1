#include <stdio.h>
#include <stdlib.h>
#include "lib_conjunto.h"

/* retorna a posicao de um elemento em um conjunto ([0..card-1]).
 * como um conjunto nao necessariamente eh ordenado, eh uma busca linear.
 * se o elemento nao esta no conjunto, retorna -1. */
int busca_cjt(conjunto_t *c, int elemento) {
	int pos = 0;
	while (pos < c->card && c->v[pos] != elemento)
		pos++;

	return pos >= c->card ? -1 : pos;
}

conjunto_t *cria_cjt(int max) {
	conjunto_t *c;
	if ( !(c = malloc(sizeof(conjunto_t))) )
		return NULL;
	c->max = max;
	c->card = 0;
	c->ptr = 0;
	if ( !(c->v = malloc(sizeof(int) * max)) )
		return NULL;
	return c;
}

conjunto_t *destroi_cjt(conjunto_t *c) {
	c->card = 0;
	free(c->v);
	c->v = NULL;
	free(c);
	return NULL;
}

int vazio_cjt(conjunto_t *c) {
	return c->card == 0;	
}

int cardinalidade_cjt(conjunto_t *c) {
	return c->card;
}

int insere_cjt(conjunto_t *c, int elemento) {
	if (c->card == c->max)
		return 0;

	if (pertence_cjt(c, elemento))
		return 1;

	c->v[c->card] = elemento;
	c->card++;
	return 1;
}

int retira_cjt(conjunto_t *c, int elemento) {
	int pos = busca_cjt(c, elemento);
	if (pos == -1)
		return 0;

	int i;
	for(i = pos; i < c->card-1; i++) {
		c->v[i] = c->v[i + 1];
	}

	c->card--;
	return 1;
}

int pertence_cjt(conjunto_t *c, int elemento) {
	return busca_cjt(c, elemento) != -1;
}

int contido_cjt(conjunto_t *c1, conjunto_t *c2) {
	int i;
	for (i = 0; i < c1->card; i++) {
		if (!(pertence_cjt(c2, c1->v[i])))
			return 0;
	}
	return 1;
}

int sao_iguais_cjt(conjunto_t *c1, conjunto_t *c2) {
	return c1->card == c2->card && contido_cjt(c1, c2);
}

conjunto_t *diferenca_cjt(conjunto_t *c1, conjunto_t *c2) {
	conjunto_t *dif;
	if ( !(dif = cria_cjt(c1->max)) )
		return NULL;

	int i;
	for (i = 0; i < c1->card; i++) {
		if (!(pertence_cjt(c2, c1->v[i])))
			insere_cjt(dif, c1->v[i]);
	}

	return dif;
}

conjunto_t *interseccao_cjt(conjunto_t *c1, conjunto_t *c2) {
	conjunto_t *inter;
	if ( !(inter = cria_cjt(c1->max)) )
		return NULL;
	
	int i;
	for (i = 0; i < c1->card; i++) {
		if (pertence_cjt(c2, c1->v[i]))
			insere_cjt(inter, c1->v[i]);
	}

	return inter;
}

conjunto_t *uniao_cjt(conjunto_t *c1, conjunto_t *c2) {
	conjunto_t *uniao;
	if ( !(uniao = cria_cjt(c1->max)) )
		return NULL;

	/* para nao ter que alocar um conjunto novo, a funcao usa ponteiros para
	 * ponteiro de conjunto */
	conjunto_t **menor_cjt = c1->card < c2->card ? &c1 : &c2;
	conjunto_t **maior_cjt = c1->card > c2->card ? &c1 : &c2;

	int i;
	for (i = 0; i < (*menor_cjt)->card; i++) {
		insere_cjt(uniao, c1->v[i]);
		insere_cjt(uniao, c2->v[i]);
	}

	for (; i < (*maior_cjt)->card; i++)
		insere_cjt(uniao, (*maior_cjt)->v[i]);

	return uniao;
}

conjunto_t *copia_cjt(conjunto_t *c) {
	conjunto_t *copia;
	if ( !(copia = cria_cjt(c->max)) )
		return NULL;

	int i;
	for (i = 0; i < c->card; i++) {
		insere_cjt(copia, c->v[i]);
	}

	return copia;
}

conjunto_t *cria_subcjt_cjt(conjunto_t *c, int n) {
	if (n >= c->card)
		return copia_cjt(c);

	conjunto_t *sub;
	if ( !(sub = cria_cjt(c->max)) )
		return NULL;

	conjunto_t *disponiveis;
	if ( !(disponiveis = copia_cjt(c)) )
		return NULL;

	int i;
	int aleat;
	for (i = 0; i < n; i++) {
		aleat = rand() % disponiveis->card;
		insere_cjt(sub, disponiveis->v[aleat]);
		retira_cjt(disponiveis, disponiveis->v[aleat]);
	}
	
	destroi_cjt(disponiveis);
	return sub;
}

/* funcao auxiliar para a ordenacao */
void troca(int *a, int *b) {
	int temp = *a;
	*a = *b;
	*b = temp;
}

/* implementacao do bubble sort */
conjunto_t *ordena_cjt(conjunto_t *c) {
	conjunto_t *ord;
	if ( !(ord = copia_cjt(c)) )
		return NULL;

	int tam = ord->card;
	int trocou = 1;
	int i;
	while (trocou) { 	/* trocou eh uma flag para sair do loop quando ja */
		trocou = 0;		/* estiver ordenado (nao trocou = ordenado)       */
		for (i = 0; i < tam-1; i++) {
			if (ord->v[i+1] < ord->v[i]) {
				troca(&ord->v[i+1], &ord->v[i]);
				trocou = 1;
			}
		}
		tam--;
	}
	return ord;
}

void imprime_cjt(conjunto_t *c) {
	if (vazio_cjt(c)) {
		printf("Conjunto vazio.\n");
		return;
	}

	conjunto_t *ord;
	if ( !(ord = ordena_cjt(c)) ) {
		printf("Erro na alocacao do conjunto.");
		return;
	}

	int i;
	for (i = 0; i < ord->card-1; i++)
		printf("%d ", ord->v[i]);
	printf("%d\n", ord->v[ord->card-1]);
	
	destroi_cjt(ord);
}

void inicia_iterador_cjt(conjunto_t *c) {
	c->ptr = 0;
}

int incrementa_iterador_cjt(conjunto_t *c, int *ret_iterador) {
	*ret_iterador = c->v[c->ptr];
	c->ptr++;
	if (c->ptr > c->card)
		return 0;
	return 1;
}

int retira_um_elemento_cjt(conjunto_t *c) {
	int aleat = rand() % c->card;
	int retirar = c->v[aleat];
	int i;
	for (i = aleat; i < c->card-1; i++)
		c->v[i] = c->v[i + 1];
	c->card--;
	return retirar;
}
