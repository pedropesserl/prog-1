#include <stdio.h>
#include <stdio.h>
#include "lib_conjunto.h"

/* retorna a posicao de um elemento em um conjunto.
 * como um conjunto nao necessariamente eh ordenado, eh uma busca linear.
 * se o elemento nao esta no conjunto, retorna -1. */
int busca_cjt(conjunto_t *c, int elemento) {
	int pos = -1;
	int i;
	for (i = 0; i < c->card; i++) {
		if (c->v[i] == elemento) {
			pos = i;
			break;
		}
	}
	return pos;
}

conjunto_t *cria_cjt(int max) {
	conjunto_t *c;
	c->max = max;
	c->card = 0;
	c->ptr;
	if ( !(c->v = malloc(sizeof(int) * max)) )
		return NULL;
	return c;
}

conjunto_t *destroi_cjt(conjunto_t *c) {
	c->card = 0;
	free(c->v);
	c->v = NULL;
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

	c->v[card] = elemento;
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

/* conjunto_t *diferenca_cjt(conjunto_t *c1, conjunto_t *c2) { */
	
/* } */

/* conjunto_t *interseccao_cjt(conjunto_t *c1, conjunto_t *c2) { */
	
/* } */

/* conjunto_t *uniao_cjt(conjunto_t *c1, conjunto_t *c2) { */
	
/* } */

conjunto_t *copia_cjt(conjunto_t *c) {
	conjunto_t *copia;

	if ( !(copia = cria_cjt(c->max)) )
		return NULL;
	copia->card = c->card;

	int i;
	for (i = 0; i < copia->card; i++) {
		copia->v[i] = c->v[i];
	}

	return copia;
}

/* conjunto_t *cria_subcjt_cjt(conjunto_t *c, int n) { */
	
/* } */

/* void imprime_cjt(conjunto_t *c) { */
	
/* } */

/* void inicia_iterador_cjt(conjunto_t *c) { */
	
/* } */

/* int incrementa_iterador_cjt(conjunto_t *c, int *ret_iterador) { */
	
/* } */

/* int retira_um_elemento_cjt(conjunto_t *c) { */
	
/* } */
