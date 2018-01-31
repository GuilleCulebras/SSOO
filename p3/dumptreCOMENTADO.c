#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <err.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>

void
leerfichero(char *rutafichero){
	int fd;
	int nr;
	char buffer[1024];

	fd = open(rutafichero, O_RDONLY);
	if (fd<0)
		err(1,"error: open");

	printf("\n");

	for(;;){
		nr = read(fd, buffer, sizeof buffer);
		if(nr<0)
			err(1,"error: read");
		if(nr==0)
			break;

		if(write(1,buffer,nr) != nr)
			err(1,"error: write");
	}
	close(fd);
}

void
leerdirectorio(char *path){
	DIR *dir;
	struct dirent *datos_dir;
	struct stat s;
	char statpath[1024];

	dir = opendir(path);// devuelve un puntero a un tipo DIR

	if (dir == NULL)
		err(1,"error: opendir");

	for(;;){
		
		datos_dir = readdir(dir); //le pasamos el directorio y nos devuelve estructura de datos con informacion del directorio, si devuelve NULL ha terminado de leer
		
		if(datos_dir == NULL) //si ha terminado de leer los directorios, terminamos de leer
			break;

		sprintf(statpath,"%s/%s",path,datos_dir->d_name);

		if (datos_dir->d_name[0] != '.'){ //omitimos el fichero de arriba y el actual

			printf("\n%s\n", statpath);

			if (stat(statpath, &s)<0)
				err(1,"error: stat");
			
        	if ((s.st_mode & S_IFMT)==S_IFDIR)// si es directorio, hacemos recursividad
        		leerdirectorio(statpath);

        	if ((s.st_mode & S_IFMT)==S_IFREG) //si es fichero, leemos el contenido                   
        		leerfichero(statpath);        	
		}		
	}
	closedir(dir);
}


int 
main(int argc, char *argv[]){	
	char path[1024]; 
	
	if(argc > 2){
		fprintf(stderr, "Usage: %s [directorio]\n",argv[0]);
		exit(1);
	}

	if(argc == 1){
		strcpy(path, ".");///si no pasamos argumento guardamos en path . directorio actual
	}else{
		strcpy(path,argv[1]);/// si hay dos argumentos guardamos en el path el argumento que pasemos
		printf("%s\n",path);
	}
	leerdirectorio(path);
	exit(0);
}











