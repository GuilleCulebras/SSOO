#!/bin/bash



cd /tmp

for i in * #GLOBING , se utiliza para listar todos los ficheros de un determinado directorio
#for i in b* todos los que contengan una b y cualquier cosa
#for i in *.c todo aquello que empiece por cualquier cosa y termine en .c
do
	echo $i

done

echo "======================="

for i in `ls /tmp/ | egrep '.+\.c$'`
do
	echo $i
done