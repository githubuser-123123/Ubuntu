// server.c 
// >> #include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<netinet/in.h>

#define PORT 8080

int main(){
    int sevrver_fd;
    sevrver_fd=socket(AF_INET,SOCK_STREAM,0);
    if (sevrver_fd < 0){
       perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in server_address;
    server_address.sin_family=AF_INET;
    server_address.sin_port=htons(PORT);
    server_address.sin_addr.s_addr=INADDR_ANY;

     int addresslen=sizeof(server_address);

    if (bind(sevrver_fd,(struct sockaddr*)&server_address,sizeof(server_address)) < 0){
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }
    
    if (listen(sevrver_fd,3)<0)
    {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }
    
    while (1)
    {
        int client_socket;
        struct sockaddr_in client_address;
        socklen_t client_address_size= sizeof(client_address);
        client_socket=accept(sevrver_fd,(struct sockaddr*)&client_address,&client_address_size);
        if (client_socket< 0){
           perror("Accept failed");
           exit(EXIT_FAILURE);
        }
        
        printf("Welcome to Hamming code Server\n");
        int data[7];
        
        printf("\n\nReceived Data :>>>\t");
        recv(client_socket,data,sizeof(data),0);

        for (int i = 0; i < 7; i++) 
        printf("%d\t", data[i]);
        printf("\n\n");
        int p,p1,p2,p4;
        // check parity
         p1=data[6]^data[4]^data[2]^data[0];
         p2=data[5]^data[4]^data[1]^data[0];
         p4=data[3]^data[0]^data[2]^data[1];

        //position of errror 
         p = (4 * p4) + (2 * p2 + p1) + p1;

        if (p==0){
            printf("No error\n\n");
        }
        else{
            printf("\nError at positon :>> %d\n",p);
            // correct it 
            printf("\nThe corrected data is :>>\t");
            if (data[7- p]==0){
            data[7-p]=1;
            }
            else{
            data[7-p]=0;
            }
            
            for (int i = 0; i < 7; i++){
                printf("%d\t",data[i]);
            }
        
        }

    }
    
    // close(client_socket);
    close(sevrver_fd);
    return 0;
}



















//client
// Client.c >>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<unistd.h>
#include<arpa/inet.h>

#define PORT 8080

int data[7];

void error(){
    int e;
    printf("\nat what position do you want to make error ? :>> ");
    scanf("%d",&e);
    
    if (data[e]==0){
       data[e]=1;
    }
    else{
        data[e]=0;
    }   
   printf("data before correction : >>\t");
     for (int i = 0; i < 7; i++){
            printf("%d\t",data[i]);
        }
        
}

int main(){

    int client_fd;
    struct sockaddr_in serv_addr;

    if((client_fd=socket(AF_INET,SOCK_STREAM,0)) < 0){
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    serv_addr.sin_family=AF_INET;
    serv_addr.sin_port=htons(PORT);
    serv_addr.sin_addr.s_addr = INADDR_ANY;

   if (connect(client_fd,(struct sockaddr*)&serv_addr,sizeof(serv_addr))){
        perror("Socket failed");
        exit(EXIT_FAILURE);
   }
   
    printf("Welcome to Hamming ki duniya :>>>>\n\n");
    printf("Enter 4 bit data to be Send : \n");
    
    scanf("%d",&data[0]);
        scanf("%d",&data[1]);
            scanf("%d",&data[2]);
                scanf("%d",&data[4]);

    //Calculate Hamming code even parity bits
    data[6]= data[2]^data[4]^data[0];
        data[5]= data[0]^data[4]^data[1];
            data[3]= data[2]^data[1]^data[0];

    printf("Parity bits Are >> P1=%d\tP2=%d\tP4=%d\t\n\n",data[6],data[5],data[3]);
    printf("Hamming code to be send >>\t");
     for (int i = 0; i < 7; i++){
            printf("%d\t",data[i]);
        }
         
   
   int choice;
   while (1)
   {
      printf("\n1: send without error\n2: send with error \nEnter Choice :>> ");
      scanf("%d",&choice);

      switch (choice)
      {
      case 1:
        
        send(client_fd,data,sizeof(data),0);
        break;
      case 2:
        error();
        send(client_fd,data,sizeof(data),0);
        break;   
      
      default:
      printf("\ninvalid input !\n");
        break;
      }
   }
   
    return 0;
}