#include <stdio.h>
#include <stdlib.h>
#include "libconjunto.h"

/* retorna a posicao de um elemento em um conjunto ([0..card-1]).
 * como estamos trabalhando com conjuntos ordenados, eh uma busca binaria.
 * se o elemento nao esta no conjunto, retorna -1. */
int busca_cjt(conjunto_t *c, int elemento) {
    int inicio = 0;
    int fim = cardinalidade_cjt(c) - 1;
    int meio = fim / 2;

    while (inicio <= fim && c->v[meio] != elemento) {
        if (elemento > c->v[meio]) {
            inicio = meio + 1;
        } else {
            fim = meio - 1;
        }
        meio = (inicio + fim) / 2;
    }

    return inicio > fim ? -1 : meio;
}

conjunto_t *cria_cjt(int max) {
    conjunto_t *c;
    if ( !(c = malloc(sizeof(conjunto_t))) )
        return NULL;
    c->max = max;
    c->card = 0;
    c->ptr = 0;
    if ( !(c->v = malloc(sizeof(int) * max)) ) {
        free(c);
        return NULL;
    }
    return c;
}

conjunto_t *destroi_cjt(conjunto_t *c) {
    free(c->v);
    c->v = NULL;
    free(c);
    return NULL;
}

int vazio_cjt(conjunto_t *c) {
    return cardinalidade_cjt(c) == 0;    
}

int cardinalidade_cjt(conjunto_t *c) {
    return c->card;
}

int insere_cjt(conjunto_t *c, int elemento) {
    if (cardinalidade_cjt(c) == c->max)
        return 0;

    if (pertence_cjt(c, elemento))
        return 1;

    int i = cardinalidade_cjt(c);
    while (i > 0 && c->v[i - 1] > elemento) {
        c->v[i] = c->v[i - 1];
        i--;
    }
    
    c->v[i] = elemento;
    c->card++;
    return 1;
}

int retira_cjt(conjunto_t *c, int elemento) {
    if (!(pertence_cjt(c, elemento)))
        return 0;

    int i;
    for(i = busca_cjt(c, elemento); i < cardinalidade_cjt(c)-1; i++) {
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
    for (i = 0; i < cardinalidade_cjt(c1); i++) {
        if (!(pertence_cjt(c2, c1->v[i])))
            return 0;
    }
    return 1;
}

int sao_iguais_cjt(conjunto_t *c1, conjunto_t *c2) {
    return cardinalidade_cjt(c1) == cardinalidade_cjt(c2) && contido_cjt(c1, c2);
}

conjunto_t *diferenca_cjt(conjunto_t *c1, conjunto_t *c2) {
    conjunto_t *dif;
    if ( !(dif = cria_cjt(c1->max)) )
        return NULL;

    int i;
    for (i = 0; i < cardinalidade_cjt(c1); i++) {
        if (!(pertence_cjt(c2, c1->v[i])))
            insere_cjt(dif, c1->v[i]);
    }

    return dif;
}

conjunto_t *interseccao_cjt(conjunto_t *c1, conjunto_t *c2) {
    conjunto_t *inter;
    if ( !(inter = cria_cjt(c1->max)) )
        return NULL;
    
    conjunto_t **menor_cjt = cardinalidade_cjt(c1) < cardinalidade_cjt(c2) ? &c1 : &c2;
    conjunto_t **maior_cjt = cardinalidade_cjt(c1) > cardinalidade_cjt(c2) ? &c1 : &c2;

    int i;
    for (i = 0; i < cardinalidade_cjt(*menor_cjt); i++) {
        if (pertence_cjt(*maior_cjt, (*menor_cjt)->v[i]))
            insere_cjt(inter, (*menor_cjt)->v[i]);
    }

    return inter;
}

conjunto_t *uniao_cjt(conjunto_t *c1, conjunto_t *c2) {
    conjunto_t *uniao;
    if ( !(uniao = cria_cjt(c1->max)) )
        return NULL;

    inicia_iterador_cjt(c1);
    inicia_iterador_cjt(c2);

    while (c1->ptr < cardinalidade_cjt(c1) && c2->ptr < cardinalidade_cjt(c2)) {
        if (c1->v[c1->ptr] < c2->v[c2->ptr]) {
            insere_cjt(uniao, c1->v[c1->ptr]);
            c1->ptr++;
        } else {
            insere_cjt(uniao, c2->v[c2->ptr]);
            c2->ptr++;
        }
    }

    conjunto_t **ainda_falta = c1->ptr == cardinalidade_cjt(c1) ? &c2 : &c1;

    int i;
    for (i = (*ainda_falta)->ptr; i < cardinalidade_cjt(*ainda_falta); i++) {
        insere_cjt(uniao, (*ainda_falta)->v[i]);
    }

    inicia_iterador_cjt(c1);
    inicia_iterador_cjt(c2);

    return uniao;
}

conjunto_t *copia_cjt(conjunto_t *c) {
    conjunto_t *copia;
    if ( !(copia = cria_cjt(c->max)) )
        return NULL;

    int i;
    for (i = 0; i < cardinalidade_cjt(c); i++) {
        insere_cjt(copia, c->v[i]);
    }

    return copia;
}

conjunto_t *cria_subcjt_cjt(conjunto_t *c, int n) {
    if (n >= cardinalidade_cjt(c))
        return copia_cjt(c);

    conjunto_t *sub;
    if ( !(sub = cria_cjt(c->max)) )
        return NULL;

    conjunto_t *disponiveis;
    if ( !(disponiveis = copia_cjt(c)) ) {
        free(sub);
        return NULL;
    }

    int i;
    int aleat;
    for (i = 0; i < n; i++) {
        aleat = rand() % cardinalidade_cjt(disponiveis);
        insere_cjt(sub, disponiveis->v[aleat]);
        retira_cjt(disponiveis, disponiveis->v[aleat]);
    }
    
    destroi_cjt(disponiveis);
    return sub;
}

void imprime_cjt(conjunto_t *c) {
    if (vazio_cjt(c)) {
        printf("Conjunto vazio.\n");
        return;
    }

    int i;
    printf("%d", c->v[0]);
    for (i = 1; i < cardinalidade_cjt(c); i++)
        printf(" %d", c->v[i]);
    printf("\n");
}

void inicia_iterador_cjt(conjunto_t *c) {
    c->ptr = 0;
}

int incrementa_iterador_cjt(conjunto_t *c, int *ret_iterador) {
    *ret_iterador = c->v[c->ptr];
    c->ptr++;
    if (c->ptr > cardinalidade_cjt(c))
        return 0;
    return 1;
}

int retira_um_elemento_cjt(conjunto_t *c) {
    int aleat = rand() % cardinalidade_cjt(c);
    int retirar = c->v[aleat];
    int i;
    for (i = aleat; i < cardinalidade_cjt(c)-1; i++)
        c->v[i] = c->v[i + 1];
    c->card--;
    return retirar;
}
