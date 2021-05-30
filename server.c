/*
	APLICAÇÃO CENTRAL (AC)
*/
//LIBRARIES
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <netdb.h>
#include <string.h>

//STATIC VARIABLES
#define SERVER_PORT     9000
#define BUF_SIZE	2048

//FUNCTIONS
void process_client(int client_fd);
void health_app();
void security_app();
void admin_app();
void erro(char *msg);
void den_reg(int client_fd);
void BTNalarm(int client_fd);
void editarS(int client_fd, char data[40]); 
void editarH(int client_fd, char data[40]);
void editarA(int client_fd, char data[40]); 
void optionH(int client_fd, char data[40]);
void optionS(int client_fd, char data[40]);
void optionA(int client_fd, char data[40]);
void deleteH(int client_fd, char data[40]);
void deleteS(int client_fd, char data[40]);
void den_vis(int client_fd, char data[40]);
void HsignUP(int client_fd);
void SsignUP(int client_fd);
void accountcheck(int client_fd);

//GLOBAL VARIABLES
FILE *text, *textaux, *redirect;
int s=1, h=1, a=1, sU=1, var=1;
int nread = 0;
char buffer[BUF_SIZE];

int main(){
  int fd, client;
  struct sockaddr_in addr, client_addr;
  int client_addr_size;
  addr.sin_family      = AF_INET;
  addr.sin_addr.s_addr = htonl(INADDR_ANY);
  addr.sin_port        = htons(SERVER_PORT);

  if ( (fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
	erro("na funcao socket");
  if ( bind(fd,(struct sockaddr*)&addr,sizeof(addr)) < 0)
	erro("na funcao bind");
  if( listen(fd, 5) < 0) 
	erro("na funcao listen");
  
  int nclients=0;
  system("clear");
  while (1) {
    client_addr_size = sizeof(client_addr);
    client = accept(fd,(struct sockaddr *)&client_addr,&client_addr_size);
      nclients++;
    if (client > 0) {
      if (fork() == 0) {
        close(fd);
        process_client(client);
        exit(0);
      }
    close(client);
    
    }
  }
  return 0;
}

void process_client(int client_fd){ 
  memset(buffer,0,BUF_SIZE); 
  nread = read(client_fd, buffer, BUF_SIZE-1);	
	buffer[nread] = '\0';
  h=strcmp(buffer,"health");
  s=strcmp(buffer,"security");
  a=strcmp(buffer,"admin");
  memset(buffer,0,BUF_SIZE); 
	// aqui o servidor vai identificar a aplicação que fez a ligação
  if (h==0){
    health_app(client_fd); //profisional de saude
  }
  if (s==0){
    security_app(client_fd); //agente de segurança 
  }
  if (a==0){
    admin_app(client_fd); //administrador do sistema
  }
	fflush(stdout);
	close(client_fd);
    
}

void health_app(int client_fd){
  memset(buffer, 0, BUF_SIZE);
  nread = read(client_fd, buffer, BUF_SIZE-1);	
	buffer[nread] = '\0';
  sU=strcmp(buffer,"UP");
  if(sU==0){
    HsignUP(client_fd);
  }
  text = fopen ("RHealth.txt","r");//tenta abrir ficheiro
  char *userinfo=NULL;
  size_t len=0;
  char user[BUF_SIZE];
  int check=1;
  char msg[BUF_SIZE];
	if(text == NULL) {//verifica se o ficheiro foi aberto
    erro("Abertura do ficheiro"); 
  }
  memset(user,0,BUF_SIZE); 
  strcat(user,buffer);
  memset(buffer,0,BUF_SIZE); 
  while ((getline(&userinfo, &len, text))!= -1){
    if(strstr(userinfo,user)){
      break;
    }
    userinfo=NULL;
  }
  userinfo[strcspn(userinfo, "\n")] = 0; //confirma login
  check=strcmp(userinfo,user);     
  if(check!=0){
    memset(msg,0,strlen(msg));
    strcat(msg,"0");
  }
  else{
    memset(msg,0,strlen(msg));
    strcat(msg,"1");  
  }
  write(client_fd, msg, strlen(msg));
  fclose(text);
  optionH(client_fd,user);
}

void HsignUP(int client_fd){
  FILE *fp;
  memset(buffer,0,BUF_SIZE);
  nread = read(client_fd, buffer, BUF_SIZE-1);
  buffer[nread] = '\0';
  fp = fopen("Health_sign_up.txt", "a");
  if(fp == NULL)
	erro("Abertura do ficheiro");
  char newuser[50];
  strcpy(newuser, buffer);
  fprintf(fp, "%s\n",newuser);
  fclose(fp);
  exit(0);
}

void optionH(int client_fd, char data[40]){
  int denuncia=1,alarme=1,edit=1;
  memset(buffer,0,BUF_SIZE); 
  nread = read(client_fd, buffer, BUF_SIZE-1);
  denuncia=strcmp(buffer,"denuncia");
  alarme=strcmp(buffer,"alarme");
  edit=strcmp(buffer,"edit");
  if (denuncia==0){
    den_reg(client_fd); //profisional de saude
  }
  if (alarme==0){
    BTNalarm(client_fd); //agente de segurança 
  }
  if (edit==0){
    editarH(client_fd,data); //administrador do sistema
  }
}

void den_reg(int client_fd){
  memset(buffer,0,BUF_SIZE);
  text = fopen ("RegistoDen.txt","a+");
  if(text == NULL) {//verifica se o ficheiro foi aberto
    erro("Abertura do ficheiro");
  }
  nread = read(client_fd, buffer, BUF_SIZE-1);	//recebe denuncia
	buffer[nread] = '\0';
  fprintf(text,"%s\n", buffer);
  memset(buffer,0,BUF_SIZE);
  fclose(text);
  return;
}

void BTNalarm(int client_fd){
  printf("Em construção");
}

void editarH(int client_fd, char data[40]){
  FILE *text1, *textaux1;
  char *userinfo;
  int comp=-1;
  char newuser[BUF_SIZE] = "";
  size_t len = 0;
  size_t len2 = 0;
  memset(buffer,0,strlen(buffer));
  nread = read(client_fd, buffer, BUF_SIZE-1);
	buffer[nread] = '\0';
  comp=strcmp(buffer,"delete");
  if(comp==0){
    deleteH(client_fd, data);
  }
  strcat(newuser, buffer);
  text1 = fopen("RHealth.txt", "r");
  if(text1 == NULL){
    erro("Abertura de ficheiro");
  }
  textaux1 = fopen("aux.txt", "w+");
  if(textaux1 == NULL){
    erro("Abertura de ficheiro");
  }
  while((getline(&userinfo, &len, text1)) != -1){
    if(strstr(userinfo, data)){
      //strcat(data[0], "");
      fprintf(textaux1, "%s\n", newuser);
    }
    else{
      fprintf(textaux1, "%s", userinfo);
    }
    userinfo = NULL;
  }
  fclose(text1);
  fclose(textaux1);
  textaux1 = fopen("aux.txt", "r");
  if(textaux1 == NULL){
    erro("Abertura de ficheiro");
  }
  text1 = fopen("RHealth.txt", "w+");
  if(text1 == NULL){
    erro("Abertura de ficheiro");
  }
  while ((getline(&userinfo, &len, textaux1)) != -1){
    fprintf(text1, "%s", userinfo);
    userinfo = NULL;
  }
  fclose(textaux1);
  fclose(text1);
  textaux1 = fopen("aux.txt", "w+");
  if(textaux1 == NULL){
    erro("Abertura de ficheiro");
  }
  fclose(textaux1);
  memset(buffer,0,strlen(buffer));
  strcat(buffer, "\nDados alterados com sucesso! As suas alterações entrarão em efeito no próximo login.\n");
  write(client_fd, buffer, BUF_SIZE-1);
  optionS(client_fd,data);
}

void deleteH(int client_fd, char data[40]){
  FILE *text1, *textaux1;
  char *userinfo;
  size_t len = 0;
  size_t len2 = 0;
  text1 = fopen("RHealth.txt", "r");
  if(text1 == NULL){
    erro("Abertura de ficheiro");
  }
  textaux1 = fopen("aux.txt", "w+");
  if(textaux1 == NULL){
    erro("Abertura de ficheiro");
  }
  while((getline(&userinfo, &len, text1)) != -1){
    if(strstr(userinfo, data)){
      continue;
    }
    else{
      fprintf(textaux1, "%s", userinfo);
    }
    userinfo = NULL;
  }
  fclose(text1);
  fclose(textaux1);
  textaux1 = fopen("aux.txt", "r");
  if(textaux1 == NULL){
    erro("Abertura de ficheiro");
  }
  text1 = fopen("RHealth.txt", "w+");
  if(text1 == NULL){
    erro("Abertura de ficheiro");
  }
  while ((getline(&userinfo, &len, textaux1)) != -1){
    fprintf(text1, "%s", userinfo);
    userinfo = NULL;
  }
  fclose(textaux1);
  fclose(text1);
  textaux1 = fopen("aux.txt", "w+");
  if(textaux1 == NULL){
    erro("Abertura de ficheiro");
  }
  fclose(textaux1);
  memset(buffer,0,strlen(buffer));
  strcat(buffer, "\nConta apagada com sucesso!");
  write(client_fd, buffer, BUF_SIZE-1);
}

void security_app(int client_fd){
  memset(buffer, 0, BUF_SIZE);
  nread = read(client_fd, buffer, BUF_SIZE-1);	
	buffer[nread] = '\0';
  sU=strcmp(buffer,"UP");
  if(sU==0){
    SsignUP(client_fd);
  }
  text = fopen ("RSecurity.txt","r"); //tenta abrir o ficheiro
  char *userinfo=NULL;
  size_t len=0;
  char user[BUF_SIZE];
  int check=1;
  char msg[BUF_SIZE];
	if(text == NULL) {//verifica se o ficheiro foi aberto
    erro("Abertura do ficheiro"); 
  }
  memset(user,0,BUF_SIZE); 
  strcat(user,buffer);
  memset(buffer,0,BUF_SIZE); 
  while ((getline(&userinfo, &len, text))!= -1){
    if(strstr(userinfo,user)){
      break;
    }
    userinfo=NULL;
  }
  userinfo[strcspn(userinfo, "\n")] = 0; //confirma login
  check=strcmp(userinfo,user);     
  if(check!=0){
    memset(msg,0,strlen(msg));
    strcat(msg,"0");
  }
  else{
    memset(msg,0,strlen(msg));
    strcat(msg,"1");  
  }
  write(client_fd,msg,sizeof(msg));
  fclose(text);
  memset(buffer,0,BUF_SIZE);
  optionS(client_fd,user);
}

void SsignUP(int client_fd){
  FILE *fp;
  memset(buffer, 0, BUF_SIZE);
  nread = read(client_fd, buffer, BUF_SIZE-1);
  buffer[nread] = '\0';
  fp = fopen("Security_sign_up.txt", "a");
  if(fp == NULL)
	erro("Abertura do ficheiro");
  char newuser[50];
  strcpy(newuser, buffer);
  fprintf(fp,"%s\n", newuser);
  fclose(fp);
  exit(0);
}

void optionS(int client_fd, char data[40]){
 int show=1,edit=1;
  memset(buffer,0,BUF_SIZE); 
  nread = read(client_fd, buffer, BUF_SIZE-1);
  buffer[nread] = '\0';
  printf("%s\n", buffer);
  show=strcmp(buffer,"show");
  edit=strcmp(buffer,"edit");
  memset(buffer,0,BUF_SIZE);
  if (edit!=0){
    den_vis(client_fd,data);
  }
  if (edit==0){
    editarS(client_fd,data); 
  }

} 

void den_vis(int client_fd, char data[40]){
  char *textdata;
  char *filter;
  char *den;
  int f;
  size_t len;
  memset(buffer,0,BUF_SIZE);
  printf("\n");
  nread = read(client_fd, buffer, BUF_SIZE-1);
  buffer[nread] = '\0';
  memset(filter, 0,sizeof(filter));
  memset(buffer,0,BUF_SIZE);
  nread = read(client_fd, buffer, BUF_SIZE-1);
  buffer[nread] = '\0';
  strcat(filter, buffer);
  f=strcmp(buffer,"all");  
  printf("\n");
  
  printf("\n");
  if(f!=0){
    text = fopen ("RegistoDen.txt","r");
    textaux=fopen("temp.txt", "w");
    if(text == NULL) {//verifica se o ficheiro foi aberto
      erro("Abertura do ficheiro"); 
    }
    if(textaux == NULL) {//verifica se o ficheiro foi aberto
      erro("Abertura do ficheiro"); 
    }
    printf("\n");
    while((getline(&den, &len, text))!= -1){
      if(strstr(den,filter)){
        fprintf(textaux, "%s", den);			
      }
    }
    fclose(textaux);
    fclose(text);
    redirect = fopen("temp.txt", "r");
  }
  if(f==0){
    redirect= fopen ("RegistoDen.txt","r");
  }
  fseek(redirect,0,SEEK_END);  //procura o fim do ficheiro
  long int fsize = ftell(redirect); //tamanho do ficheiro
	fseek(redirect,0,SEEK_SET); //move para o início do ficheiro
	textdata = malloc (fsize);  //guarda memoria do tamanho do ficheiro
	fread(textdata,1,fsize,redirect);
  printf("\n");
  write(client_fd, textdata, 1 + strlen(textdata));
  fclose(redirect);
  optionS(client_fd,data);
}

void editarS(int client_fd, char data[40]){
  FILE *text1, *textaux1;
  char *userinfo;
  int comp=-1;
  char newuser[BUF_SIZE] = "";
  size_t len = 0;
  size_t len2 = 0;
  memset(buffer,0,strlen(buffer));
  nread = read(client_fd, buffer, BUF_SIZE-1);
	buffer[nread] = '\0';
  comp=strcmp(buffer,"delete");
  if(comp==0){
    deleteH(client_fd, data);
  }
  strcat(newuser, buffer);
  text1 = fopen("RSecurity.txt", "r");
  if(text1 == NULL){
    erro("Abertura de ficheiro");
  }
  textaux1 = fopen("aux.txt", "w+");
  if(textaux1 == NULL){
    erro("Abertura de ficheiro");
  }
  while((getline(&userinfo, &len, text1)) != -1){
    if(strstr(userinfo, data)){
      fprintf(textaux1, "%s\n", newuser);
    }
    else{
      fprintf(textaux1, "%s", userinfo);
    }
    userinfo = NULL;
  }
  fclose(text1);
  fclose(textaux1);
  textaux1 = fopen("aux.txt", "r");
  if(textaux1 == NULL){
    erro("Abertura de ficheiro");
  }
  text1 = fopen("RSecurity.txt", "w+");
  if(text1 == NULL){
    erro("Abertura de ficheiro");
  }
  while ((getline(&userinfo, &len, textaux1)) != -1){
    fprintf(text1, "%s", userinfo);
    userinfo = NULL;
  }
  fclose(textaux1);
  fclose(text1);
  textaux1 = fopen("aux.txt", "w+");
  if(textaux1 == NULL){
    erro("Abertura de ficheiro");
  }
  fclose(textaux1);
  memset(buffer,0,strlen(buffer));
  strcat(buffer, "\nDados alterados com sucesso! As suas alterações entrarão em efeito no próximo login.\n");
  write(client_fd, buffer, BUF_SIZE-1);
  optionS(client_fd,data);
}

void deleteS(int client_fd, char data[40]){
  FILE *text1, *textaux1;
  char *userinfo;
  size_t len = 0;
  size_t len2 = 0;
  text1 = fopen("RSecurity.txt", "r");
  if(text1 == NULL){
    erro("Abertura de ficheiro");
  }
  textaux1 = fopen("aux.txt", "w+");
  if(textaux1 == NULL){
    erro("Abertura de ficheiro");
  }
  while((getline(&userinfo, &len, text1)) != -1){
    if(strstr(userinfo, data)){
      continue;
    }
    else{
      fprintf(textaux1, "%s", userinfo);
    }
    userinfo = NULL;
  }
  fclose(text1);
  fclose(textaux1);
  textaux1 = fopen("aux.txt", "r");
  if(textaux1 == NULL){
    erro("Abertura de ficheiro");
  }
  text1 = fopen("RSecurity.txt", "w+");
  if(text1 == NULL){
    erro("Abertura de ficheiro");
  }
  while ((getline(&userinfo, &len, textaux1)) != -1){
    fprintf(text1, "%s", userinfo);
    userinfo = NULL;
  }
  fclose(textaux1);
  fclose(text1);
  textaux1 = fopen("aux.txt", "w+");
  if(textaux1 == NULL){
    erro("Abertura de ficheiro");
  }
  fclose(textaux1);
  memset(buffer,0,strlen(buffer));
  strcat(buffer, "\nConta apagada com sucesso!");
  write(client_fd, buffer, BUF_SIZE-1);
}

void admin_app(int client_fd){
  memset(buffer,0,BUF_SIZE);
  text = fopen ("RAdmin.txt","r"); //tenta abrir ficheiro
  char *userinfo=NULL;
  size_t len=0;
  char user[BUF_SIZE];
  int check=1;
  char msg[BUF_SIZE];
	if(text == NULL) {//verifica se o ficheiro foi aberto
    erro("Abertura do ficheiro"); 
  }
  memset(user,0,BUF_SIZE); 
  nread = read(client_fd, buffer, BUF_SIZE-1);	//recebe login
	buffer[nread] = '\0';
  strcat(user,buffer);
  memset(buffer,0,BUF_SIZE); 
  while ((getline(&userinfo, &len, text))!= -1){
    if(strstr(userinfo,user)){
      break;
    }
    userinfo=NULL;
  }
  userinfo[strcspn(userinfo, "\n")] = 0; //confirma login
  check=strcmp(userinfo,user);     
  if(check!=0){
    memset(msg,0,strlen(msg));
    strcat(msg,"0");
  }
  else{
    memset(msg,0,strlen(msg));
    strcat(msg,"1");  
  } 
  write(client_fd, msg, strlen(msg));
  fclose(text);
  optionA(client_fd,user);
}

void optionA(int client_fd, char data[40]){
  int accheck=1,edit=1;
  memset(buffer,0,BUF_SIZE); 
  nread = read(client_fd, buffer, BUF_SIZE-1);
  accheck=strcmp(buffer,"check");
  edit=strcmp(buffer,"edit");
  if (accheck==0){
    accountcheck(client_fd); //profisional de saude
  }
  if (edit==0){
    editarS(client_fd, data); //administrador do sistema
  }
} 

void accountcheck(int client_fd){
  char contas[BUF_SIZE] = "";
  memset(buffer,0,BUF_SIZE);
  int opt;
  nread = read(client_fd, buffer, BUF_SIZE-1);
  buffer[nread] = '\0';
  opt = strcmp(buffer, "1");
  memset(buffer,0,BUF_SIZE);
  int o = -1;
  if(opt == 0){
    text = fopen("Health_sign_up.txt", "r");
    textaux = fopen("RHealth.txt", "a");
    char *newusers = NULL;
    size_t len = 0;
    while((getline(&newusers, &len, text)) != -1){
      strcat(buffer, newusers); 
    }
    strcat(contas, buffer);
    write(client_fd, buffer, BUF_SIZE-1);
    memset(buffer,0,BUF_SIZE);
    nread = read(client_fd, buffer, BUF_SIZE-1);
    buffer[nread] = '\0';
    o = strcmp(buffer, "1");
    if(o == 0){
      fprintf(textaux, "%s", contas);
    }
    memset(buffer, 0, strlen(buffer));
    fclose(text);
    fclose(textaux);
    text = fopen("Health_sign_up.txt", "w");
    fclose(text);
    memset(buffer,0,BUF_SIZE);
  }
  if(opt != 0){
    text = fopen("Security_sign_up.txt", "r");
    textaux = fopen("RSecurity.txt", "a");
    char *newusers = NULL;
    size_t len = 0;
    while((getline(&newusers, &len, text)) != -1){
      strcat(buffer, newusers); 
    }
    strcat(contas, buffer);
    write(client_fd, buffer, BUF_SIZE-1);
    memset(buffer,0,BUF_SIZE);
    nread = read(client_fd, buffer, BUF_SIZE-1);
    buffer[nread] = '\0';
    o = strcmp(buffer, "1");
    if(o == 0){
      fprintf(textaux, "%s", contas);
    }
    memset(buffer, 0, strlen(buffer));
    fclose(text);
    fclose(textaux);
    text = fopen("Security_sign_up.txt", "w");
    fclose(text);
    memset(buffer,0,BUF_SIZE);
  }
  return;
}

void editarA(int client_fd, char data[40]){
  FILE *text1, *textaux1;
  char *userinfo;
  char newuser[BUF_SIZE] = "";
  size_t len = 0;
  size_t len2 = 0;
  memset(buffer,0,strlen(buffer));
  nread = read(client_fd, buffer, BUF_SIZE-1);
	buffer[nread] = '\0';
  strcat(newuser, buffer);
  text1 = fopen("RAdmin.txt", "r");
  if(text1 == NULL){
    erro("Abertura de ficheiro");
  }
  textaux1 = fopen("aux.txt", "w+");
  if(textaux1 == NULL){
    erro("Abertura de ficheiro");
  }
  while((getline(&userinfo, &len, text1)) != -1){
    if(strstr(userinfo, data)){
      //strcat(data[0], "");
      fprintf(textaux1, "%s\n", newuser);
    }
    else{
      fprintf(textaux1, "%s", userinfo);
    }
    userinfo = NULL;
  }
  fclose(text1);
  fclose(textaux1);
  textaux1 = fopen("aux.txt", "r");
  if(textaux1 == NULL){
    erro("Abertura de ficheiro");
  }
  text1 = fopen("RAdmin.txt", "w+");
  if(text1 == NULL){
    erro("Abertura de ficheiro");
  }
  while ((getline(&userinfo, &len, textaux1)) != -1){
    fprintf(text1, "%s", userinfo);
    userinfo = NULL;
  }
  fclose(textaux1);
  fclose(text1);
  textaux1 = fopen("aux.txt", "w+");
  if(textaux1 == NULL){
    erro("Abertura de ficheiro");
  }
  fclose(textaux1);
  memset(buffer,0,strlen(buffer));
  strcat(buffer, "\nDados alterados com sucesso! As suas alterações entrarão em efeito no próximo login.\n");
  write(client_fd, buffer, BUF_SIZE-1);
  optionA(client_fd,data);
}

void erro(char *msg){
	printf("Erro: %s\n", msg);
	exit(-1);
}
