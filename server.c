#include "message.h"
#include <sys/stat.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include "command.h"

#define ETHERNET "enp2s0"
#define BIT_INICIO 01111110

int main(int argc, char *argv[]) {
    int serverSocket = ConexaoRawSocket("lo");
    char path[1024];
    getcwd(path, sizeof(path)); // stores the current directory 
    mensagem_t receivedMsg, *sentMsg;
    struct sockaddr_ll addr;
    socklen_t addr_len = sizeof(addr);
    // recv(serverSocket, &receivedMsg, 67, 0);

    FILE* file = NULL;
    int ultimaSequencia = -1;
    while (1) {
        recv(serverSocket, &receivedMsg, 67, 0); 
        if ((receivedMsg.tipo == 0) && receivedMsg.sequencia != ultimaSequencia && (receivedMsg.ini == (unsigned char)BIT_INICIO)) {
            printf ("Dados recebidos: %s\n", receivedMsg.dados);
            printf ("tamanho recebido: %d\n", receivedMsg.tam);
            fwrite(receivedMsg.dados, sizeof(unsigned char), receivedMsg.tam, file);
            fflush(file);
            mandaResposta(serverSocket, &receivedMsg, sentMsg);
            printf ("Sequencia recebida: %d\n", receivedMsg.sequencia);
            ultimaSequencia = receivedMsg.sequencia;
        }
        if ((receivedMsg.sequencia != ultimaSequencia) && receivedMsg.tipo == 11  && (receivedMsg.ini == (unsigned char)BIT_INICIO)) {
            printf("Nome do arquivo: %s\n", receivedMsg.dados);
            // se ja existe o arquivo o apaga 
            if (access(receivedMsg.dados, F_OK) != -1) {
                remove(receivedMsg.dados);
            }
            file = fopen(receivedMsg.dados, "wb");
            if (!file) {
                printf("Error opening file.\n");
                return 1;
            }
            ultimaSequencia = -1;
            continue;
        }
        else if ((receivedMsg.sequencia != ultimaSequencia) && receivedMsg.tipo == 9  && (receivedMsg.ini == (unsigned char)BIT_INICIO)) {
        {
            printf("Tamanho do arquivo: %d\n", receivedMsg.tam);
            printf ("Sequencia recebida: %d\n", receivedMsg.sequencia);
            fwrite(receivedMsg.dados, sizeof(unsigned char), receivedMsg.tam, file);
            fflush(file);
            ultimaSequencia = receivedMsg.sequencia;
            continue;
        }      
        ultimaSequencia = -1;        
        } else if((receivedMsg.tipo == 2) && (receivedMsg.ini == (unsigned char)BIT_INICIO)) {
            backupArquivo(receivedMsg.dados, serverSocket);
            printf ("Nome do arquivo: %s\n", receivedMsg.dados);
        }
    }
    fclose(file);
    return 0;
}
