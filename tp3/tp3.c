#include <stdio.h>
#include <stdlib.h>
#include "lib_racionais.h"
#define MAX 100

/* le um inteiro na faixa [0..MAX-1] */
int ler_tamanho() {
	printf("Insira o tamanho do vetor (entre 1 e %d):\n", MAX);
	int n;
	scanf("%d", &n);
	return n;
}

/* imprime os racionais apontados pelo vetor de ponteiros para racionais */
void imprimir_vetor_racional(racional **vet, int tam) {
	int i;
	for (i = 0; i < tam-1; i++) {
		imprimir_r(*(vet + i));
		printf(" ");
	}
	imprimir_r(*(vet + tam-1));
	printf("\n");
}

/* retorna um vetor de tam ponteiros para numeros racionais validos gerados aleatoriamente */
/* retorna NULL em caso de falha                                                           */
racional **aleatorio_vetor_racional(int tam) {
	racional **vet;
	if ( !(vet = malloc(tam * sizeof(racional*))) )
		return NULL;
	int i;
	for (i = 0; i < tam; i++) {
		*(vet + i) = sortear_r();
	}
	return vet;
}

/* retorna um vetor de tam ponteiros para numeros racionais que apontam em ordem crescente para os 
 * racionais apontados pelo vetor recebido no parametro. Defina outras funcoes para te ajudar caso
 * ache necessario */
racional **ordenar_vetor_racional(racional **vet, int tam) {
	
}

/* libera a memoria alocada em um vetor vet de tam ponteiros para numeros racionais */
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
    racional **v, **w;
    /* v e w são vetores de ponteiros para racionais (racional *)
       alternativamente poderia ser declarado como racional *v[] */
    
    int tam;

    /* inicializa semente randomica                            */
    /* depois pode trocar para, por exemplo, srand (time (0)); */
    srand(0);

    /* ler o tamanho do vetor de racionais */
    tam = ler_tamanho();

    /* aloca v com tam ponteiros para racional */
    v = aleatorio_vetor_racional(tam);
    /*  lembre-se que a funcao acima retorna NULL em caso de falha */

    /* chama a funcao para ordenar o vetor */
    w = ordenar_vetor_racional(v, tam); 
    /*  lembre-se que a funcao acima retorna 0 em caso de falha */

    /* imprime o vetor ordenado */
    imprimir_vetor_racional(v, tam);

    /* libera toda memoria alocada dinamicamente        */
    /* nao esqueca de testar com valgrind para conferir */
	v = liberar_vetor_racional(v, tam);

    return 0;
}
