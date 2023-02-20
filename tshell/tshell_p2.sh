#!/bin/bash

if [ $# -ne 2 ]; then
	>&2 echo "Uso: <dcsv> <dsin>"
	exit 1
fi

testa_se_diretorio() {
	if [ ! -d $1 ]; then
		>&2 echo "Erro: $1 não é um diretório."
		exit 1
	fi
}

DCSV=$1
DSIN=$2

testa_se_diretorio $DCSV
testa_se_diretorio $DSIN
