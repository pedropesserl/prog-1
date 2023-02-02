#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "libconjunto.h"
#include "libfila.h"
#include "liblef.h"

#define MEM_ERROR_EXIT												\
	do {															\
		fprintf(stderr, "Erro: não foi possível alocar memória.");	\
		exit(1);													\
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
	conjunto_t **missoes;
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

int max(int a, int b) {
	return a > b ? a : b; 
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
		evento_t evento_chegada_heroi = { aleat(0, 96*7), CHEGADA, m->herois[i].id, aleat(0, m->n_locais-1) };
		adiciona_ordem_lef(lista_de_eventos, &evento_chegada_heroi);
	}

	/* cria vetor de missões */
	if ( !(m->missoes = malloc(m->n_missoes * sizeof(conjunto_t*))) ) {
		MEM_ERROR_EXIT;
	}
	/* preenche vetor de missões, inicialmente com NULL;
	 * para cada missao, cria evento e insere na lef */
	for (i = 0; i < m->n_missoes; i++) {
		m->missoes[i] = NULL;
		evento_t evento_missao = { aleat(0, m->fim_do_mundo), MISSAO, i, 0 };
		adiciona_ordem_lef(lista_de_eventos, &evento_missao);
	}

	/* cria evento de fim e insere na lef */
	evento_t fim = { m->fim_do_mundo, FIM, 0, 0 };
	adiciona_ordem_lef(lista_de_eventos, &fim);

	return m;
}

int local_lotado(int id_local, mundo_t *m) {
	return cardinalidade_cjt(m->locais[id_local].publico) >= m->locais[id_local].lotacao_max;
}

int heroi_tem_paciencia(int id_heroi, int id_local, mundo_t *m) {
	return m->herois[id_heroi].paciencia/4 > tamanho_fila(m->locais[id_local].fila);
}

int velocidade_heroi(heroi_t heroi) {
	return 100 - max(0, heroi.idade - 40);
}

conjunto_t *escolhe_menor_equipe(conjunto_t *missao, int id_missao, mundo_t *m, local_t *local_encontrado) {
	conjunto_t *menor;
	if ( !(menor = cria_cjt(cardinalidade_cjt(m->cj_habilidades))) )
		MEM_ERROR_EXIT;
	conjunto_t *uniao_old;
	conjunto_t *uniao;
	int i, j, id_heroi_atual;
	
	for (i = 0; i < m->n_locais; i++) {
		if (vazio_cjt(m->locais[i].publico)) continue;

		/* copia o conjunto de habilidades do primeiro herói do local atual */
		inicia_iterador_cjt(m->locais[i].publico);
		incrementa_iterador_cjt(m->locais[i].publico, &id_heroi_atual);
		if ( !(uniao = copia_cjt(m->herois[id_heroi_atual].habilidades_do_heroi)) )
			MEM_ERROR_EXIT;

		/* faz a união entre os conjuntos de habilidades de todos os heróis do local atual */
		for (j = 1; j < cardinalidade_cjt(m->locais[i].publico); j++) {
			uniao_old = uniao;
			incrementa_iterador_cjt(m->locais[i].publico, &id_heroi_atual);
			uniao = uniao_cjt(uniao, m->herois[id_heroi_atual].habilidades_do_heroi);
			uniao_old = destroi_cjt(uniao_old);
		}

		printf("%6d:MISSAO %4d HAB_EQL %d:", m->tempo_atual, id_missao, m->locais[i].id);
		imprime_cjt(uniao);

		/* compara se o conjunto construído contém a missao e é menor que o achado anteriormente */
		if (contido_cjt(missao, uniao)) {
			if (vazio_cjt(menor) || cardinalidade_cjt(uniao) < cardinalidade_cjt(menor)) {
				menor = destroi_cjt(menor);
				if ( !(menor = copia_cjt(uniao)) )
					MEM_ERROR_EXIT;
				*local_encontrado = m->locais[i];
			}
		}
		uniao = destroi_cjt(uniao);
	}
	
	return menor;
}

void trata_evento_chegada(int id_heroi, int id_local, mundo_t *m, lef_t *lista_de_eventos) {
	printf("%6d:CHEGA HEROI %2d Local %d (%2d/%2d), ",
			m->tempo_atual,
			id_heroi,
			id_local,
			cardinalidade_cjt(m->locais[id_local].publico),
			m->locais[id_local].lotacao_max);

	if (local_lotado(id_local, m)) {
		if (heroi_tem_paciencia(id_heroi, id_local, m)) {
			insere_fila(m->locais[id_local].fila, id_heroi);
			printf("FILA %d\n", tamanho_fila(m->locais[id_local].fila));
			return;
		}
		printf("DESISTE\n");
		evento_t saida = { m->tempo_atual, SAIDA, id_heroi, id_local };
		adiciona_ordem_lef(lista_de_eventos, &saida);
		return;
	}

	printf("ENTRA\n");
	insere_cjt(m->locais[id_local].publico, id_heroi);
	int t_permanencia_local = max(1, m->herois[id_heroi].paciencia/10 + aleat(-2,6));
	evento_t saida = { m->tempo_atual + t_permanencia_local, SAIDA, id_heroi, id_local };
	adiciona_ordem_lef(lista_de_eventos, &saida);
	return;
}

int main() {
	/* srand(time(0)); */
	srand(0);

	lef_t *lista_de_eventos;
	if ( !(lista_de_eventos = cria_lef()) )
		MEM_ERROR_EXIT;

	mundo_t *mundo = cria_mundo(lista_de_eventos);
	
	evento_t *evento_atual;
	/* ciclo da simulação */
	while ( lista_de_eventos && (evento_atual = obtem_primeiro_lef(lista_de_eventos)) ) {
		mundo->tempo_atual = evento_atual->tempo;

		/* trata o evento e atualiza estado do sistema */
		int id_heroi, id_local, id_missao, i;
		switch (evento_atual->tipo) {
			case CHEGADA:
				trata_evento_chegada(evento_atual->dado1, evento_atual->dado2, mundo, lista_de_eventos);
				break;

			case SAIDA: ;
				id_heroi = evento_atual->dado1;
				id_local = evento_atual->dado2;
				printf("%6d:SAIDA HEROI %2d Local %d (%2d/%2d)",
					mundo->tempo_atual,
					id_heroi,
					id_local,
					cardinalidade_cjt(mundo->locais[id_local].publico),
					mundo->locais[id_local].lotacao_max);

				if (pertence_cjt(mundo->locais[id_local].publico, id_heroi)) {
					retira_cjt(mundo->locais[id_local].publico, id_heroi);
					if (!vazia_fila(mundo->locais[id_local].fila)) {
						int id_heroi_fila;
						retira_fila(mundo->locais[id_local].fila, &id_heroi_fila);
						printf(", REMOVE FILA HEROI %d", id_heroi_fila);
						evento_t chegada_heroi_fila = { mundo->tempo_atual, CHEGADA, id_heroi_fila, id_local };
						adiciona_inicio_lef(lista_de_eventos, &chegada_heroi_fila);
					}
				}
				printf("\n");

				int id_local_destino = aleat(0, mundo->n_locais-1);
				int t_deslocamento = distancia(mundo->locais[id_local], mundo->locais[id_local_destino]) / velocidade_heroi(mundo->herois[id_heroi]);
				evento_t chegada_heroi = { mundo->tempo_atual + t_deslocamento/15, CHEGADA, id_heroi, id_local_destino };
				adiciona_ordem_lef(lista_de_eventos, &chegada_heroi);
				break;

			case MISSAO: ;
				id_missao = evento_atual->dado1;
				local_t local_encontrado;
				conjunto_t *missao;
				if (mundo->missoes[id_missao] == NULL)
					if ( !(mundo->missoes[id_missao] = cria_subcjt_cjt(mundo->cj_habilidades, aleat(3, 6))) )
						MEM_ERROR_EXIT;
				
				missao = mundo->missoes[id_missao];
				printf("%6d:MISSAO %4d HAB_REQ ", mundo->tempo_atual, id_missao);
				imprime_cjt(missao);
				
				conjunto_t *equipe_escolhida = escolhe_menor_equipe(missao, id_missao, mundo, &local_encontrado);

				printf("%6d:MISSAO %4d ", mundo->tempo_atual, id_missao);
				if (vazio_cjt(equipe_escolhida)) {
					printf("IMPOSSIVEL\n");
					evento_t nova_tentativa = { aleat(mundo->tempo_atual, mundo->fim_do_mundo), MISSAO, id_missao, 0 };
					adiciona_ordem_lef(lista_de_eventos, &nova_tentativa);
				} else {
					printf("HER_EQS %d:", local_encontrado.id);
					imprime_cjt(local_encontrado.publico);

					int id_heroi_encontrado;
					inicia_iterador_cjt(local_encontrado.publico);
					for (i = 0; i < cardinalidade_cjt(local_encontrado.publico); i++) {
						incrementa_iterador_cjt(local_encontrado.publico, &id_heroi_encontrado);
						(mundo->herois[id_heroi_encontrado].exp)++;
					}
					missao = destroi_cjt(missao);
					mundo->missoes[id_missao] = NULL;
				}
				equipe_escolhida = destroi_cjt(equipe_escolhida);
				break;

			case FIM:
				printf("%6d:FIM\n", mundo->tempo_atual);
				for (i = 0; i < mundo->n_herois; i++) {
					printf("HEROI %2d EXPERIENCIA %2d\n", mundo->herois[i].id, mundo->herois[i].exp);
					mundo->herois[i].habilidades_do_heroi = destroi_cjt(mundo->herois[i].habilidades_do_heroi);
				}

				for (i = 0; i < mundo->n_locais; i++) {
					mundo->locais[i].publico = destroi_cjt(mundo->locais[i].publico);
					mundo->locais[i].fila = destroi_fila(mundo->locais[i].fila);
				}
				
				free(mundo->herois);
				free(mundo->locais);
				for (i = 0; i < mundo->n_missoes; i++)
					if (mundo->missoes[i] != NULL)
						mundo->missoes[i] = destroi_cjt(mundo->missoes[i]);
				free(mundo->missoes);
				mundo->cj_habilidades = destroi_cjt(mundo->cj_habilidades);
				free(mundo);
				lista_de_eventos = destroi_lef(lista_de_eventos);
				break;
		}

		free(evento_atual);
	}

	return 0;
}
