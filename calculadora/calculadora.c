#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <math.h>
#include "libpilha.h"

/* Definindo o valor do operador como um inteiro, prevenimos
   erros de aritmética com ponto flutuante. */
typedef double t_operador;

#define TAM_ENTRADA 256
#define TAM_PILHA 1024


/* Constantes com valores para identificar os operadores. O valor 
   antes do ponto flutuante define a precedência entre os operadores, 
   valores maiores tem maior precedência. */
#define SOM 10.1
#define SUB 10.2
#define MUL 20.1
#define DIV 20.2
#define EXP 30.0


/* Identificador de '(' para ser empilhado na pilha de operadores */
#define PAR 99.0


/* Converte os caracteres que representam os operadores na entrada
   para valores constantes que identificam os operadores. 
   Retorna 1 se o caracter c representa um operador valido e 0 caso
   contrario. */
int converte_operador(t_operador *op, char c) {
    switch(c) {
        case '+': *op = SOM; break;
        case '-': *op = SUB; break;
        case '*': *op = MUL; break;
        case '/': *op = DIV; break;
        case '^': *op = EXP; break;
        default : return 0;
    }
    return 1;
}


/* Retorna 1 se o operador op1 tem precedencia sobre o operador op2.
   Retorna 0 caso contrario. */
int precedencia_maior_ou_igual(t_operador op1, t_operador op2) {
    return floor(op1) >= floor(op2);
}


/* Desempilha os dois valores no topo da pilha de valores, aplica o
   operador sobre esses valores e empilha o resultado na pilha de 
   valores. */
int opera(t_operador op, t_pilha *valores) {
    double val_esq, val_dir;

    if(!desempilha(&val_dir, valores))
        return 0;
    if(!desempilha(&val_esq, valores))
        return 0;
    if(op == SOM)
        return empilha(val_esq + val_dir, valores);
    if(op == SUB)
        return empilha(val_esq - val_dir, valores);
    if(op == MUL)
        return empilha(val_esq * val_dir, valores);
    if(op == DIV && val_dir != 0.0)
        return empilha(val_esq / val_dir, valores);
    if(op == EXP)
        return empilha(pow(val_esq, val_dir), valores);

    return 0;
}


/* Imprime na saida de erro (stderr) a mensagem de erro e a linha e 
   a coluna da entrada onde o erro foi detectado. */
void erro(char *msg, int col, int *flag_erro) {
    fprintf(stderr, "ERRO: %s (coluna %d)\n", msg, col);
    *flag_erro = 1;
}


/* Le da entrada padrao, usando fgets, um vetor de caracteres ate o \n.
   Se o tamanho da entrada for maior que o vetor de leitura, aumenta
   o tamanho do vetor e continua a leitura ate encontrar o \n.
   Retorna um ponteiro para o vetor lido ou NULL caso ocorra algum erro
   ne leitura ou ne alocação de memoria. */
/*
char* le_entrada() {
    char *ent, *ret_realloc, *ret_fgets;
    int tam, pos;

    ent = NULL;
    tam = 0;
    pos = 0;
    
    do {
        tam += TAM_ENTRADA;
        ret_realloc = (char*) realloc(ent, sizeof(char) * tam);
        if(!ret_realloc) {
            free(ent);
            return NULL;
        }
        ent = ret_realloc;
        ent[tam - 1] = ' ';
        ret_fgets = fgets(ent + pos, tam - pos, stdin);
        pos = tam - 1;    
    } while(ret_fgets && ent[tam - 1] == '\0' && ent[tam - 2] != '\n');
    
    if(!ret_fgets && tam == TAM_ENTRADA) {
        free(ent);
        return NULL;
    }
    
    return ent;
}
*/


/* Le da entrada padrao, usando fgets, um vetor de caracteres ate o \n.
   Retorna um ponteiro para o vetor lido ou NULL caso ocorra algum erro
   ne leitura ou na alocação de memoria. Se o tamanho da entrada for 
   maior que o vetor de leitura, retorna NULL. */
char* le_entrada() {
    char *ent, *ret_fgets;
    int tam;
    
    tam = TAM_ENTRADA;
    ent = (char*) malloc(sizeof(char) * tam);
    if(!ent)
        return NULL;
    ent[tam - 1] = ' ';
    ret_fgets = fgets(ent, tam, stdin);
    if(!ret_fgets || (ent[tam - 1] == '\0' && ent[tam - 2] != '\n')) {
        free(ent);
        return NULL;
    }
    return ent;
}

int main() {
    t_pilha *pilha_valores, *pilha_operadores;
    t_operador operador, op_topo;
    double operando, resultado, memoria = 0.0;
    char *entrada, *c, *prox;
    int flag_erro = 0;

    entrada = le_entrada();
    if(!entrada) {
        erro("erro de leitura", 0, &flag_erro);
        return 1;
    }

    c = entrada;
    /* Loop principal, le valores e realiza operacoes ate ler uma linha
       que contém "q" na primeira posição */
    while(*c != 'q') {

        pilha_valores = cria_pilha(TAM_PILHA);
        if(!pilha_valores) {
            erro("erro ao criar pilha de valores", 0, &flag_erro);
            return 1;
        }

        pilha_operadores = cria_pilha(TAM_PILHA);
        if(!pilha_operadores) {
            erro("erro ao criar pilha de operadores", 0, &flag_erro);
            return 1;
        }

        if( !(isdigit(*c) || *c == '('))
            /* Se o primeiro caracter não for nem um dígito nem um abre parênteses,
               o formato está errado. */
            erro("formato incorreto", c - entrada + 1, &flag_erro);

        while(*c != 'q' && *c != '\n' && !flag_erro) {
            /* Percorre o ponteiro c pela entrada até achar um q, o final da linha ou um erro. */

            /* Caso 1: separadores */
            if(*c == ' ' || *c == '\t')
                /* Se for separador, passa para o proximo caracter. */
                c++;

            /* Caso 2: operando */
            else if(isdigit(*c)) {
                /* Se for [1..9] le um valor e empilha na pilha de valores. */
                operando = strtod(c, &prox);
                if(c == prox)
                    erro("operando incorreto", c - entrada + 1, &flag_erro);
                else if(!empilha(operando, pilha_valores))
                    erro("pilha de valores cheia", c - entrada + 1, &flag_erro);
                else 
                    c = prox;
            }

            /* Caso 3: abre parenteses */
            else if(*c == '(') {
                /* Se for abre parenteses, empilha PAR na pilha de operadores. */
                if(!empilha(PAR, pilha_operadores))
                    erro("pilha de operadores cheia", c - entrada + 1, &flag_erro);
                else
                    c++;
            }

            /* Caso 4: fecha parenteses */
            else if(*c == ')') {
                /* Se for fecha parenteses, processa a pilha de operadores até 
                   encontar um PAR. */ 
                while(topo(&op_topo, pilha_operadores) && op_topo != PAR && !flag_erro) {
                    desempilha(&op_topo, pilha_operadores);
                    if(!opera(op_topo, pilha_valores))
                        erro("formato incorreto", c - entrada + 1, &flag_erro);
                }
                if(pilha_vazia(pilha_operadores) ||
                   (desempilha(&op_topo, pilha_operadores) && op_topo != PAR))
                    erro("formato incorreto", c - entrada + 1, &flag_erro);
                else
                    c++;
            }

            /* Caso 5: operador */
            else if(converte_operador(&operador, *c)) {
                /* Se for um operador valido, verifica a precedencia em relacao
                   ao topo da pilha de operadores. */
                while(topo(&op_topo, pilha_operadores) &&
                      op_topo != PAR &&
                      precedencia_maior_ou_igual(op_topo, operador) &&
                      !flag_erro) {
                    /* Enquando o topo da pilha tiver precedencia, desempilha e
                       processa o operador do topo da pilha. */
                    desempilha(&op_topo, pilha_operadores);
                    if(!opera(op_topo, pilha_valores))
                        erro("formato incorreto", c - entrada + 1, &flag_erro);
                }
                if(!empilha(operador, pilha_operadores))
                    /* Empilha o novo operador na pilha de operadores. */
                    erro("pilha de operadores cheia", c - entrada + 1, &flag_erro);
                else
                    c++;
            }

            /* Caso 6: memória */
            else if(*c == 'm') {
                /* Se for m, empilha o valor guardado na memória na pilha de valores */
                if(!empilha(memoria, pilha_valores))
                    erro("pilha de valores cheia", c - entrada + 1, &flag_erro);
                else
                    c++;
            }

            /* Caso 7: caracter invalido na entrada */
            else
                erro("caracter desconhecido", c - entrada + 1, &flag_erro);
        }

        /* Sai da leitura se encontrar um q em qualquer posição da entrada */
        if(*c == 'q') break;

        if(!flag_erro) {
            /* Nesse ponto o processamento da entrada terminou (por enquanto sem erros)
               e pode ser o caso da pilha de operadores nao estar vazia. */
            while(desempilha(&op_topo, pilha_operadores) && !flag_erro) {
                /* Processa os operadores que restaram na pilha. */
                if(!opera(op_topo, pilha_valores))
                    erro("formato incorreto", c - entrada + 1, &flag_erro);
            }

            /* Após o processamento, o resultado final da expressao esta no topo da 
               pilha de valores. A pilha deve conter apenas esse valor. */
            if(!flag_erro) {
                if(!desempilha(&resultado, pilha_valores) || !pilha_vazia(pilha_valores))
                    erro("formato incorreto", c - entrada + 1, &flag_erro);
                else {
                    memoria = resultado;
                    printf("%.16g\n", resultado);
                }
            }
        }
        /* Libera a entrada e lê uma nova entrada. */

        destroi_pilha(pilha_valores);
        destroi_pilha(pilha_operadores);
        free(entrada);

        entrada = le_entrada();
        if(!entrada)
            erro("erro de leitura", 0, &flag_erro);

        c = entrada;

        flag_erro = 0;
    }

    free(entrada);

    return 0;
}
