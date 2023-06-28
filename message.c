#include "message.h"

int ConexaoRawSocket(char *device)
{
  int soquete;
  struct ifreq ir;
  struct sockaddr_ll endereco;
  struct packet_mreq mr;

  soquete = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));  	/*cria socket*/
  printf(" %s\n",strerror(errno));
  if (soquete == -1) {
    printf("Erro no Socket\n");
    exit(-1);
  }

  memset(&ir, 0, sizeof(struct ifreq));  	/*dispositivo eth0*/
  memcpy(ir.ifr_name, device, sizeof(device));
  if (ioctl(soquete, SIOCGIFINDEX, &ir) == -1) {
    printf(" %s\n",strerror(errno));
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

mensagem_t *CriaMensagem(unsigned int msgTipo, unsigned char *msgDados, unsigned int sequencia, int tamDados){
  mensagem_t *msg = malloc(sizeof(mensagem_t));

	msg->ini = BIT_INICIO;
	msg->tam = tamDados; // guarda o tamanho da mensagem em bytes
	msg->sequencia = sequencia; 
	msg->tipo = msgTipo;
	memcpy(msg->dados, msgDados, tamDados);
  msg->paridade = calculaParidade(msg);

  return msg;
}

unsigned int *readArchive(FILE *file) {
    int fileSize = 0;
    int c;
    while ((c = fgetc(file)) != EOF)
        fileSize++;
    rewind(file);

    return fileSize;
}

void mandaResposta(int socket, unsigned int paridade, mensagem_t *msg) {
  if (calculaParidade(msg) == paridade){
    msg = CriaMensagem(14, "", 0, 3);
    printf ("Ack enviado!\n");
  }
  else {
    msg = CriaMensagem(15, "", 0, 3);
    printf ("NACK enviado!\n");
  }
  // send(socket, &msg, 67, 0);
}

unsigned int calculaParidade(mensagem_t *mensagem) {
    unsigned int paridade = 0;
    int i;
    printf("%d\n", mensagem->tam);
    for(i=0; i<mensagem->tam; i++)
      paridade ^= mensagem->dados[i];
    return paridade;
}
