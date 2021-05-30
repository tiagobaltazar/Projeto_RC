/************* UDP CLIENT CODE *******************/

#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#define BUF_SIZE 1024

void CHAT(char ID[20],struct sockaddr_in serverAddr, socklen_t addr_size, int clientSocket);

int main(){
    int clientSocket, portNum, nBytes;
    char buffer[BUF_SIZE];
    struct sockaddr_in serverAddr;
    socklen_t addr_size;
    char ID[20];
    char mensagem[BUF_SIZE];
    int chat, opt;
    chat = -1;
    opt = -1;
    memset(ID, 0, strlen(ID));
    strcat(ID, "Medico1");

    /*Create UDP socket*/
    clientSocket = socket(PF_INET, SOCK_DGRAM, 0);
    
    /*Configure settings in address struct*/
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(7891);
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);
    
    /*Initialize size variable to be used later on*/
    addr_size = sizeof serverAddr;

    char warning_sign[500];

    stpcpy(warning_sign, "         *         \n");
    strcat(warning_sign, "        * *        \n");
    strcat(warning_sign, "       *   *       \n");
    strcat(warning_sign, "      *  *  *      \n");
    strcat(warning_sign, "     *   *   *     \n");
    strcat(warning_sign, "    *    *    *    \n");
    strcat(warning_sign, "   *           *   \n");
    strcat(warning_sign, "  *      *      *  \n");
    strcat(warning_sign, " *               * \n");
    strcat(warning_sign, "* * * * * * * * * *");
    
    strcpy(buffer,       "      ALERTA!!!\n");
    strcat(buffer, warning_sign);
    strcat(buffer, "\n\nMédico ");
    strcat(buffer, ID);
    strcat(buffer, "acaba de sofrer uma agressão\n");
        
    nBytes = strlen(buffer) + 1;
    system("clear");

    printf("MENU:\n1- ALARME!\n2- CHAT\n");
    scanf("%d", &opt);

    if(opt == 1){
        sendto(clientSocket,buffer,nBytes,0,(struct sockaddr *)&serverAddr,addr_size);
        memset(buffer, '\0', BUF_SIZE);
        nBytes = recvfrom(clientSocket,buffer,BUF_SIZE,0,NULL, NULL);
        printf("%s\n",buffer);
    }
    
    if(opt == 2){
        memset(buffer, '\0', BUF_SIZE);

        strcat(buffer, "chat");
        sendto(clientSocket,buffer,nBytes,0,(struct sockaddr *)&serverAddr,addr_size);
        memset(buffer, '\0', BUF_SIZE);
        strcat(buffer, ID);
        sendto(clientSocket,buffer,nBytes,0,(struct sockaddr *)&serverAddr,addr_size);
        memset(buffer, '\0', BUF_SIZE);

        do{
            printf("\nYou: ");
            memset(buffer, '\0', BUF_SIZE);
            strcat(buffer, ID);
            strcat(buffer, ": ");
            fgets(mensagem, BUF_SIZE, stdin);
            mensagem[strcspn(mensagem, "\n")] = 0;
            strcat(buffer, mensagem);
            if(strcmp(mensagem, "sair") == 0){
                memset(buffer, '\0', BUF_SIZE);
                strcat(buffer, mensagem);
                chat = 0;
                sendto(clientSocket,buffer,nBytes,0,(struct sockaddr *)&serverAddr,addr_size);
                break;
            }
            else{
                sendto(clientSocket,buffer,nBytes,0,(struct sockaddr *)&serverAddr,addr_size);
            }
            memset(mensagem, '\0', BUF_SIZE);
            memset(buffer, '\0', BUF_SIZE);
            nBytes = recvfrom(clientSocket,buffer,BUF_SIZE,0,NULL, NULL);
            if(strcmp(buffer, "sair") == 0){
                chat = 0;
                break;
            }
            printf("%s", buffer);
            memset(buffer, '\0', BUF_SIZE);
        }while(chat != 0);

    }
    
    //CHAT(ID, serverAddr, addr_size, clientSocket);
    
    return 0;
}