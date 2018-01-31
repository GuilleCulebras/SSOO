/*
	Guillermo Culebras Talaya
	Tecnologías
*/

#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <string.h>
#include <unistd.h>	
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>


int nprocs;

int
trocea(char *str, char *delm, char *trozos[]){
    char *aux;
    char *token;
    int n=0;

    token=strtok_r(str,delm,&aux);

    for(;;){

        trozos[n]=token;
        n++;
        str = aux;
        token=strtok_r(str,delm,&aux);
        if(token == NULL)
        	break;
    }
    return n;
}

void
compilar(char *fichero, char *ejecutable, char *nomfich){
	int sts;
	pid_t pid;
	char *cflags;
	int fd;

	pid = fork();					

	switch(pid){
		case -1:
			err(1,"error: fork");
			break;
		case 0:
			fd = open("/dev/null", O_WRONLY);
			if(fd<0)
				err(1,"error: open");
			dup2(fd,1);
			dup2(fd,2); //desviar la salida estandar y de error al fichero /dev/null
			close(fd);

			cflags = getenv("CFLAGS");
			if (cflags)
				execl("/usr/bin/gcc","gcc",cflags,"-o",ejecutable,fichero,NULL);
			else
				execl("/usr/bin/gcc","gcc","-o",ejecutable,fichero,NULL);
			err(1, "error: execl");
			break;
		default:
			wait(&sts);//Devuelve en stat como ha terminado el hijo, 0 bien, 1 mal.,
			if(sts == 0)
				printf("%s: compila\n",nomfich);
			else
				printf("%s: no compila\n",nomfich);
	}
}

void
leerdirectorio(char *path){
	DIR *dir;
	struct dirent *datos_dir;
	struct stat st;
	char fullpath[1024];
	char *trozos[1024];
	char aux[1024];
	int ntokens;
	char ejecutable[1024];

	dir = opendir(path);
	if (dir == NULL)
		err(1,"error: opendir");

	for(;;){
		datos_dir = readdir(dir);

		if(datos_dir == NULL) 
			break;

		sprintf(fullpath,"%s/%s",path,datos_dir->d_name);

		if (datos_dir->d_name[0] != '.'){

			if (stat(fullpath, &st)<0)
				err(1,"error: stat");

			if ((st.st_mode & S_IFMT)==S_IFREG){
				strcpy(aux,datos_dir->d_name); //Guardamos el nombre del fichero original para no perderlo al trocearlo
				ntokens = trocea(aux,".",trozos); //trocea el nombre del fichero por punto y lo guarda en trozos

				if((ntokens > 1) && strcmp(trozos[ntokens-1],"c") == 0){//tiene extension c
					nprocs ++;

					switch(fork()){
						case -1:
							err(1,"error: fork");
							break;
						case 0:
							sprintf(ejecutable,"%s/%s",path,trozos[0]);
							compilar(fullpath,ejecutable,datos_dir->d_name);
							exit(0);
					}	
				} 
			}
		}
	}
	closedir(dir);
}


int
main(int argc, char *argv[]){
	char path[1024];
	int i;

	if (argc > 2){
		fprintf(stderr, "%s\n",argv[0]);
		exit(1);
	}

	if (argc == 1)//si no hay argumento ruta actual
		strcpy(path,".");

	if (argc == 2)
		strcpy(path,argv[1]);

	leerdirectorio(path);

	for (i = 0; i < nprocs; i++){//Esperamos a todos los hijos
		if(wait(NULL)<0)
			err(1,"error: wait");
	}
	exit(0);
}