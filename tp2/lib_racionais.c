#include <stdio.h>
#include "lib_racionais.h"

int mdc(int a, int b) {
    int resto;
    if (a != 0 && b != 0) { resto = a % b;
        while (resto) {
            a = b;
            b = resto;
            resto = a % b;
        }
        return b;
    }
    return 0;
}

racional le_racional() {
    racional r;
    scanf("%d %d", &r.num, &r.den);
    
    return r;
}

racional multiplica_rs(racional r, racional s) {
    racional p;
    p.num = r.num * s.num;
    p.den = r.den * s.den;

    return p;
}

racional divide_rs(racional r, racional s) {
    racional p;
    p.num = r.num * s.den;
    p.den = r.den * s.num;

    return p;
}

racional soma_rs(racional r, racional s) {
    racional p;
    p.den = r.den * s.den;
    p.num = r.num * s.den + s.num * r.den;

    return p;
}

racional subtrai_rs(racional r, racional s) {
    racional p;
    p.den = r.den * s.den;
    p.num = r.num * s.den - s.num * r.den;
    
    return p;
}

void formata_racional(racional r) {
    int fator = mdc(r.num, r.den);
    
    if (fator) {
        r.num /= fator;
        r.den /= fator;
    }
    
    if (!r.den) {
        printf("Erro: divisao por 0\n");
    } else {
        if (!r.num) {
            printf("0\n");
        } else {
            if (r.den == 1) {
                printf("%d\n", r.num);
            } else {
                printf("%d/%d\n", r.num, r.den);
            }
        }
    }
    return;
}

int valida_racional(racional r) {
    if (!r.den) return 0;
    return 1;
}
