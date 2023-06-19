#include "message.h"
#include <sys/stat.h>
#include <time.h>
#define ETHERNET "lo"

int main(int argc, char *argv[]) {
    mensagem_t msg;
    int serverSocket = ConexaoRawSocket(ETHERNET);
    char path[1024];
    getcwd(path, sizeof(path)); // stores the current directory path
    FILE *file = fopen("received_messages.txt", "a");
    if (file == NULL) {
          printf("Error opening file.\n");
          return 1;
      }
    
    while (1) {
        mensagem_t receivedMsg;
        struct sockaddr_ll addr;
        socklen_t addr_len = sizeof(addr);

        ssize_t bytesReceived = recv(serverSocket, &receivedMsg, sizeof(mensagem_t), 0);

        if (bytesReceived == sizeof(mensagem_t)) {
            // Message received
            // printf("Received message:\n");
            // printf("  ini: %u\n", receivedMsg.ini);
            // printf("  tam: %u\n", receivedMsg.tam);
            // printf("  sequencia: %u\n", receivedMsg.sequencia);
            // printf("  tipo: %u\n", receivedMsg.tipo);
            printf("%s\n", receivedMsg.dados);
            if (fprintf(file, "%s\n", receivedMsg.dados) < 0) {
                printf("Error writing to file.\n");
                // Handle the error accordingly (e.g., close the file, exit the program)
            }
            fprintf(file, "\n");
            // printf("  paridade: %u\n", receivedMsg.paridade);
        }
    }
    return 0;
}
