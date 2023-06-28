#include "message.h"
#include <sys/stat.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

#define ETHERNET "enp2s0"
#define BIT_INICIO 01111110

int main(int argc, char *argv[]) {
    int serverSocket = ConexaoRawSocket("lo");
    char path[1024];
    getcwd(path, sizeof(path)); // stores the current directory 
    mensagem_t receivedMsg, *sentMessage;
    struct sockaddr_ll addr;
    socklen_t addr_len = sizeof(addr);
    // recv(serverSocket, &receivedMsg, 67, 0);
    if (access("output.png", F_OK) != -1) {
        remove("output.png");
    }
    FILE* file = fopen("output.png", "wb");
    if (!file) {
        printf("Error opening file.\n");
        return 1;
    }

    // Set line buffering for the file stream
    // if (setvbuf(file, NULL, _IOLBF, 0) != 0) {
    //     printf("Error setting line buffering mode for the file.\n");
    //     return 1;
    // }

    int ultimaSequencia = -1;
    while (1) {
        char* comando = "";
        // se o arquivo já existe no diretório atual, não é necessário fazer backup
        /* printf ("Dados da mensagem: %s\n", receivedMsg.dados); */
        recv(serverSocket, &receivedMsg, 67, 0);
        if (receivedMsg.sequencia != ultimaSequencia && (receivedMsg.ini == (unsigned char)BIT_INICIO)) {
            /* printf("Tipo da mensagem: %d\n", receivedMsg.tipo); */
            fwrite(receivedMsg.dados, sizeof(unsigned char), receivedMsg.tam, file);
            fflush(file);
            // printf ("Paridade recebida: %d\n", receivedMsg.paridade);
            // printf ("Paridade calculada: %d\n", calculaParidade(&receivedMsg));
            mandaResposta(serverSocket, receivedMsg.paridade, calculaParidade(&receivedMsg), sentMessage);
            ultimaSequencia = receivedMsg.sequencia;
        }
        // else if ((receivedMsg.tipo == 0) && (receivedMsg.sequencia != ultimaSequencia) && (receivedMsg.ini == (unsigned char)BIT_INICIO))
    }
    free(sentMessage);
    return 0;
}
