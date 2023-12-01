//Server

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>

int main()
{
    int port = 5500;
    char * ip="127.0.0.1";
    struct sockaddr_in server_addr,client_addr;
    int server_sock,client_sock;
    int bi;
    char buffer[1024];
    int ack;
    int frameCount;
    socklen_t serverAddr_len,clientAddr_len;
    server_sock = socket(AF_INET,SOCK_STREAM,0);
    if(server_sock<0)
    {
        perror("Socket Error!");
        exit(0);
    }
    printf("\n---> Server Socket Created Successfully!");
    memset(&server_addr,'\0',sizeof(server_addr));
    server_addr.sin_family=AF_INET;
    server_addr.sin_port=port;
    server_addr.sin_addr.s_addr=inet_addr(ip);
    serverAddr_len=sizeof(server_addr);
    bi = bind(server_sock,(struct sockaddr*)&server_addr,serverAddr_len);
    if(bi<0)
    {
        perror("Binding Error!");
    }
    printf("\n---> Socket Bind on port %d successfully",port);
    printf("\n--->Listening.....");
    listen(server_sock,5);

    while(1)
    {
        clientAddr_len=sizeof(client_addr);
        client_sock=accept(server_sock,(struct sockaddr*)&client_addr,&clientAddr_len);
        if(client_sock<0)
        {
            perror("Client Socket Error!");
        }
        printf("\n---> Client Connected!");
        printf("\n---> Enter the how many frames do you want to Transmist : ");
        scanf("%d",&frameCount);
        fflush(stdin);
        for(int i=0;i<frameCount;i++)
        {
            printf("\n---> Enter the %d frame data : ",i+1);
            scanf(" %c",&buffer[i]);
        }
        send(client_sock,&frameCount,sizeof(frameCount),0);
        ack = recv(client_sock,&ack,sizeof(ack),0);
        if(ack>=0)
        {
            int i=0;
            while(i<frameCount)
            {
                printf("\n---> Frame %d is send ",i+1);
                printf("\n---> Waiting for Ack....");
                send(client_sock,&buffer[i],sizeof(buffer[i]),0);
                ack = recv(client_sock,&ack,sizeof(ack),0);
                sleep(1); //<---TimeOut Timer 1Sec
                if(ack>=0)
                {
                printf("\n---> Ack Received for frame %d",i+1);
                i++;
                }
                else
                {
                    printf("\n---> Ack Not Received, Resending the Frame!");
                }
            }

        }

        close(client_sock);
        printf("\n---> CLient Disconnected!");
        
    }


}




















//client:
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>

int main()
{
    int port = 5500;
    char * ip="127.0.0.1";
    struct sockaddr_in client_addr;
    int client_sock;
    int bi;
    char buffer[1024];
    char data;
    int ack;
    int stat;
    int frameCount;
    socklen_t clientAddr_len;
    client_sock = socket(AF_INET,SOCK_STREAM,0);
    if(client_sock<0)
    {
        perror("Socket Error!");
        exit(0);
    }
    printf("\n---> Server Socket Created Successfully!");
    memset(&client_addr,'\0',sizeof(client_addr));
    client_addr.sin_family=AF_INET;
    client_addr.sin_port=port;
    client_addr.sin_addr.s_addr=inet_addr(ip);
    clientAddr_len=sizeof(client_addr);
    bi = connect(client_sock,(struct sockaddr*)&client_addr,clientAddr_len);
    if(bi<0)
    {
        perror("Connection Error!");
    }
    printf("\n---> You are connected to server ");
    recv(client_sock,&frameCount,sizeof(frameCount),0);
    printf("\n---> frameCount %d ",frameCount);
    ack=1;
    send(client_sock,&ack,sizeof(ack),0);
    int i=0;
    while(i<frameCount)
    {
        printf("\n---> Frame %d is recived : ",i+1);
        stat = recv(client_sock,&data,sizeof(data),0);
        if(stat>0)
        {
        printf(" %c",data);
        send(client_sock,&ack,sizeof(ack),0);
        i++;
        }
        else{
            printf("\nError in data Transmission!");
            printf("\nSender Resending the frame!");
        }
    }

    close(client_sock);
    printf("\n--->Disconnected from Server!");


    return 0;
}