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
	struct stat st;
	char statpath[1024];

	dir = opendir(path);

	if (dir == NULL)
		err(1,"error: opendir");

	for(;;){
		
		datos_dir = readdir(dir); 
		
		if(datos_dir == NULL) 
			break;

		sprintf(statpath,"%s/%s",path,datos_dir->d_name);

		if (datos_dir->d_name[0] != '.'){

			printf("\n%s\n", statpath);

			if (stat(statpath, &st)<0)
				err(1,"error: stat");
			
        	if ((st.st_mode & S_IFMT)==S_IFDIR)
        		leerdirectorio(statpath);

        	if ((st.st_mode & S_IFMT)==S_IFREG)                   
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
		strcpy(path, ".");
		printf("%s\n",path);
	}else{
		strcpy(path,argv[1]);
		printf("%s\n",path);
	}
	leerdirectorio(path);
	exit(0);
}











