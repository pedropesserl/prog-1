typedef struct {
	int num;
	int den;
} racional;

/* le um numero racional (dois inteiros) */
racional le_racional();

/* multiplica dois numeros racionais e retorna o resultado como um racional */
racional multiplica_rs(racional r, racional s);

/* divide dois numeros racionais e retorna o resultado como um racional */
racional divide_rs(racional r, racional s);

/* soma dois numeros racionais e retorna o resultado como um racional */
racional soma_rs(racional r, racional s);
 
/* subtrai dois numeros racionais e retorna o resultado como um racional */
racional subtrai_rs(racional r, racional s);

/* simplifica um numero racional na sua forma irredutivel e imprime o numero na tela */
void formata_racional(racional r);

/* checa se um numero racional eh valido, i.e. ele nao possui 0 como denominador */
int valida_racional(racional r);
