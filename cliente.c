#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "message.h"
#include "command.h"

#define ETHERNET "lo"
#define TIMEOUT 0xFF

int main(int argc, char *argv[]) {
    int clientSocket, i, select;
    char *path, comando[100], arg[100], *token = NULL;
    // char* comando, argumento;
    // char *token = NULL;

    // printf("Mensagem: %s\n", mensagem);
    clientSocket = ConexaoRawSocket(ETHERNET);
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
            // send (clientSocket, argumento, strlen(argumento) + 1, 0);
            // printf ("Argumento: %s\n", argumento);
            backupArquivo(argumento, clientSocket);
            /* putArchive(select = 1); */
        } else if (!strcmp(token, "get")){

            /* getArchive(); */
        } else if(!strcmp(token, "exit")){
            
            break;
        }
    close(clientSocket);
    }
}