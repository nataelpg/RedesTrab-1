#include "message.h"
#include <sys/stat.h>
#include <time.h>
#define ETHERNET "enp2s0"
#define BIT_INICIO 01111110

int main(int argc, char *argv[]) {
    int serverSocket = ConexaoRawSocket("lo");
    char path[1024];
    getcwd(path, sizeof(path)); // stores the current directory 
    mensagem_t receivedMsg;
    // recv(serverSocket, &receivedMsg, 67, 0);
    if (access("output.txt", F_OK) != -1) {
        remove("output.txt");
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
    int ultimaSequencia = 0;
    while (1) {
        char* comando = "";
        struct sockaddr_ll addr;
        socklen_t addr_len = sizeof(addr);
        // se o arquivo já existe no diretório atual, não é necessário fazer backup
        /* printf("Tipo da mensagem: %d\n", receivedMsg.tipo); */
        printf ("tamanho da mensagem: %d\n", receivedMsg.tam);
        printf ("tamanho da mensagem2: %d\n", strlen(receivedMsg.dados));
        /* printf ("Dados da mensagem: %s\n", receivedMsg.dados); */
        if (receivedMsg.sequencia == 0) {
            ultimaSequencia = -1;
        }
        if ((receivedMsg.sequencia != ultimaSequencia) && (receivedMsg.ini == (unsigned char)BIT_INICIO)) {
            printf("%s\n", receivedMsg.dados);
            fwrite(receivedMsg.dados, sizeof(unsigned char), strlen(receivedMsg.dados), file);
            printf("Sequencia: %d\n", receivedMsg.sequencia);
            fflush(file);
        }
        ultimaSequencia = receivedMsg.sequencia;
        recv(serverSocket, &receivedMsg, 67, 0);
    }
    return 0;
}

// unsigned char *msg = createString(root, &fileSize);
// writeFile(msg, root->head->message->data, fileSize);

// void writeFile(unsigned char *string, unsigned char *fileName, int fileSize) {
//     char filePath[200];
//     strcpy(filePath, "./backup/");
//     strcat(filePath, fileName);
//     FILE *file = fopen(filePath, "w");
//     fwrite(string, fileSize, 1, file);
//     fclose(file);
// }