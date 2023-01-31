#include <stdio.h>
#include <stdlib.h>

int cria_inteiro(int valor) {
	int i;
	
	i = valor;

	return i;
}

int main() {
	int tam;
	printf("Tamanho: ");
	scanf("%d", &tam);

	int *vetor;
	if ( !(vetor = malloc(tam * sizeof(int))) ) {
		printf("ERRO COMPRA MAIS RAM");
		return 1;
	}

	int i;
	for (i = 0; i < tam; i++) {
		vetor[i] = cria_inteiro(i);
	}

	for (i = 0; i < tam; i++)
		printf("%d ", vetor[i]);
	printf("\n");

	free(vetor);
	return 0;
}
