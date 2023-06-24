#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "message.h"
#include "command.h"

#define ETHERNET "lo"
#define TIMEOUT 0xFF

int main(int argc, char *argv[]) {
int clientSocket, i, select;
char *path, comando[100], arg[100], *token = NULL;
    // char* comando, argumento;
    // char *token = NULL;
    FILE* arq = fopen("input.txt", "r");
    if (!arq) {
        printf("Erro ao abrir o arquivo.\n");
        return 1;
    }
    unsigned char *arquivo = readArchive(arq);
    if (!arquivo) {
        printf("Erro ao ler o arquivo.\n");
        fclose(arq);
        return 1;
    }
    fclose(arq);

    // printf("Mensagem: %s\n", mensagem);
    clientSocket = ConexaoRawSocket(ETHERNET);
    mensagem_t msg;
    while(1) {
        fgets(comando, sizeof(comando), stdin);
        // token = strtok(comando, " ");
        // printf("%s\n", token);  //armazena o comando na variavel token 
        // path = strtok(NULL, "\n");
        // printf("%s\n", path);  //armazena o caminho na variavel path
        select = leComando(comando);
        if (select == -1)
            break;
        }


        // se o arquivo possui mais de 63 bytes, divide em pacotes
        if(strlen(arquivo) > TAM_BUFFER_DADOS) {
        int pacotes = strlen(arquivo) / TAM_BUFFER_DADOS;
        int resto = strlen(arquivo) % TAM_BUFFER_DADOS;

        for(i = 0; i < pacotes + 1; i++) {
            char parte[TAM_BUFFER_DADOS + 1];
            strncpy(parte, arquivo + (i * TAM_BUFFER_DADOS), TAM_BUFFER_DADOS);
            parte[TAM_BUFFER_DADOS] = '\0';  // Adiciona o caractere nulo ao final da parte

            // se o tamanho da parte for menor que o tamanho do buffer, preenche com 0 o restante
            if(strlen(parte) < TAM_BUFFER_DADOS) {
                int j;
                for(j = strlen(parte); j < TAM_BUFFER_DADOS; j++) {
                    parte[j] = '0';
                }
            }

            CriaMensagem(&msg, 0000, parte);
            send(clientSocket, &msg, sizeof(msg), 0);
            printf("Mensagem enviada: %s\n", msg.dados);

        }
    }
        // send(clientSocket, mensagem, strlen(mensagem), 0);
        // if(!strcmp(comando, "exit")) {
        //   printf("saindo\n");
        //   break;
        // }
        // if(!strcmp(comando, "cd")) // Comando cd remoto
        //   if(!strcmp(argumento, ""))
        //     printf("Digite um caminho como argumento.\n");
        //   else{
        //     EnviaChangeDirectory(socket, argumento);
    close(clientSocket);
    }