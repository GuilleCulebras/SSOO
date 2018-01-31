#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <err.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/file.h>
 
char palabra[128];

void
createfile(char *path){
    int fd;
    
    fd = creat(path, 0777);
    if(fd < 0)
        err(1,"creat");
    write(fd,"0",1);
    close(fd);
}

void
writefile(void){
    int fd;
    char path[1024];
    char buf[128];
    int nr;
    int nb;
    char *home;

    home = getenv("HOME");
    if(home == NULL)
        err(1,"getenv");
     
    sprintf(path,"%s/cfiles.cnt",home); 
   
    fd = open(path, O_RDWR);
        if(fd < 0) 
            err(1, "open");

    if(flock(fd, LOCK_EX) != 0){
        err(1, "lock");
    }

    nr = read(fd, buf, sizeof(buf)-1);
    if(nr < 0){
        err(1, "read1");
    }

    buf[nr] = 0;
    nb = atoi(buf);

    snprintf(buf, sizeof buf, "%d", nb+1);

    lseek(fd, 0, 0);

    if(write(fd, buf, strlen(buf)) != strlen(buf)) {
        err(1, "write");
    }

    if(flock(fd, LOCK_UN) != 0){
        err(1, "lock");
    }

    close(fd);
}
 
 
void *
readfile (void *argumento){

    char *fichero=(char*) argumento;
    FILE *f;
    char buf[1024];
    int nr;

    f=fopen(fichero,"r");

    if (f==NULL){
        pthread_exit(NULL);
    }
    for(;;){
        nr=fread(buf,sizeof(char),sizeof buf,f);

        if(nr == 0){
            if(ferror(f)) {
                pthread_exit(NULL);
            }
            break; 
        } 
        if(strstr(buf,palabra) != NULL){
            writefile();
            break;
        }
    }   
    fclose(f);
    
    pthread_exit(NULL);
}
 
int
main(int argc, char *argv[]){
    pthread_t *t;
    int nthreads;
    int i;
    char path[1024];
    char *home;

    if(argc < 3){
        fprintf(stderr, "Usage: %s palabra rutafichero1 [rutafichero2] [rutafichero3] ...\n",argv[0]);
        exit(1);
    }
    strcpy(palabra,argv[1]);   

    home = getenv("HOME");
    if(home == NULL)
        err(1,"getenv");  

    sprintf(path,"%s/cfiles.cnt",home);

    if(access(path,F_OK)!=0){
        createfile(path);
    }

    nthreads = argc-2;
    t = malloc(sizeof(pthread_t)*nthreads); 
    for(i=0; i<nthreads; i++)
        pthread_create(&t[i],NULL,readfile,argv[i+2]);
     
    for(i=0; i<nthreads; i++) 
        pthread_join(t[i],NULL);
     
    free(t);
     
    exit(0);
}
