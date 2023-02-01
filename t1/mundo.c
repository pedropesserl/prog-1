#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "libconjunto.h"
#include "libfila.h"
#include "liblef.h"

#define MEM_ERROR_EXIT						    		  \
	do {											      \
		printf("Erro: não foi possível alocar memória."); \
		exit(1);										  \
	} while(0)

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
	conjunto_t *publico;
	fila_t *fila;
	int posx;
	int posy;
} local_t;

typedef struct {
	int tempo_atual;
	int tamanho_mundo; /* o mundo é um quadrado */
	int fim_do_mundo;
	int n_missoes;
	conjunto_t *cj_habilidades;
	int n_herois;
	int n_locais;
	heroi_t *herois;
	local_t *locais;
} mundo_t;

/* retorna um inteiro aleatório entre a e b (inclusive) */
int aleat(int a, int b) {
	return rand() % b + 1 + a;
}

/* retorna a distancia euclidiana entre dois locais em double */
double distancia(local_t loc1, local_t loc2) {
	int deltax = loc2.posx - loc1.posx;
	int deltay = loc2.posy - loc1.posy;
	int quadrado_da_distancia = deltax * deltax + deltay * deltay;
	return sqrt((double)quadrado_da_distancia);
}

int vazia_lef(lef_t *l) {
	return l->Primeiro == NULL;
}

heroi_t inicializa_heroi(int id, conjunto_t *conjunto_de_habilidades) {
	heroi_t h;
	
	h.id = id;
	h.paciencia = aleat(0, 100);
	h.idade = aleat(18, 100);
	h.exp = 0;

	if ( !(h.habilidades_do_heroi = cria_subcjt_cjt(conjunto_de_habilidades, aleat(2, 5))) )
		MEM_ERROR_EXIT;

	return h;
}

local_t inicializa_local(int id, int tamanho_mundo) {
	local_t l;
	
	l.id = id;
	l.lotacao_max = aleat(5, 30);

	if ( !(l.publico = cria_cjt(l.lotacao_max)) )
		MEM_ERROR_EXIT;

	if ( !(l.fila = cria_fila()) )
		MEM_ERROR_EXIT;

	l.posx = aleat(0, tamanho_mundo-1);
	l.posy = aleat(0, tamanho_mundo-1);

	return l;
}

/* evento_t *cria_evento(int tempo, int tipo, int dado1, int dado2) { */
/* 	evento_t *e; */
/* 	if ( !(e = malloc(sizeof(evento_t))) ) */
/* 		MEM_ERROR_EXIT; */

/* 	e->tempo = tempo; */
/* 	e->tipo = tipo; */
/* 	e->dado1 = dado1; */
/* 	e->dado2 = dado2; */

/* 	return e; */
/* } */

mundo_t *cria_mundo(lef_t *lista_de_eventos) {
	mundo_t *m;
	if ( !(m = malloc(sizeof(mundo_t))) )
		MEM_ERROR_EXIT;
	
	m->tempo_atual = 0;
	m->tamanho_mundo = 20000;
	m->fim_do_mundo = 34944;
	m->n_missoes = m->fim_do_mundo / 100;

	const int n_habilidades = 10;
	if ( !(m->cj_habilidades = cria_cjt(n_habilidades)) )
		MEM_ERROR_EXIT;
	int i;
	for (i = 0; i < n_habilidades; i++)
		insere_cjt(m->cj_habilidades, i);

	m->n_herois = n_habilidades * 5;
	m->n_locais = m->n_herois / 6;

	/* cria vetor de heróis e preenche */
	if ( !(m->herois = malloc(m->n_herois * sizeof(heroi_t))) )
		MEM_ERROR_EXIT;
	for (i = 0; i < m->n_herois; i++)
		m->herois[i] = inicializa_heroi(i, m->cj_habilidades);

	/* cria vetor de locais e preenche */
	if ( !(m->locais = malloc(m->n_locais * sizeof(local_t))) )
		MEM_ERROR_EXIT;
	for (i = 0; i < m->n_locais; i++)
		m->locais[i] = inicializa_local(i, m->tamanho_mundo);

	/* para cada herói, cria evento de chegada e insere na lef */
	/* evento_t *evento_chegada_heroi; */
	for (i = 0; i < m->n_herois; i++) {
		evento_t evento_chegada_heroi = {
			aleat(0, 96*7),
			CHEGADA,
			m->herois[i].id,
			aleat(0, m->n_locais-1)
		};
		if ( !(adiciona_ordem_lef(lista_de_eventos, &evento_chegada_heroi)) )
			MEM_ERROR_EXIT;
	}

	/* para cada missao, cria evento e insere na lef */
	for (i = 0; i < m->n_missoes; i++) {
		evento_t evento_missao = {
			aleat(0, m->fim_do_mundo),
			MISSAO,
			i,
			0
		};
		if ( !(adiciona_ordem_lef(lista_de_eventos, &evento_missao)) )
			MEM_ERROR_EXIT;
	}

	/* cria evento de fim e insere na lef */
	evento_t fim = {
		m->fim_do_mundo,
		FIM,
		0,
		0,
	};
	if ( !(adiciona_ordem_lef(lista_de_eventos, &fim)) )
		MEM_ERROR_EXIT;

	return m;
}

/* lef_t *inicializa_lista_de_eventos() */

int main() {
	srand(time(0));

	lef_t *lista_de_eventos;
	if ( !(lista_de_eventos = cria_lef()) )
		MEM_ERROR_EXIT;

	mundo_t *mundo;
	mundo = cria_mundo(lista_de_eventos);
	printf("inicializou o mundo!\n");
	printf("tamanho do mundo: %d\n", mundo->tamanho_mundo);
	printf("conjunto de habilidades do mundo:\n");
	imprime_cjt(mundo->cj_habilidades);
	
	int i = 0;
	nodo_lef_t *exemplo = lista_de_eventos->Primeiro;
	printf("eventos alocados na lef:\n");
	while (exemplo) {
		printf("tempo do evento %d: %d\n", i, exemplo->evento->tempo);
		printf(" tipo do evento %d: ", i);
		switch (exemplo->evento->tipo) {
			case CHEGADA:
				printf("CHEGADA\n");
				break;
			case SAIDA:
				printf("SAIDA\n");
				break;
			case MISSAO:
				printf("MISSAO\n");
				break;
			case FIM:
				printf("FIM\n");
				break;
			default:
				printf("deu alguma coisa muito louca aqui\n");
				break;
		}
		i++;
		exemplo = exemplo->prox;
	}

	/* ciclo */
	/* retira o primeiro evento da lista_de_eventos; */
	evento_t *evento_atual;
	evento_atual = obtem_primeiro_lef(lista_de_eventos);
	/* atualiza o estado do sistema; */
	switch (evento_atual->tipo) {
		case CHEGADA:
			break;
		case SAIDA:
			break;
		case MISSAO:
			break;
		case FIM:
			break;
	}
	/* agenda os novos eventos na lista_de_eventos; */

	lista_de_eventos = destroi_lef(lista_de_eventos);

	return 0;
}
