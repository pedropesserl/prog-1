#include <stdio.h>
#include <stdlib.h>
#include "lib_racionais.h"
#define MAX 100

/* le um inteiro na faixa [0..MAX-1] */
int ler_tamanho() {
	printf("Insira o tamanho do vetor (entre 1 e MAX):\n");
	int n;
	scanf("%d", &n);
	return n;
}

/* imprime os racionais apontados pelo vetor de ponteiros para racionais */
void imprimir_vetor_racional(defina os parametros) {
    /* coloque aqui seu codigo */
}

/* retorna um vetor de tam ponteiros para numeros racionais validos gerados aleatoriamente */
/* retorna NULL em caso de falha                                                           */
racional** aleatorio_vetor_racional(defina os parametros) {
    /* coloque aqui seu codigo */
}

/* retorna um vetor de tam ponteiros para numeros racionais que apontam em ordem crescente para os 
 * racionais apontados pelo vetor recebido no parametro. Defina outras funcoes para te ajudar caso
 * ache necessario */
racional** ordenar_vetor_racional(defina os parametros) {
    /* coloque aqui seu codigo */
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
    v = aletorio_vetor_racional(tam);
    /*  lembre-se que a funcao acima retorna NULL em caso de falha */

    /* chama a funcao para ordenar o vetor */
    w = ordenar_vetor_racional(v, tam); 
    /*  lembre-se que a funcao acima retorna 0 em caso de falha */

    /* imprime o vetor ordenado */
    imprimir_vetor_racional(w, tam);

    /* libera toda memoria alocada dinamicamente        */
    /* nao esqueca de testar com valgrind para conferir */
    /* coloque aqui seu codigo */

    return 0;
}
