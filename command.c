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

        int filesize;
        unsigned char *arquivo = readArchive(arq, &filesize);
        if (!arquivo) {
            printf("Erro ao ler o arquivo.\n");
            fclose(arq);
            return 1;
        }

        int sequencia = (filesize + TAM_BUFFER_DADOS - 1) / TAM_BUFFER_DADOS;
        int resto = filesize % TAM_BUFFER_DADOS;
        int i;

        for (i = 0; i < sequencia; i++) {
            int tamDados = (i == sequencia - 1 && resto > 0) ? resto : TAM_BUFFER_DADOS;
            unsigned char* parte = malloc(tamDados);
            memcpy(parte, arquivo + i * TAM_BUFFER_DADOS, tamDados);

            msg = CriaMensagem(1, parte, i, TAM_BUFFER_DADOS);
            ssize_t sentBytes = send(clientSocket, msg, 67, 0);
            printf ("Sequencia: %d\n", msg->sequencia);
            printf ("tamanho dados: %d\n", msg->tam);
            if (sentBytes == -1) {
                perror("Erro ao enviar mensagem");
                fclose(arq);
                free(arquivo);
                free(msg);
                return 1;
            }
            /* printf("Mensagem enviada: %s\n", msg->dados); */
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