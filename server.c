/************* UDP SERVER CODE *******************/

#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define BUF_SIZE 1024

void CHAT(char ID[20], struct sockaddr_storage serverStorage, socklen_t addr_size, int udpSocket);

int main(){
  int udpSocket, nBytes;
  char buffer[BUF_SIZE];
  char ID[20];
  memset(ID, 0, strlen(ID));
  strcat(ID, "Seguranca1");
  
  struct sockaddr_in serverAddr, clientAddr;
  struct sockaddr_storage serverStorage;
  
  socklen_t addr_size, client_addr_size;
  int i;

  /*Create UDP socket*/
  udpSocket = socket(PF_INET, SOCK_DGRAM, 0);

  /*Configure settings in address struct*/
  serverAddr.sin_family = AF_INET;
  serverAddr.sin_port = htons(7891);
  serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
  memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);  

  /*Bind socket with address struct*/
  bind(udpSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr));

  /*Initialize size variable to be used later on*/
  addr_size = sizeof serverStorage;
  system("clear");
  /* Try to receive any incoming UDP datagram. Address and port of 
      requesting client will be stored on serverStorage variable */
  nBytes = recvfrom(udpSocket,buffer,BUF_SIZE,0,(struct sockaddr *)&serverStorage, &addr_size);
  if(strcmp(buffer, "chat") != 0){
    printf("%s", buffer);
    memset(buffer, '\0', BUF_SIZE);
    strcat(buffer, ID);
    strcat(buffer, " a caminho!");
    sendto(udpSocket,buffer,nBytes,0,(struct sockaddr *)&serverStorage,addr_size);
    memset(buffer, '\0', BUF_SIZE);
  }
  if(strcmp(buffer, "chat") == 0){
    CHAT(ID, serverStorage, addr_size, udpSocket);
  }

  return 0;
}

void CHAT(char ID[20], struct sockaddr_storage serverStorage, socklen_t addr_size, int udpSocket){
  char buffer[BUF_SIZE];
  char mensagem[BUF_SIZE];
  char filename[20];
  char aux[BUF_SIZE];
  int nBytes, chat;
  chat = -1;
  FILE *fp;

  memset(buffer, '\0', BUF_SIZE);
  nBytes = recvfrom(udpSocket,buffer,BUF_SIZE,0,(struct sockaddr *)&serverStorage, &addr_size);
  memset(filename, '\0', strlen(filename));
  strcat(filename, "chat_hist_");
  strcat(filename, ID);
  strcat(filename, "_");
  strcat(filename, buffer);
  strcat(filename, ".txt");
  memset(buffer, '\0', BUF_SIZE);

  do{
    fp = fopen(filename, "a");
    memset(buffer, '\0', BUF_SIZE);
    nBytes = recvfrom(udpSocket,buffer,BUF_SIZE,0,(struct sockaddr *)&serverStorage, &addr_size);
    if(strcmp(buffer, "sair") == 0){
      chat = 0;
      fprintf(fp, "===================== end of session =====================\n");
      break;
    }
    fprintf(fp, "%s\n", buffer);
    printf("%s", buffer);
    memset(buffer, '\0', BUF_SIZE);
    printf("\nYou: ");
    strcat(buffer, ID);
    strcat(buffer, ": ");
    fgets(mensagem, BUF_SIZE, stdin);
    mensagem[strcspn(mensagem, "\n")] = 0;
    strcat(buffer, mensagem);
    if(strcmp(mensagem, "sair") == 0){
      memset(buffer, '\0', BUF_SIZE);
      strcat(buffer, mensagem);
      chat = 0;
      sendto(udpSocket,buffer,nBytes,0,(struct sockaddr *)&serverStorage,addr_size);
      fprintf(fp, "===================== end of session =====================\n");
      break;
    }
    else{
      sendto(udpSocket,buffer,nBytes,0,(struct sockaddr *)&serverStorage,addr_size);
      strcat(aux, ID);
      strcat(aux, ": ");
      strcat(aux, mensagem);
      fprintf(fp, "%s\n", aux);
      memset(aux, '\0', BUF_SIZE);
    }
    memset(mensagem, '\0', BUF_SIZE);
    memset(buffer, '\0', BUF_SIZE);
    fclose(fp);
  }while(chat != 0);
}