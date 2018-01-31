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
copyfile(char *fOrigen, char *fDest, mode_t per){
	int fd1;
	int fd2;
	int nr;
	char buffer[1024];

	fd1 = open(fOrigen, O_RDONLY);
	if (fd1<0)
		err(1,"error: open");

	fd2 = creat(fDest, 0777);
	if (fd2<0)
		err(1,"error: creat");

	for(;;){
		nr = read(fd1, buffer, sizeof buffer);
		if(nr<0)
			err(1,"error: read");
		if(nr==0)
			break;

		if(write(fd2,buffer,nr) != nr)
			err(1,"error: write");
	}
	close(fd1);
	close(fd2);
	if (chmod(fDest,per)<0)
		fprintf(stderr, "%s: error chmod\n",fDest);
}

void
copydir(char *dOrigen, char *dDest, mode_t perD, mode_t perF){
	char pathOrigen[1024];
	char pathDest[1024];
	DIR *dir;
	struct dirent *datos_dir;
	struct stat st;

	dir = opendir(dOrigen);

	if (dir == NULL)
		err(1,"error: opendir");

	for(;;){
		
		datos_dir = readdir(dir); 
		
		if(datos_dir == NULL) 
			break;
 
		sprintf(pathOrigen,"%s/%s",dOrigen,datos_dir->d_name);
		sprintf(pathDest,"%s/%s",dDest,datos_dir->d_name);

		if (datos_dir->d_name[0] != '.'){

			if (stat(pathOrigen, &st)<0)
				err(1,"error: stat");
			
        	if ((st.st_mode & S_IFMT)==S_IFDIR){
				if(mkdir(pathDest, 0777) < 0)
					err(1,"error: mkdir %s",pathDest);

				copydir(pathOrigen,pathDest,perD,perF);   	
			
				if(chmod(pathDest,perD)<0)
					fprintf(stderr, "%s:err chmod\n",pathDest);
			}
        	if ((st.st_mode & S_IFMT)==S_IFREG)                   
        		copyfile(pathOrigen,pathDest,perF);
        }        			
	}
	closedir(dir);
}

int 
main(int argc, char *argv[]){	
	long perDir;
	long perFile;
	struct stat st;

	if (argc != 5){
		fprintf(stderr, "Usage: %s PermisosDirectorio PermisosFichero PathOrigen PathDestino\n",argv[0]);
		exit(1);
	}

	perDir = strtol(argv[1],0,8);
	perFile = strtol(argv[2],0,8);

	if (stat(argv[3], &st)<0)
		err(1,"error: stat");

	if ((st.st_mode & S_IFMT)==S_IFREG)
		copyfile(argv[3],argv[4],perFile);

	if ((st.st_mode & S_IFMT)==S_IFDIR){

		if(mkdir(argv[4], 0777) < 0)
			err(1,"error: mkdir %s",argv[4]);

		copydir(argv[3],argv[4],perDir,perFile);

		if (chmod(argv[4],perDir)<0)
			fprintf(stderr, "%s:error chmod\n",argv[4]);
	}
	exit(0);
}