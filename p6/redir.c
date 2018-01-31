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
int p[2]; //array con dos posociones para el pipe


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
cerrarpipes(){

	close(p[0]);
	close(p[1]);
}

int
makegcc(char *ejecutable, char *fichero){
	int pid;
	char *cflags;

	pid = fork();					

	switch(pid){
		case -1:
			err(1,"error: fork");
			break;
		case 0:
			dup2(p[1],2);// redireccionamos la salida de error al parte de escritura del pipe
			cerrarpipes();


			cflags = getenv("CFLAGS");
			if (cflags)
				execl("/usr/bin/gcc","gcc",cflags,"-o",ejecutable,fichero,NULL);
			else
				execl("/usr/bin/gcc","gcc","-o",ejecutable,fichero,NULL);
			err(1, "error: execl");
			break;
	}
	return pid;
}

int
makegrep(char *palabra){
	int pid;

	pid = fork();

	switch(pid){
		case -1:
			err(1,"error: fork");
			break;
		case 0:
			dup2(p[0],0);//redireccionamos la entrada estandar a la
			cerrarpipes();

			execl("/bin/grep","grep",palabra,NULL);
			err(1, "error: execl");
			break;
	}
	return pid;

}

void
waitmakes(int pid1, int pid2, char *nomfich){
	int sts;

	if(waitpid(pid1,&sts,0)<0)
		err(1,"error: wait gcc");

	if(sts == 0)
		printf("%s: compila\n",nomfich);
	else
		printf("%s: no compila\n",nomfich);

	if(waitpid(pid2,NULL,0)<0)
		err(1,"error: wait grep");
}

void
compilar(char *fichero, char *ejecutable, char *nomfich, char *palabra){

	pid_t pidgcc;
	pid_t pidgrep;	
	
	if(pipe(p)<0)
		err(1,"error: pipe");
	
	pidgcc = makegcc(ejecutable,fichero);
	pidgrep = makegrep(palabra);	
	cerrarpipes();
	waitmakes(pidgcc,pidgrep,nomfich);
}

void
leerdirectorio(char *path, char *palabra){
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
				strcpy(aux,datos_dir->d_name);
				ntokens = trocea(aux,".",trozos); 

				if((ntokens > 1) && strcmp(trozos[ntokens-1],"c") == 0){
					nprocs ++;

					switch(fork()){
						case -1:
							err(1,"error: fork");
							break;
						case 0:
							sprintf(ejecutable,"%s/%s",path,trozos[0]);
							compilar(fullpath,ejecutable,datos_dir->d_name,palabra);
							exit(0);
					}	
				} 
			}
		}
	}
	closedir(dir);
}

void
waitprocs(){
	int i;
	for (i = 0; i < nprocs; i++){
		if(wait(NULL)<0)
			err(1,"error: wait");
	}
}


int
main(int argc, char *argv[]){
	char *path;
	char *palabra;


	if (argc > 3 || argc == 1){ //si tenemos mas de tres argumentos error
		fprintf(stderr, "Usage: %s [directorio] palabra\n",argv[0]);
		exit(1);
	}

	if (argc == 2){
		path = malloc(sizeof(char));
		strcpy(path,".");
	}
	if (argc == 3){
		path = malloc(sizeof(char)*strlen(argv[1]));
		strcpy(path,argv[1]);
	}

	palabra = malloc(sizeof(char)*strlen(argv[argc-1]));
	strcpy(palabra,argv[argc-1]);

	leerdirectorio(path,palabra);

	waitprocs();

	free(path);
	free(palabra);

	exit(0);
}