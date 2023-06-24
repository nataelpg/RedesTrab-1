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
        char* comando;
        struct sockaddr_ll addr;
        socklen_t addr_len = sizeof(addr);

        ssize_t bytesReceived = recv(serverSocket, &receivedMsg, sizeof(mensagem_t), 0);

        // if (bytesReceived == sizeof(mensagem_t)) {
        //     // Message received
        //     if (file == NULL) {
        //         printf("Error opening file.\n");
        //         return 1;
        //     }
        //     printf("%s\n", receivedMsg.dados);
        //     fprintf(file, "%s\n", receivedMsg.dados);
        //     fflush(file);
        //     // printf("  paridade: %u\n", receivedMsg.paridade);
        // }
    }
    return 0;
}



            // printf("Received message:\n");
            // printf("  ini: %u\n", receivedMsg.ini);
            // printf("  tam: %u\n", receivedMsg.tam);
            // printf("  sequencia: %u\n", receivedMsg.sequencia);
            // printf("  tipo: %u\n", receivedMsg.tipo);
            // switch (comando){
            // case 'CD':
            //     printf("  tipo: %u\n", receivedMsg.tipo);
            //     break;
            // case 'LS':
            //     printf("  tipo: %u\n", receivedMsg.tipo);
            //     break;
            // case 'exit':
            //     printf("  tipo: %u\n", receivedMsg.tipo);
            //     break;
            // }
