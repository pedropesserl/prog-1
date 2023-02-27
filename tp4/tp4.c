#include <stdio.h>
#include <stdlib.h>
#include "lib_conjunto.h"
#define MAX 100

conjunto_t *le_conjunto_simples(int max) {
    conjunto_t *leitura;
    if ( !(leitura = cria_cjt(max)) )
        return NULL;
    int n;
    scanf("%d", &n);
    while (n) {
        insere_cjt(leitura, n);
        scanf("%d", &n);
    }
    return leitura;
}

conjunto_t **le_vetor_conjuntos(int *tam, int max) {
    conjunto_t **vetor_c;
    if ( !(vetor_c = malloc(sizeof(conjunto_t *) * max)) )
        return NULL;
    int n, m;
    scanf("%d", &n);
    while (n) {
        if ( !(vetor_c[*tam] = cria_cjt(15)) )
            return NULL;
        insere_cjt(vetor_c[*tam], n);
        scanf("%d", &m);
        while (m) {
            insere_cjt(vetor_c[*tam], m);
            scanf("%d", &m);
        }
        *tam = *tam + 1;
        scanf("%d", &n);
    }
    return vetor_c;
}

conjunto_t **libera_vetor_conjuntos(conjunto_t **vetor_c, int tam) {
    int i;
    for (i = 0; i < tam; i++)
        vetor_c[i] = destroi_cjt(vetor_c[i]);
    free(vetor_c);
    return NULL;
}

conjunto_t *acha_solucao(conjunto_t **herois, conjunto_t *missao, conjunto_t **equipes, int tam_equipes) {
    conjunto_t *menor;
    menor = cria_cjt(15);
    conjunto_t *uniao_old;
    conjunto_t *uniao;
    int i, j;
    
    for (i = 0; i < tam_equipes; i++) {
        uniao = copia_cjt(herois[equipes[i]->v[0] - 1]);
        /* copia o conjunto de habilidades de heroi    correspondente ao primeiro elemento do conjunto equipe da posicao atual do vetor de equipes. ex: se a equipe atual é [5 6 7], copia o quinto conjunto de habilidades de heroi (de indice 4). */
        for (j = 1; j < cardinalidade_cjt(equipes[i]); j++) {
            uniao_old = uniao;
            uniao = uniao_cjt(uniao, herois[equipes[i]->v[j] - 1]);
            uniao_old = destroi_cjt(uniao_old);
            /* realiza a uniao de todos os conjuntos de habilidades de heroi referenciados no conjunto equipe da posicao atual do vetor de equipes. */
        }
        if (contido_cjt(missao, uniao)) {
            if (vazio_cjt(menor) || cardinalidade_cjt(equipes[i]) < cardinalidade_cjt(menor)) {
                menor = destroi_cjt(menor);
                menor = copia_cjt(equipes[i]);
            }
        }
        uniao = destroi_cjt(uniao);
    }
    
    return menor;
}

int main() {
    /* ler os herois e suas habilidades */
    int tam_herois = 0;
    conjunto_t **vetor_herois;
    vetor_herois = le_vetor_conjuntos(&tam_herois, MAX);
    
    /* ler a missao */
    conjunto_t *missao;
    missao = le_conjunto_simples(20);
    
    /* ler as equipes de herois */
    int tam_equipes = 0;
    conjunto_t **vetor_equipes;
    vetor_equipes = le_vetor_conjuntos(&tam_equipes, MAX);

    /* a solucao eh encontrada se a missao esta contido na uniao das 
     * habilidades de uma equipe, mas tomando-se aquela de menor tamanho. */
    conjunto_t *solucao;
    solucao = acha_solucao(vetor_herois, missao, vetor_equipes, tam_equipes);
    if (vazio_cjt(solucao)) {
        printf("NENHUMA\n");
    } else {
        imprime_cjt(solucao);
    }

    /* libera toda a memoria alocada dinamicamente */
    vetor_herois = libera_vetor_conjuntos(vetor_herois, tam_herois);
    missao = destroi_cjt(missao);
    vetor_equipes = libera_vetor_conjuntos(vetor_equipes, tam_equipes);
    solucao = destroi_cjt(solucao);

    return 0;
}
