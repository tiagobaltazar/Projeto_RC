/************* UDP CLIENT CODE *******************/


#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <netdb.h>
#include <string.h>

#define BUF_SIZE 1024

void CHAT(char ID[20],struct sockaddr_in serverAddr, socklen_t addr_size, int clientSocket);
void process(struct sockaddr_in serverAddr, socklen_t addr_size, int clientSocket);

int main(){
    int clientSocket, portNum, nBytes, chat;
    char buffer[BUF_SIZE], ID[20];
    struct sockaddr_in serverAddr;
    socklen_t addr_size;

    clientSocket = socket(PF_INET, SOCK_DGRAM, 0);

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(7891);
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);

    addr_size = sizeof serverAddr;

    chat = -1;
    memset(ID, 0, strlen(ID));
    strcat(ID, "Seguranca1");
    printf("\ncheguei");

    while(1){

        nBytes = recvfrom(clientSocket, buffer, BUF_SIZE, 0, NULL, NULL);
        printf("\n%s", buffer);
        if(strcmp(buffer, "0") != 0){
            printf("\n%s", buffer);
            memset(buffer, '\0', BUF_SIZE);
            strcat(buffer, ID);
            strcat(buffer, " a caminho!!");
            sendto(clientSocket,buffer,nBytes,0,(struct sockaddr *)&serverAddr,addr_size);
        }
        else{
            printf("\ncheguei");
            CHAT(ID, serverAddr, addr_size, clientSocket);
        }
    }
    return 0;
}

/*void process(struct sockaddr_in serverAddr, socklen_t addr_size, int clientSocket){
    
}*/

void CHAT(char ID[20],struct sockaddr_in serverAddr, socklen_t addr_size, int clientSocket){
    system("clear");

    int pid, nBytes;
    pid = fork();
    char buffer[BUF_SIZE];
    memset(buffer, '\0', BUF_SIZE);
    int chat = -1;
    char mensagem[BUF_SIZE];


    strcat(buffer, ID);
    sendto(clientSocket, buffer, nBytes, 0, (struct sockaddr *) &serverAddr, addr_size);
    memset(buffer, '\0', BUF_SIZE);
    
    if(pid == 0){
        do{
            memset(buffer, '\0', BUF_SIZE);
            nBytes = recvfrom(clientSocket, buffer, BUF_SIZE, 0, NULL, NULL);
            printf("\n%s", buffer);
            memset(buffer, '\0', BUF_SIZE);
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
                sendto(clientSocket, buffer, nBytes, 0, (struct sockaddr *) &serverAddr, addr_size);
                chat = 0;
            }
            else{
                strcat(buffer, ID);
                strcat(buffer, ": ");
                strcat(buffer, mensagem);
                sendto(clientSocket, buffer, nBytes, 0, (struct sockaddr *) &serverAddr, addr_size);
                memset(buffer, '\0', BUF_SIZE);
                memset(mensagem, '\0', BUF_SIZE);
            }
        }while(chat != 0);
    }
}