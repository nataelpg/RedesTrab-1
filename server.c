#include "message.h"
#include <sys/stat.h>
#include <time.h>
#define ETHERNET "lo"

int main(int argc, char *argv[]) {
    int serverSocket = ConexaoRawSocket(ETHERNET);
    char path[1024];
    getcwd(path, sizeof(path)); // stores the current directory path
    if (access("output.txt", F_OK) != -1) {
        remove("output.txt");
    }
    FILE* file = fopen("output.txt", "w");
    if (!file) {
        printf("Error opening file.\n");
        return 1;
    }

    // Set line buffering for the file stream
    // if (setvbuf(file, NULL, _IOLBF, 0) != 0) {
    //     printf("Error setting line buffering mode for the file.\n");
    //     return 1;
    // }

    while (1) {
        mensagem_t receivedMsg;
        char* comando = "";
        struct sockaddr_ll addr;
        socklen_t addr_len = sizeof(addr);
        // se o arquivo já existe no diretório atual, não é necessário fazer backup
        recv(serverSocket, &receivedMsg, 67, 0);
        printf("Tipo da mensagem: %d\n", receivedMsg.tipo);
        printf ("tamanho da mensagem: %d\n", receivedMsg.tam);
        printf ("Dados da mensagem: %s\n", receivedMsg.dados);
        if (receivedMsg.tam > 0) {
            printf("%s\n", receivedMsg.dados);
            fwrite(receivedMsg.dados, sizeof(unsigned char), strlen(receivedMsg.dados), file);
            fflush(file);
            printf("Sequencia da mensagem Original: %d\n", receivedMsg.sequencia);  
        }
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