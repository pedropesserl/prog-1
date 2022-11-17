#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "lib_racionais.h"
#define MAX 100

/* le um inteiro na faixa [0..MAX-1] */
int ler_tamanho() {
	printf("Insira o tamanho do vetor (entre 0 e %d):\n", MAX-1);
	int n;
	scanf("%d", &n);
	while (n < 0 || n > MAX-1) {
		printf("Por favor, insira um inteiro entre 0 e %d:\n", MAX-1);
		scanf("%d", &n);
	}
	return n;
}

/* imprime os racionais apontados pelo vetor de ponteiros para racionais */
void imprimir_vetor_racional(racional **vet, int tam) {
	int i;
	for (i = 0; i < tam-1; i++) {
		imprimir_r(*(vet + i));
		printf(" ");
	}
	imprimir_r(*(vet + tam-1));     /* por motivos de formatacao da saida, */
	printf("\n");				    /* imprime o ultimo numero separadamente */
}

/* retorna um vetor de tam ponteiros para numeros racionais validos gerados aleatoriamente */
/* retorna NULL em caso de falha */
racional **aleatorio_vetor_racional(int tam) {
	racional **vet;
	if ( !(vet = malloc(tam * sizeof(racional*))) )
		return NULL;
	int i;
	for (i = 0; i < tam; i++)
		*(vet + i) = sortear_r();
	return vet;
}

/* inverte dois ponteiros para racional */
void inverte_ponteiros(racional **r1, racional **r2) {
	racional *temp = *r1;
	*r1 = *r2;
	*r2 = temp;
}

/* retorna um vetor de tam ponteiros para numeros racionais que apontam em ordem crescente para os 
 * racionais apontados pelo vetor recebido no parametro. */
/* essa funcao eh uma implementacao do bubble sort */
racional **ordenar_vetor_racional(racional **vet, int tam) {
	racional **ord;
	if ( !(ord = malloc(tam * sizeof(racional*))) )
		return NULL;
	int i;
	for (i = 0; i < tam; i++) {
		*(ord + i) = *(vet + i);
	}

	int swaps = 1;
	while (swaps) {
		swaps = 0;
		for (i = 0; i < tam-1; i++) {
			if (menor_r(*(ord + i+1), *(ord + i))) {
				inverte_ponteiros((ord + i+1), (ord + i));
				swaps++;
			}
		}
	}
	return ord;
}

/* libera a memoria alocada em um vetor vet de tam ponteiros para numeros racionais,
 * e libera todos os ponteiros dentro dele */
racional **liberar_vetor_racional(racional **vet, int tam) {
	int i;
	for (i = 0; i < tam; i++) {
		free(*(vet + i));
		*(vet + i) = NULL;
	}
	free(vet);
	vet = NULL;
	return vet;
}

int main() {
	/* inicializa semente randomica */
	srand(time(0));

    racional **v, **w;
    /* v e w são vetores de ponteiros para racionais (racional *) */
    
    int tam;
    /* ler o tamanho do vetor de racionais */
    if (!(tam = ler_tamanho()))
		return 0;  /* impede de passar 0 como um parametro para malloc */
	
    /* aloca v com tam ponteiros para racional */
    v = aleatorio_vetor_racional(tam);
    /* a funcao acima retorna NULL em caso de falha */

    /* chama a funcao para ordenar o vetor */
    w = ordenar_vetor_racional(v, tam); 
    /* a funcao acima retorna NULL em caso de falha */

    /* imprime o vetor ordenado */
    imprimir_vetor_racional(w, tam);

    /* libera toda memoria alocada dinamicamente */
	v = liberar_vetor_racional(v, tam);
	free(w);   /* o vetor w pode ser liberado normalmente, porque os ponteiros que apontam para numeros */
	w = NULL;  /* racionais ja foram liberados na chamada de liberar_vetor_racional para v. */

    return 0;
}
