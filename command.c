#include "message.h"
#include <unistd.h>

void lsDir(char *comando) {
    if (!comando || !strcmp(comando, "")) {
        system("ls"); 
    } else if (!strcmp(comando, "-l")) {
        system("ls -l");
    } else if (strcmp(comando, "-a") == 0) {
        system("ls -a");
    } else {
        printf("Digite uma opção válida.\n");
    }
}

int backupArquivo(unsigned char *argumento, int clientSocket) {
    if (argumento) {
        mensagem_t *msg;
        FILE *arq = fopen(argumento, "rb");
        if (!arq) {
            perror("Erro ao abrir o arquivo");
            return 1;
        }

        int filesize = readArchive(arq);
        unsigned char* arquivo = malloc(sizeof(unsigned char) * TAM_BUFFER_DADOS);

        // int sequencia = (filesize + TAM_BUFFER_DADOS - 1) / TAM_BUFFER_DADOS;
        // int resto = filesize % TAM_BUFFER_DADOS;

        msg = CriaMensagem(11, argumento, 0, strlen(argumento));
        // IMPRIME ENVIA_NOME
        printf ("tipo mensagem: %d\n", msg->tipo);
        send(clientSocket, msg, 67, 0);
        printf ("mensagem enviada");
        // unsigned char buffer[1024];
        size_t bytesRead;
        int i = 0;
        while ((bytesRead = fread(arquivo, sizeof(unsigned char), sizeof(arquivo)*8 -1, arq)) > 62){ 
            msg = CriaMensagem(0, arquivo, i, TAM_BUFFER_DADOS);
            send(clientSocket, msg, 67, 0);
            printf ("Sequencia: %d\n", msg->sequencia);
            printf ("tamanho dados: %d\n", msg->tam);
            recebeConfirmacao(clientSocket, msg);
            i++;
        }
        if (TAM_BUFFER_DADOS > filesize-TAM_BUFFER_DADOS*i){ 
            msg = CriaMensagem(9, arquivo, i, filesize-TAM_BUFFER_DADOS*i);
            send(clientSocket, msg, 67, 0);
        }
        fclose(arq);
        free(arquivo);
        free(msg);
    }
    return 0;
}

void getPath(char *comando, char *path) {
    char *token = NULL;
    token = strtok(comando, " ");
    token = strtok(NULL, " ");
    if(token) {
        strcpy(path, token);
        path[strcspn(path, "\n")] = '\0';
    }
}

void mudaDiretorio(char *path) {
    if (chdir(path) != 0)
        printf("Diretório inválido!");
    char cwd[128];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("Diretório atual: %s\n", cwd);
    } else {
        perror("getcwd() error");
    }
}

int recuperaArquivo(const char *argumento, int clientSocket) {
    if (! argumento)
        return 1;   

    printf ("Argumento: %s\n", argumento);
    FILE* arq = fopen(argumento, "wb");
    if (!arq) {
        printf("Erro ao abrir o arquivo.");
        return 1;
    }

    int tamanho = readArchive(arq);

    mensagem_t *receivedMsg;
    mensagem_t* sentMsg = malloc(sizeof(mensagem_t));

    receivedMsg = CriaMensagem(2, argumento, 0, TAM_BUFFER_DADOS);
    send(clientSocket, receivedMsg, 67, 0);

    ssize_t recvReturn;
    int ultimaSequencia = -1;
    while(1) {
        recvReturn = recv(clientSocket, receivedMsg, 67, 0);
        printf ("sequencia recebida: %d\n", receivedMsg->sequencia);
        if(recvReturn == -1) {
            printf("Timeout!");
            continue;
        } else {
            if(receivedMsg->ini == (unsigned char)BIT_INICIO) {
                if(receivedMsg->tipo == 0 && receivedMsg->sequencia != ultimaSequencia) {
                    printf ("sequencia recebida: %d\n", receivedMsg->sequencia);
                    printf("Dados recebidos\n");
                    fwrite(receivedMsg->dados, sizeof(unsigned char), receivedMsg->tam, arq);
                    fflush(arq);
                    mandaResposta(clientSocket, receivedMsg, sentMsg);
                    ultimaSequencia = receivedMsg->sequencia;
                } 
                
                else if(receivedMsg->tipo == 9 && receivedMsg->sequencia != ultimaSequencia) {
                {
                    printf ("sequencia recebida: %d\n", receivedMsg->sequencia);
                    printf ("ultima sequencia: %d\n", ultimaSequencia);
                    printf("Final do arquivo\n");
                    fwrite(receivedMsg->dados, sizeof(unsigned char), receivedMsg->tam, arq);
                    fflush(arq);
                    mandaResposta(clientSocket, receivedMsg, sentMsg);
                    ultimaSequencia = receivedMsg->sequencia;
                    break;
                }
                ultimaSequencia = -1;
                }
            }
        }
    }
    fclose(arq);
    free(receivedMsg);
    return 0;
}