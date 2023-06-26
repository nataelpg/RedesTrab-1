#include "message.h"
#include <sys/stat.h>
#include <time.h>
#include <string.h>
#define ETHERNET "enp2s0"

int main(int argc, char *argv[])
{
    mensagem_t receivedMsg;
    int serverSocket = ConexaoRawSocket(ETHERNET);
    char path[1024];
    getcwd(path, sizeof(path)); // stores the current directory path
    FILE *file = fopen("received_messages.txt", "a");
    if (file == NULL)
    {
        printf("Error opening file.\n");
        return 1;
    }

    while (1)
    {
        memset(&receivedMsg, 0, sizeof(mensagem_t));
        struct sockaddr_ll addr;
        socklen_t addr_len = sizeof(addr);

        ssize_t bytesReceived = recvfrom(serverSocket, &receivedMsg, sizeof(mensagem_t), 0, (struct sockaddr *)&addr, &addr_len);
        // ve marcador de inicio

        if ((bytesReceived == sizeof(mensagem_t)) && (receivedMsg.ini == (unsigned char)BIT_INICIO)) {
            // Mensagem recebida
            if (file == NULL) {
                printf("Error opening file.\n");
                return 1;
            }
            printf("Received message:\n");
            printf("  ini: %u\n", receivedMsg.ini);
            printf("  tam: %u\n", receivedMsg.tam);
            printf("  sequencia: %u\n", receivedMsg.sequencia);
            printf("  tipo: %u\n", receivedMsg.tipo);
            printf("  dados: %s\n", receivedMsg.dados);
            printf("  paridade: %u\n", receivedMsg.paridade);
            printf("--------------------------------------\n");
            if (fprintf(file, "%s\n", receivedMsg.dados) < 0) {
                printf("Error writing to file.\n");
                // Handle the error accordingly (e.g., close the file, exit the program)
                switch (receivedMsg.tipo) {
                case 0x00:
                    printf("  tipo: %u\n", receivedMsg.tipo);
                    break;
                case 0x01:
                    printf("  tipo: %u\n", receivedMsg.tipo);
                    break;
                case 0x02:
                    printf("  tipo: %u\n", receivedMsg.tipo);
                    break;
                }
                fprintf(file, "\n");
                printf("%s\n", receivedMsg.dados);
                fprintf(file, "%s\n", receivedMsg.dados);
                fflush(file);

                //     if (file == NULL) {
                //         printf("Error opening file.\n");
                //         return 1;
                //     }
                //     printf("%s\n", receivedMsg.dados);
                //     fprintf(file, "%s\n", receivedMsg.dados);
                //     fflush(file);
                //     // printf("  paridade: %u\n", receivedMsg.paridade);
            }
        }
    }
    fclose(file);
    return 0;
}