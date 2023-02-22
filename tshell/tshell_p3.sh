#!/bin/bash

if [ $# -ne 2 ]; then
	>&2 echo "Uso: ./tshell_p3.sh <axml> <acsv>"
	exit 1
fi

confere_arq_entrada() {
	IFS=. read -r NOME EXT <<< $1
	if [ $EXT != "xml.gz" ]; then
		>&2 echo "Erro: formato incorreto para $1 (deve ser .xml.gz)"
		exit 2
	fi
	if [ ! -f $1 ]; then
		>&2 echo "Erro: $1 não existe"
		exit 3
	fi
}

confere_arq_saida() {
	IFS=. read -r NOME EXT <<< $1
	if [ $EXT != "csv" ]; then
		>&2 echo "Erro: formato incorreto para $1 (deve ser .csv)"
		exit 2
	fi

	echo "version+pmid<title<abstract<keywords" > $1
}

XML=$1
confere_arq_entrada $XML
XML=$(realpath $XML)

CSV=$2
confere_arq_saida $CSV
CSV=$(realpath $CSV)


