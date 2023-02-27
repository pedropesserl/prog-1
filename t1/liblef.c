#include <stdlib.h>
#include <string.h>
#include "liblef.h"

lef_t *cria_lef() {
    lef_t *l;
    if ( !(l = malloc(sizeof(lef_t))) )
        return NULL;

    l->Primeiro = NULL;
    
    return l;
}

lef_t *destroi_lef(lef_t *l) {
    nodo_lef_t *aux;
    while (l->Primeiro != NULL) {
        aux = l->Primeiro;
        l->Primeiro = l->Primeiro->prox;
        free(aux->evento);
        free(aux);
    }

    free(l);

    return NULL;
}

int adiciona_inicio_lef(lef_t *l, evento_t *evento) {
    nodo_lef_t *novo_nodo;
    if ( !(novo_nodo = malloc(sizeof(nodo_lef_t))) )
        return 0;
    
    evento_t *novo_evento;
    if ( !(novo_evento = malloc(sizeof(evento_t))) ) {
        free(novo_nodo);
        return 0;
    }

    memcpy(novo_evento, evento, sizeof(evento_t));

    novo_nodo->evento = novo_evento;

    novo_nodo->prox = l->Primeiro;
    l->Primeiro = novo_nodo;

    return 1;
}

int adiciona_ordem_lef(lef_t *l, evento_t *evento) {
    nodo_lef_t *novo_nodo;
    if ( !(novo_nodo = malloc(sizeof(nodo_lef_t))) )
        return 0;
    
    evento_t *novo_evento;
    if ( !(novo_evento = malloc(sizeof(evento_t))) ) {
        free(novo_nodo);
        return 0;
    }

    memcpy(novo_evento, evento, sizeof(evento_t));
    
    novo_nodo->prox = l->Primeiro;
    novo_nodo->evento = novo_evento;

    if (l->Primeiro == NULL || l->Primeiro->evento->tempo > novo_nodo->evento->tempo) {
        l->Primeiro = novo_nodo;
        return 1;
    }

    nodo_lef_t *atual = l->Primeiro;
    
    while (atual->prox && atual->prox->evento->tempo < novo_nodo->evento->tempo)
        atual = atual->prox;

    novo_nodo->prox = atual->prox;
    atual->prox = novo_nodo;

    return 1;
}

evento_t *obtem_primeiro_lef(lef_t *l) {
    if (l->Primeiro == NULL)
        return NULL;
    
    nodo_lef_t *aux = l->Primeiro;
    
    evento_t *e = l->Primeiro->evento;
    
    l->Primeiro = l->Primeiro->prox;

    free(aux);

    return e;
}
