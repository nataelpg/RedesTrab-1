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

int backupArquivo(char* argumento, int clientSocket, mensagem_t msg){
	if (argumento){
		FILE* arq = fopen(argumento, "r");
		if (!arq) {
			printf("Erro ao abrir o arquivo.\n");
			return 1;
		}
		unsigned char *arquivo = readArchive(arq);
		if (!arquivo) {
			printf("Erro ao ler o arquivo.\n");
			fclose(arq);
			return 1;
		}
		
		char nomeArquivo[63 + 1];
		strncpy(nomeArquivo, argumento, 63);
		nomeArquivo[63] = '\0'; 
		CriaMensagem(&msg, 0000, nomeArquivo, 0);
		send(clientSocket, &msg, sizeof(msg), 0);
		// printf("Nome arquivo enviado: %s\n", msg.dados);

		int pacotes = 0;
		printf ("Mensagem: %s\n", arquivo);
		
		if(strlen((char*)arquivo) > 63) {
        	int pacotes = strlen((char*)arquivo) / 63;
        	int resto = strlen((char*)arquivo) % 63;

			for(int i = 0; i < pacotes + 1; i++) {
				char parte[63 + 1];
				strncpy(parte, (char*)(arquivo + (i * 63)), 63);
				parte[63] = '\0';  // Adiciona o caractere nulo ao final da parte

				// se o tamanho da parte for menor que o tamanho do buffer, preenche com 0 o restante
				// if(strlen(parte) < 63) {
				// 	int j;
				// 	for(j = strlen(parte); j < 63; j++) {
				// 		parte[j] = '0';
				// 	}
				// }

				CriaMensagem(&msg, 0000, parte, i + 1);
				send(clientSocket, &msg, sizeof(msg), 0);
				printf("Mensagem enviada: %s\n", msg.dados);

			}
		}
		fclose(arq); 
	}
	return 0;
}
// int leComando (char* comando){
// 	printf ("comando: %s\n", comando);
//     char *token = NULL;
// 	char *argumento = NULL;
//     token = strtok(comando, " ");
// 	token[strcspn(token, "\n")] = '\0';
// 	argumento = strtok(NULL, " ");
// 	if (argumento)
// 		argumento[strcspn(argumento, "\n")] = '\0';
//     if (!strcmp(token, "cd")) {
// 		enviaCD(comando);
// 	} else if (!strcmp(token, "ls")) {
// 		lsDir(argumento);
// 		return 1;
// 	} else if (!strcmp(token, "Backup") || !strcmp(token, "BACKUP") || !strcmp(token, "backup")) {
// 		/* putArchive(select = 1); */
// 		return 1;
// 	} else if (!strcmp(token, "get")){
// 		/* getArchive(); */
// 		return 1;
// 	} else if(!strcmp(token, "exit")){
// 		return -1;
// 	}
// }

void getPath(char *comando, char *path) {

    char *token = NULL;
    token = strtok(comando, " ");
    token = strtok(NULL, " ");
    if(token) {
        strcpy(path, token);
        path[strcspn(path, "\n")] = '\0';
    }
}
