#!/bin/bash

if test $# -ne 4
then
    echo "Uso: $0 Notas1 Notas2 Notas3 Notas4" 1>&2
    exit 1
fi
 
nombres=`cat $* | awk '{print $1}' | egrep '^([A-Z]|[a-z])+' | sort -u`

echo "#Nombres	Ejer1	Ejer2	Ejer3	Ejer4	Final" > notasfinales.txt

for i in $nombres
do
    echo -n $i "		" >> notasfinales.txt
    for j in $*
    do
    	nota=`cat $j | awk '$1 ~ /'$i'/ {print $2}'`
    	if ! test $nota
    	then
    		echo -n "-	" >> notasfinales.txt
    	else	
        	echo -n "$nota	" >> notasfinales.txt
        fi
    done
    cat $* |  awk '$1 ~ /'$i'/{tot=tot+$2;n=n+1} END {if (n==4) {print tot/4}else {print "NP"}}' >> notasfinales.txt
done