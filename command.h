#ifndef COMMAND_H
#define COMMAND_H
#define BIT_INICIO 01111110

void getPath(char *comando, char *path);
void lsDir(char *comando);
void backupArquivo(char* argumento, int clientSocket);
void mudaDiretorio(char *path);
int recuperaArquivo(const char *argumento, int clientSocket);

#endif