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

int backupArquivo(char* argumento, int clientSocket, mensagem_t msg) {
    if (argumento) {
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
        // Send the filename to the server

        printf("Mensagem: %s\n", arquivo);

        if (strlen(arquivo) > TAM_BUFFER_DADOS) {
            int sequencia = strlen(arquivo) / TAM_BUFFER_DADOS;
            int resto = strlen(arquivo) % TAM_BUFFER_DADOS;

            for (int i = 0; i < sequencia + 1; i++) {
                char parte[TAM_BUFFER_DADOS + 1];
                strncpy(parte, (const char*)arquivo + (i * TAM_BUFFER_DADOS), TAM_BUFFER_DADOS);
                parte[TAM_BUFFER_DADOS] = '\0';  // Adiciona o caractere nulo ao final da parte

                // se o tamanho da parte for menor que o tamanho do buffer, preenche com 0 o restante
                if (strlen(parte) < TAM_BUFFER_DADOS) {
                    int j;
                    for (j = strlen(parte); j < TAM_BUFFER_DADOS; j++) {
                        // parte[j] = '0';
                    }
                }

                CriaMensagem(&msg, BACKUP, parte, sequencia);
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
