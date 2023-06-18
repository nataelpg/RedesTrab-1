#include "message.h"
#include <sys/stat.h>
#define ETHERNET "lo"

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

int main (int argc, char *argv[]) {
    mensagem_t msg;
    int serverSocket = ConexaoRawSocket(ETHERNET);
    char path[1024];
    getcwd(path, sizeof(path)); //armazena o caminho do diretório atual
    while (1) {
        char buffer[1024];
        struct sockaddr_ll addr;
        socklen_t addr_len = sizeof(addr);
        
        ssize_t bytesReceived = recvfrom(serverSocket, buffer, sizeof(buffer), MSG_DONTWAIT, (struct sockaddr*)&addr, &addr_len);
        
        if (bytesReceived > 0) {
            // Dados recebidos
            buffer[bytesReceived] = '\0';
            printf("%s\n", buffer);
        }
    /* switch(msg.tipo){
			case CD: cd_remoto(socket, &msg);break;
			case LS: ls_remoto(socket, &msg);break;
			case PUT: put_remoto(socket, &msg);break;
			case GET: get_remoto(socket, &msg);break;
			default: printf("Tipo de mensagem inválido\n");break;
		
        } */
    }
    close(serverSocket); 

    return 0;   
}
