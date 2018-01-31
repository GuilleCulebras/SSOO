#!/bin/bash

cflag=n
dflag=n 

if test $# -gt 5
then
	echo "Uso: ./ccall.sh [-c OpCompilacion] [-d Directorio] [argumento]" 1>&2
	exit 1
fi

while test $# -gt 0 
do
	case $1 in
		-c)
			cflag=y
			argC=$2
			shift
			;; #igual que el break
		-d)
			dflag=y
			directorio=$2
			shift
			;;

		-*)
			echo "$1: Flag incorrecto " 1>&2
			exit 1
			;;
		*) 
			argumento=$1 #si nuestro programa recibe un argumeto que no sea un flag
	esac
	shift #avanaza al siguiente argumento y elimina el actual
done

if test $dflag = y #si hay flag de directorio nos movemos a ese directorio
then
	cd $directorio
fi


if test $argumento 
then
	for i in *.c #iteramos en todos los archivos que acaben .c
	do
		ejecutable=`echo $i | sed 's/\.c$//g'`
		gcc -o $ejecutable $argC $i 2>&1 | egrep $argumento
	done
else
	for i in *.c #iteramos en todos los archivos que acaben .c
	do
		ejecutable=`echo $i | sed 's/\.c$//g'`
		gcc -o $ejecutable $argC $i 
	done

fi


