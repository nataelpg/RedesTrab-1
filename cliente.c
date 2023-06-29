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

    // printf("Mensagem: %s\n", mensagem);
    clientSocket = ConexaoRawSocket("lo");
    while(1) {
        fgets(comando, sizeof(comando), stdin);
        char *token = NULL;
        char *argumento = NULL;
        token = strtok(comando, " ");
        token[strcspn(token, "\n")] = '\0';
        argumento = strtok(NULL, " ");
        if (argumento)
            argumento[strcspn(argumento, "\n")] = '\0';      
        if (!strcmp(token, "cd")) {
            mudaDiretorio(argumento);
        } else if (!strcmp(token, "ls")) {
            lsDir(argumento);
        } else if (!strcmp(token, "Backup") || !strcmp(token, "BACKUP") || !strcmp(token, "backup")) {
            backupArquivo(argumento, clientSocket);
        } else if (!strcmp(token, "get")){
            recuperaArquivo(argumento, clientSocket);
        } else if(!strcmp(token, "exit")){
            
            break;
        }
    close(clientSocket);
    }
}