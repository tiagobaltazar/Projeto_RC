/************* UDP SERVER CODE *******************/

#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define BUF_SIZE 1024

void process_client(struct sockaddr_storage serverStorage, socklen_t addr_size, int udpSocket);
void chat(char ID[20], struct sockaddr_storage serverStorage, socklen_t addr_size, int udpSocket);

int main(){
  int udpSocket, nBytes;
  char buffer[BUF_SIZE];
  
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

  process_client(serverStorage, addr_size, udpSocket);

  return 0;
}

void process_client(struct sockaddr_storage serverStorage, socklen_t addr_size, int udpSocket){
    char ID[20];
    int nBytes;
    memset(ID, 0, strlen(ID));
    strcat(ID, "Medico1");
    char buffer[BUF_SIZE];
    int opt = -1;
    char warning_sign[BUF_SIZE];
    system("clear");
    memset(warning_sign, '\0', BUF_SIZE);
    memset(buffer, '\0', BUF_SIZE);

    stpcpy(warning_sign, "         *         \n");
    strcat(warning_sign, "        * *        \n");
    strcat(warning_sign, "       *   *       \n");
    strcat(warning_sign, "      *  *  *      \n");
    strcat(warning_sign, "     *   *   *     \n");
    strcat(warning_sign, "    *    *    *    \n");
    strcat(warning_sign, "   *           *   \n");
    strcat(warning_sign, "  *      *      *  \n");
    strcat(warning_sign, " *               * \n");
    strcat(warning_sign, "*******************");
    
    strcpy(buffer,       "      ALERTA!!!\n");
    strcat(buffer, warning_sign);
    strcat(buffer, "\n\nMédico /*Insersiri Id do médico*/ acaba de sofrer uma agressão\n");

    printf("MENU:\n1- ALARME!\n2- CHAT\n");
    scanf("%d", &opt);

    switch (opt){
    case 1:{
        //system("clear");
        sendto(udpSocket,buffer,nBytes,0,(struct sockaddr *)&serverStorage,addr_size);
        memset(buffer, '\0', BUF_SIZE);
        nBytes = recvfrom(udpSocket, buffer, BUF_SIZE, 0, (struct sockaddr *)&serverStorage, &addr_size);
        printf("\n%s", buffer);
        memset(buffer, '\0', BUF_SIZE);
        //printf("\ncheguei");
        break;
    }
    
    case 2:
        //system("clear");
        memset(buffer, '\0', BUF_SIZE);
        strcat(buffer, "chat");
        sendto(udpSocket,buffer,nBytes,0,(struct sockaddr *)&serverStorage,addr_size);
        chat(ID, serverStorage, addr_size, udpSocket);
        //printf("\ncheguei2");
        break;
    }
}

void chat(char ID[20], struct sockaddr_storage serverStorage, socklen_t addr_size, int udpSocket){
    system("clear");
    printf("\ncheguei");
    int pid, nBytes;
    char ID_2[20];
    pid = fork();
    char buffer[BUF_SIZE];
    memset(buffer, '\0', BUF_SIZE);
    FILE *fp;
    int chat = -1;
    char filename[20];
    char mensagem[BUF_SIZE];
    memset(ID_2, '\0', strlen(ID_2));
    memset(filename, '\0', strlen(filename));
    
    nBytes = recvfrom(udpSocket, buffer, BUF_SIZE, 0, (struct sockaddr *)&serverStorage, &addr_size);
    strcat(ID_2, buffer);
    strcat(filename, "chat_hist_");
    strcat(filename, ID);
    strcat(filename, "_");
    strcat(filename, ID_2);
    strcat(filename, ".txt");
    printf("\ncheguei3");

    if(pid == 0){
        do{
            fp = fopen(filename, "a");
            memset(buffer, '\0', BUF_SIZE);
            nBytes = recvfrom(udpSocket, buffer, BUF_SIZE, 0, (struct sockaddr *)&serverStorage, &addr_size);
            if (strcmp(buffer, "sair")){
                chat = 0;
                fprintf(fp, "=================================== end of session ===================================\n");
                fclose(fp);
            }
            else{
                memset(mensagem, '\0', BUF_SIZE);
                strcat(mensagem, buffer);
                printf("\n%s", mensagem);
                fprintf(fp, "%s\n", mensagem);
                memset(buffer, '\0', BUF_SIZE);
                memset(mensagem, '\0', BUF_SIZE);  
            }         
        } while (chat != 0);
    }
    else{
        do{
            memset(mensagem, '\0', BUF_SIZE);
            fgets(mensagem, BUF_SIZE, stdin);
            mensagem[strcspn(mensagem, "\n")] = 0;
            if(strcmp(mensagem, "sair") == 0){
                memset(buffer, '\0', BUF_SIZE);
                strcat(buffer, mensagem);
                sendto(udpSocket, buffer, nBytes, 0, (struct sockaddr *) &serverStorage, addr_size);
                chat = 0;
                fprintf(fp, "=================================== end of session ===================================\n");
                fclose(fp);
            }
            else{
                strcat(buffer, ID);
                strcat(buffer, ": ");
                strcat(buffer, mensagem);
                sendto(udpSocket, buffer, nBytes, 0, (struct sockaddr *) &serverStorage, addr_size);
                fprintf(fp, "%s\n", buffer);
            }
        }while(chat != 0);
    }
}