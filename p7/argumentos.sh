cflag= n

while test $0 -gt 0 #si hay mas de 0 argumentos
do
	case $1 in
		-c)
			cflag = y
			valorC = $2
			shift
			;; #igual que el break
		-c)
			dflag = y
			valorD = $2
			shift
			;;
		-s) # mirar si se puede terminar el argumento para que no coja cualquier cosa que empiece por -c
			sflag = y 
			valorS = $2
			shift
			;;
		-*)
			echo "$1: opcion incorrecta"
			exit 1
		*) #default
			argumento = $1 #si nuestro programa recibe un argumeto que no sea un flag

		

	esac
	shift #avanaza al siguiente argumento y elimina el actual
done

if test $cflag = y
	echo "cflag " $cflag $valorC
fi
if test $dflag = y
	echo "dflag " $dflag $valorD
fi










