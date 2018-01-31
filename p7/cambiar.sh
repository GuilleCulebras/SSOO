#!/bin/bash

cd /tmp

for i in * #GLOBING , se utiliza para listar todos los ficheros de un determinado directorio
do
	nuevo = `echo $i | sed 's/\.c$//g'` #sustituir todo lo que acabe por .c, quitamos el .c y ponemos nada
	echo $i $nuevo
done 
