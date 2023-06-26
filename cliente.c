#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "message.h"
#include "command.h"

#define ETHERNET "enp2s0"
#define TIMEOUT 0xFF

int main(int argc, char *argv[]) {
    int clientSocket, i, select;
    char *path, comando[100], arg[100], *token = NULL;
    // char* comando, argumento;
    // char *token = NULL;
    FILE* arq = fopen("input.txt", "r");
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
    fclose(arq);

    // printf("Mensagem: %s\n", mensagem);
    clientSocket = ConexaoRawSocket(ETHERNET);
    mensagem_t msg;
    while(1) {
        fgets(comando, sizeof(comando), stdin);
        // token = strtok(comando, " ");
        // printf("%s\n", token);  //armazena o comando na variavel token 
        // path = strtok(NULL, "\n");
        // printf("%s\n", path);  //armazena o caminho na variavel path
        char *token = NULL;
        char *argumento = NULL;
        token = strtok(comando, " ");
        token[strcspn(token, "\n")] = '\0';
        argumento = strtok(NULL, " ");
        if (argumento)
            argumento[strcspn(argumento, "\n")] = '\0';

        if (!strcmp(token, "cd")) {
            // enviaCD(comando);
        } else if (!strcmp(token, "ls")) {
            lsDir(argumento);
        } else if (!strcmp(token, "Backup") || !strcmp(token, "BACKUP") || !strcmp(token, "backup")) {
            backupArquivo(argumento, clientSocket, msg);
            /* putArchive(select = 1); */
        } else if (!strcmp(token, "get")){

            /* getArchive(); */
        } else if(!strcmp(token, "exit")){
            
            break;
        }
    close(clientSocket);
    }
}