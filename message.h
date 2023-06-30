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
#include <sys/time.h>
#include <fcntl.h>
#include <arpa/inet.h>


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
#define ENVIA_NOME 1011
#define ERRO 1100  
#define OK 1101
#define ACK 1110
#define NACK 1111


#define BIT_INICIO 01111110 // 01111110 sequencia para começar a ler a mensagem
#define TAM_BUFFER_DADOS 63 //tamanho do buffer de dados 
#define MAX_VEZES 100 // numero maximo de vezes que o servidor pode tentar enviar a mensagem
struct mensagem {
	unsigned int ini:8;
	unsigned int tam:6;
	unsigned int sequencia:6;
	unsigned int tipo:4;
	unsigned char dados[TAM_BUFFER_DADOS];
	unsigned int paridade:8; 
};
typedef struct mensagem mensagem_t;

int ConexaoRawSocket(char* socket); //Cria socket
mensagem_t *CriaMensagem(unsigned int msgTipo, unsigned char *msgDados, unsigned int sequencia, int tamDados); //Cria a mensagem que será mandada por parametro
unsigned int *readArchive(FILE *file);
void mandaResposta(int socket, mensagem_t* receivedMsg, mensagem_t *msg);
int setSocketTimeout(int socket);
void recebeConfirmacao(int socket, mensagem_t *msg);
unsigned int calculaParidade(mensagem_t *mensagem);

#endif
