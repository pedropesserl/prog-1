#include <stdio.h>
#include "lib_racionais.h"

int main() {
    racional r, s;
    r = le_racional();
    if (r.den) { s = le_racional(); }
    while(valida_racional(s) && valida_racional(r)) {
        racional soma = soma_rs(r, s);
        formata_racional(soma);

        racional subtracao = subtrai_rs(r, s);
        formata_racional(subtracao);

        racional multiplicacao = multiplica_rs(r, s);
        formata_racional(multiplicacao);

        racional divisao = divide_rs(r, s);
        formata_racional(divisao);
        
        printf("\n");

        r = le_racional();
        if (r.den) { s = le_racional(); }
    }
    return 0;
}
