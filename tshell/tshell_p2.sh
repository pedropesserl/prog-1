#!/bin/bash

if [ $# -ne 2 ]; then
	>&2 echo "Uso: <dcsv> <dsin>"
	exit 1
fi

testa_diretorio() {
	if [ ! -d $1 ]; then
		>&2 echo "Erro: $1 não é um diretório."
		exit 1
	fi
}

DCSV=$1
DSIN=$2

testa_diretorio $DCSV
DCSV=$(realpath $DCSV)

testa_diretorio $DSIN
DSIN=$(realpath $DSIN)

cd $DSIN
for FATOR in *; do

	CONT=$(grep -iwF -f $FATOR $DCSV/* | wc -l)

	echo "${FATOR%.*}:$CONT"
done
cd - > /dev/null
