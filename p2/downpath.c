/*
	Guillermo Culebras Talaya
	Tecnologías
*/


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <ctype.h>

void
strtoupper(char *str){
	int i = 0;

	while(str[i]){
		str[i] = toupper(str[i]);
		i++;
	}
}

int
makepath(char *buf, int len){

	char *home;
	char *user;
	pid_t pid;

	home = getenv("HOME");
	if (home == NULL){
		return -1;
	}

	user = getenv("USER");
	if (user == NULL){
		return -1;
	}

	pid = getpid();
	if (pid < 0){
		return -1;
	}

	strtoupper(user);

	sprintf(buf, "%s/Downloads/%s.%d", home, user, pid);

	return strlen(buf);
}

int 
main(void){

	int longitud = 1024;
	char path[longitud];

	makepath(path, longitud);
	printf("%s\n",path);

	exit(EXIT_SUCCESS);
}