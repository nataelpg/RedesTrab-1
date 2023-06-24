#include "message.h"

void lsDir(char *comando) {
	printf("argumento: %s\n", comando);
    if (!comando || !strcmp(comando, "")) {
        system("ls"); 
    } else if (!strcmp(comando, "-l")) {
        system("ls -l");
    } else if (strcmp(comando, "-a") == 0) {
        system("ls -a");
    } else {
        printf("Digite uma opção válida.\n");
    }
}


int leComando (char* comando){
	printf ("comando: %s\n", comando);
    char *token = NULL;
	char *argumento = NULL;
    token = strtok(comando, " ");
	token[strcspn(token, "\n")] = '\0';
	argumento = strtok(NULL, " ");
	if (argumento)
		argumento[strcspn(argumento, "\n")] = '\0';
    if (!strcmp(token, "cd")) {
		enviaCD(comando);
	} else if (!strcmp(token, "ls")) {
		lsDir(argumento);
		return 1;
	} else if (!strcmp(token, "Backup") || !strcmp(token, "BACKUP") || !strcmp(token, "backup")) {
		/* putArchive(select = 1); */
		return 1;
	} else if (!strcmp(token, "get")){
		/* getArchive(); */
		return 1;
	} else if(!strcmp(token, "exit")){
		return -1;
	}
}

void getPath(char *comando, char *path) {

    char *token = NULL;
    token = strtok(comando, " ");
    token = strtok(NULL, " ");
    if(token) {
        strcpy(path, token);
        path[strcspn(path, "\n")] = '\0';
    }
}
