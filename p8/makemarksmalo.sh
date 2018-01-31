#!/bin/bash


if test $# -ne 4 #Comprobamos numero de argumentos
then
    echo "Uso: $0 Notas1 Notas2 Notas3 Notas4" 1>&2
    exit 1
fi
 
 
nombres=`cat $* | awk '{print $1}' | egrep '^([A-Z]|[a-z])+' | sort -u` #Nos quedamos con la columna de los nombres

echo "#Nombres 	Ejer1	Ejer2	Ejer3	Ejer4	Final" >> notasfinales.txt

for i in $nombres
do
	#for i $* hacer un bucle para todos los ifs
	nota1=`cat $1 | awk '$1 ~ /'$i'/ {print $2}'`
	if ! test $nota1
	then
		echo -n $i "		-" >> notasfinales.txt
	else	
    	echo -n "$i 		$nota1" >>notasfinales.txt
    fi

    nota2=`cat $2 | awk '$1 ~ /'$i'/ {print $2}'`
    if ! test $nota2
    then
		echo -n "	-" >> notasfinales.txt
	else	
    	echo -n "	$nota2" >> notasfinales.txt
    fi

    nota3=`cat $3 | awk '$1 ~ /'$i'/ {print $2}'`
    if ! test $nota3
    then
		echo -n "	-" >> notasfinales.txt
	else	
    	echo -n "	$nota3" >> notasfinales.txt
    fi

    nota4=`cat $4 | awk '$1 ~ /'$i'/ {print $2}'`	
    if ! test $nota4
    then
		echo -n "	- 	" >> notasfinales.txt
	else	
    	echo -n "	$nota4 	" >> notasfinales.txt
    fi

    cat $* |  awk '$1 ~ /'$i'/{total=total+$2;n=n+1} END {if (n==4) {print total/4}else {print "NP"}}' >> notasfinales.txt

done

