#include "message.h"
#include <unistd.h>

void lsDir(char *comando) {
	// printf("argumento: %s\n", comando);
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

int backupArquivo(const char *argumento, int clientSocket) {
    if (argumento) {
        mensagem_t *msg;
        FILE *arq = fopen(argumento, "rb");
        if (!arq) {
            perror("Erro ao abrir o arquivo");
            return 1;
        }

        int filesize = readArchive(arq);
        // unsigned char *arquivo = readArchive(arq, &filesize);
        // if (!arquivo) {
        //     printf("Erro ao ler o arquivo.\n");
        //     fclose(arq);
        //     return 1;
        // }
        unsigned char* arquivo = malloc(sizeof(unsigned char) * TAM_BUFFER_DADOS);

        // int sequencia = (filesize + TAM_BUFFER_DADOS - 1) / TAM_BUFFER_DADOS;
        // int resto = filesize % TAM_BUFFER_DADOS;

        ssize_t sentBytes = send(clientSocket, msg, 67, 0);
        if (sentBytes == -1) {
            perror("Erro ao enviar primeira mensagem");
            return 1;
        }
        // unsigned char buffer[1024];
        size_t bytesRead;
        int i = 0;
        while ((bytesRead = fread(arquivo, sizeof(unsigned char), sizeof(arquivo)*8 -1, arq)) > 62){ 
            msg = CriaMensagem(0, arquivo, i, TAM_BUFFER_DADOS);
            printf("Dados enviados: %s\n", msg->dados);
            printf ("Sequencia: %d\n", msg->sequencia);
            printf ("tamanho dados: %d\n", msg->tam);
            send(clientSocket, msg, 67, 0);
            recebeConfirmacao(clientSocket, msg);
            i++;
        }
        if (TAM_BUFFER_DADOS > filesize-TAM_BUFFER_DADOS*i){ 
            msg = CriaMensagem(0, arquivo, i+1, filesize-TAM_BUFFER_DADOS*i);
            send(clientSocket, msg, 67, 0);
        }

            // while ((bytesRead = fread(buffer, sizeof(unsigned char), sizeof(buffer), sourceFile)) > 0) {
            //     fwrite(buffer, sizeof(unsigned char), bytesRead, destinationFile);
            // }

        

        // for (i = 0; i < sequencia; i++) {
        //     int tamDados = (i == sequencia - 1 && resto > 0) ? resto : TAM_BUFFER_DADOS;
        //     unsigned char* parte = malloc(tamDados);
        //     memcpy(parte, arquivo + i * TAM_BUFFER_DADOS, tamDados);

        //     msg = CriaMensagem(1, parte, i, TAM_BUFFER_DADOS);
        //     send(clientSocket, msg, 67, 0);
        //     printf ("Sequencia: %d\n", msg->sequencia);
        //     printf ("tamanho dados: %d\n", msg->tam);
        //     /* printf("Mensagem enviada: %s\n", msg->dados); */
        // }

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

    FILE* arq = fopen(argumento, "wb");
    if (!arq) {
        printf("Erro ao abrir o arquivo.");
        return 1;
    }

    int tamanho = readArchive(arq);

    mensagem_t *receivedMsg;
    mensagem_t* sentMsg = calloc(1, sizeof(mensagem_t)+1);

    receivedMsg = CriaMensagem(2, argumento, 0, TAM_BUFFER_DADOS);
    send(clientSocket, receivedMsg, 67, 0);

    unsigned char* arquivo = malloc(sizeof(unsigned char) * TAM_BUFFER_DADOS);
    ssize_t recvReturn;

    while(1) {
        recvReturn = recv(clientSocket, receivedMsg, 67, 0);
        if(recvReturn == -1) {
            printf("Timeout!");
            continue;
        } else {
            if(receivedMsg->ini == (unsigned char)BIT_INICIO) {
                if(receivedMsg->tipo == 8) {
                    printf("Dados recebidos\n");
                    fwrite(arquivo, sizeof(char), receivedMsg->tam, arq);
                    mandaResposta(clientSocket, receivedMsg, sentMsg);
                } 
                else if(receivedMsg->tipo == 9) {
                    printf("Final do arquivo\n");
                    mandaResposta(clientSocket, receivedMsg, sentMsg);
                    fclose(arq);
                    free(arquivo);
                    return;
                }
            }
        }
    }

    free(arquivo);
    free(receivedMsg);
    return 0;
}