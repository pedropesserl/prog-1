#include <stdio.h>
#include <stdlib.h>
#include "lib_racionais.h"

int aleat(int min, int max) {
    return (rand() % (max - min + 1) + min);
}

racional *criar_r() {
    racional *r;
    if ( !(r = malloc(sizeof(racional))) )
        return NULL;
    return r;
}

racional *liberar_r(racional *r) {
    free(r);
    r = NULL;
    return r;
}

racional *sortear_r() {
    racional *r;
    r = criar_r();
    r->num = aleat(0, 100);
    r->den = aleat(1, 100);
    simplifica_r(r);
    return r;
}

int ler_r(racional *r) {
    if (!scanf("%d %d", &r->num, &r->den))
        return 0;
    if (!r->den)
        return 0;
    return 1;    
}

void imprimir_r(racional *r) {
    if (!r->num) {
        printf("0");
        return;
    }
    if (r->den == 1) {
        printf("%d", r->num);
        return;
    }
    printf("%d/%d", r->num, r->den);
    return;
}
 
int mdc(int a, int b) {
    int r;
    while (b) {
        r = a % b;
        a = b;
        b = r;
    }
    return a;
}

int mmc(int a, int b) {
    if (a && b)
        return ((a * b) / mdc(a, b));
    return 0;
}

int valido_r(racional *r) {
    if (r->den)
        return 1;
    return 0;
}

void simplifica_r(racional *r) {
    int fator = mdc(r->num, r->den);
    if (fator) {
        r->num /= fator;
        r->den /= fator;
    }
}

int menor_r(racional *r1, racional *r2) {
    int a = r1->num * r2->den;
    int b = r1->den * r2->num;
    if (a < b)
        return 1;
    return 0;
}

int iguais_r(racional *r1, racional *r2) {
    int a = r1->num * r2->den;
    int b = r1->den * r2->num;
    if (a == b)
        return 1;
    return 0;
}

racional *somar_r(racional *r1, racional *r2) {
    racional *soma;
    soma = criar_r();

    soma->den = r1->den * r2->den;
    soma->num = r1->num * r2->den + r2->num * r1->den;
    simplifica_r(soma);
    return soma;
}

racional *subtrair_r(racional *r1, racional *r2) {
    racional *dif;
    dif = criar_r();

    dif->den = r1->den * r2->den;
    dif->num = r1->num * r2->den - r2->num * r1->den;
    simplifica_r(dif);
    return dif;
}

racional *multiplicar_r(racional *r1, racional *r2) {
    racional *mult;
    mult = criar_r();

    mult->den = r1->den * r2->den;
    mult->num = r1->num * r2->num;
    simplifica_r(mult);
    return mult;
}

racional *dividir_r(racional *r1, racional *r2) {
    racional *div;
    div = criar_r();

    div->den = r1->den * r2->num;
    div->num = r1->num * r2->den;
    simplifica_r(div);
    return div;
}
