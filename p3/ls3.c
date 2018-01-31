#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <err.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>


void
ls( char *path){
	DIR *d;
	struct dirent *entry;
	struct stat s;
	char fullpath[1024];
	
	d=opendir(path);
	if (d==NULL)
		err(1,"error: opendir");
	
	for(;;){
		entry=readdir(d);
				
		if (entry==NULL)
			break;
					
		sprintf(fullpath,"%s/%s",path,entry->d_name);
		if (entry->d_name[0]!='.'){
			printf("\n%s\n",fullpath);

			if (stat(fullpath,&s)<0)
				err(1,"error: stat");

			if ((s.st_mode & S_IFMT)==S_IFDIR)
				ls(fullpath);
		}
	}
	closedir(d);	
}


int 
main (int argc, char *argv[]){
	
	
	char path[1024];

	
	if (argc>2){
		fprintf(stderr,"Uso: %s [directorio]\n",argv[0]);
		exit(1);
	}
	
	if (argc==1)
		strcpy(path,".");
	else
		strcpy(path,argv[1]);
	
	ls(path);
	exit(0);
}
