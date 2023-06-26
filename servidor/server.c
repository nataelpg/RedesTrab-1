#include "message.h"
#include <sys/stat.h>
#include <time.h>
#define ETHERNET "lo"

int main(int argc, char *argv[]) {
    mensagem_t msg;
    int serverSocket = ConexaoRawSocket(ETHERNET);
    char path[1024];
    getcwd(path, sizeof(path)); // stores the current directory path

    while (1) {
        mensagem_t receivedMsg;
        char* comando = "";
        struct sockaddr_ll addr;
        socklen_t addr_len = sizeof(addr);;
        // se o arquivo já existe no diretório atual, não é necessário fazer backup
        // if (access("output.txt", F_OK) != -1) {
        //     printf("Arquivo já existe no diretório atual.\n");
        // }
        FILE* file = fopen("output.txt", "a");
        if (!file) {
            printf("Error opening file.\n");
            return 1;
        }

        ssize_t bytesReceived = recv(serverSocket, &receivedMsg, sizeof(mensagem_t), 0);
        printf("Bytes received: %ld\n", bytesReceived);
        printf ("Tipo da mensagem: %d\n", receivedMsg.tipo);
        if (receivedMsg.tipo == BACKUP && receivedMsg.ini == BIT_INICIO ){
            if (file == NULL) {
                printf("Error opening file.\n");
                return 1;
            }
            printf("%s\n", receivedMsg.dados);
            fwrite(receivedMsg.dados, sizeof(char), strlen(receivedMsg.dados), file);
            fflush(file);
        }
    }
    return 0;
}