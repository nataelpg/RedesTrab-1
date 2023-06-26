#ifndef MESSAGE_H
//bibliotecas raw socket
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/ethernet.h>
#include <linux/if_packet.h>
#include <linux/if.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/socket.h>  
#include <netinet/in.h>   
#include <unistd.h>   
#include <errno.h>


//código dos tipos
#define MESSAGE_H
#define BACKUP 0000 // backup 1 arquivo
#define BACKUP_DIR 0001// backup +1 arquivo
#define GET 0010 // recupera 1 arquivo 
#define GET_DIR 0011 // recupera +1 arquivo
#define ESCOLHE 0100 // escolher dir servidor
#define VERIFICA_BACKUP // verifica backup
#define RECUPERA 0110
#define MD5 0111
#define DADOS 1000
#define FIM_ARQ 1001
#define FIM_DIR 1010
#define INDEFINIDO 1011
#define ERRO 1100  
#define OK 1101
#define ACK 1110
#define NACK 1111


#define BIT_INICIO 01111110 // 01111110 sequencia para começar a ler a mensagem
#define TAM_BUFFER_DADOS 63 //tamanho do buffer de dados 

struct mensagem {
	unsigned int ini:8;
	unsigned int tam:6;
	unsigned int sequencia:6;
	unsigned int tipo:4;
	unsigned char* dados[TAM_BUFFER_DADOS];
	unsigned int paridade:8; 
};
typedef struct mensagem mensagem_t;

int ConexaoRawSocket(char* socket); //Cria socket
mensagem_t *CriaMensagem(unsigned int msgTipo, unsigned char *msgDados, unsigned int sequencia, int tamDados); //Cria a mensagem que será mandada por parametro
unsigned char *readArchive(FILE *file, int* tamArquivo);

#endif
