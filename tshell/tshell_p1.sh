#!/bin/bash

if [ $# -lt 3 ] || [ $# -gt 5 ]; then
    >&2 echo "Uso: ./tshell_p1.sh <dpm> <ds> <t1> [<t2> [<t3>]]"
    exit 1
fi

FONTE=$1
DESTINO=$2
T1=$3
T2=$4
T3=$5

if [ ! -d $FONTE ]; then
    >&2 echo "Erro: $FONTE não é um diretório."
    exit 2
fi

FONTE=$(realpath $FONTE)

if [ ! -d $DESTINO ]; then
    mkdir $DESTINO
fi

DESTINO=$(realpath $DESTINO)

cd $FONTE
for ARQ in *.csv; do
    awk '(NR>1)' $ARQ | grep -iwF "$T1" | grep -iwF "$T2" | grep -iwF "$T3" > $DESTINO/$ARQ
done
cd - > /dev/null
