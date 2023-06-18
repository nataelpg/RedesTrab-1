#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "message.h"

#define ETHERNET "lo"
#define TIMEOUT 0xFF

int ConexaoRawSocket(char *device)
{
  int soquete;
  struct ifreq ir;
  struct sockaddr_ll endereco;
  struct packet_mreq mr;

  soquete = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));  	/*cria socket*/
  if (soquete == -1) {
    printf("Erro no Socket\n");
    exit(-1);
  }

  memset(&ir, 0, sizeof(struct ifreq));  	/*dispositivo eth0*/
  memcpy(ir.ifr_name, device, sizeof(device));
  if (ioctl(soquete, SIOCGIFINDEX, &ir) == -1) {
    printf("Erro no ioctl\n");
    exit(-1);
  }
	

  memset(&endereco, 0, sizeof(endereco)); 	/*IP do dispositivo*/
  endereco.sll_family = AF_PACKET;
  endereco.sll_protocol = htons(ETH_P_ALL);
  endereco.sll_ifindex = ir.ifr_ifindex;
  if (bind(soquete, (struct sockaddr *)&endereco, sizeof(endereco)) == -1) {
    printf("Erro no bind\n");
    exit(-1);
  }


  memset(&mr, 0, sizeof(mr));          /*Modo Promiscuo*/
  mr.mr_ifindex = ir.ifr_ifindex;
  mr.mr_type = PACKET_MR_PROMISC;
  if (setsockopt(soquete, SOL_PACKET, PACKET_ADD_MEMBERSHIP, &mr, sizeof(mr)) == -1)	{
    printf("Erro ao fazer setsockopt\n");
    exit(-1);
  }

  return soquete;
}

typedef struct mensagem {
    unsigned char ini;
    unsigned char tam;
    unsigned char sequencia;
    unsigned char tipo;
    char dados[TAM_BUFFER_DADOS];
    char lixo[29];
} mensagem_t;

void enviarStruct(int socket, const struct mensagem* msg) {
    char buffer[sizeof(struct mensagem)];
    memcpy(buffer, msg, sizeof(struct mensagem));
    send(socket, buffer, sizeof(struct mensagem), 0);
}

mensagem_t CriaMensagem(char msgTipo, char *msgDados) {
    mensagem_t msg;
    msg.ini = BIT_INICIO;
    msg.tam = strlen(msgDados); // guarda o tamanho da mensagem em bytes
    msg.sequencia = 0x00;
    msg.tipo = msgTipo;

    if (msgDados == NULL)
        memset(msg.dados, 0x00, sizeof(msg.dados));
    else
        strncpy(msg.dados, msgDados, TAM_BUFFER_DADOS);

    memset(msg.lixo, 0x00, sizeof(msg.lixo));

    return msg;
}


int main(int argc, char *argv[]) {
    int clientSocket, i;
    clientSocket = ConexaoRawSocket(ETHERNET);
    char path[1024];


    while (1) {
		/* memset(comando, '\0', (size_t)sizeof(comando));
		memset(arg, '\0', (size_t)sizeof(arg)); */
        getcwd(path, sizeof(path)); //armazena o caminho do diretório atual
		send(clientSocket, &path, sizeof(path), 0);
        printf("%s ", path);
		/* struct mensagem msg;
        CriaMensagem(&msg, 'T');
		enviarStruct(clientSocket, &msg); */
		
        /* char c = getchar();
        i = 0;
		while((c != ' ') && (c != '\n')){
			comando[i] = c;
			i++;
			c = getchar();
		}
		comando[i] = '\0';
        		if(c == ' '){ //Recebe o argumento, se tiver
			i = 0;
			while((c = getchar()) != '\n'){
				arg[i] = c;
				i++;
			}
			arg[i] = '\0';
		}
		if(!strcmp(comando, "cd")) // Comando cd remoto
			if(!strcmp(arg, ""))
				printf("Digite um caminho como argumento.\n");
            else{
				changeDirectory(socket, arg);
			} */


    }

    close(clientSocket);
}