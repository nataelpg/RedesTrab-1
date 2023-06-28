#ifndef COMMAND_H
#define COMMAND_H

void getPath(char *comando, char *path);
void lsDir(char *comando);
void backupArquivo(char* argumento, int clientSocket);
void mudaDiretorio(char *path);

#endif