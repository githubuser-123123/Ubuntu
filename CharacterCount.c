//server:

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/types.h>
#include<sys/socket.h>

struct Fra
{
    int charcount;
    char frame[100];
}
Frames[100];


int main()
{
    char *ip = "127.0.0.1";
    int port = 5544;
    int i=0;
    struct sockaddr_in server_addr,client_addr;
    int server_sock,client_sock;
    char buffer[1024];
    socklen_t serveraddr_len,clientaddr_len;
    int n=0,send_status;
    server_sock = socket(AF_INET,SOCK_STREAM,0);
    if(server_sock<0)
    {
        perror("Socket Error");
        exit(0);
    }
    printf("\n**Server Socket Created Successfully! ");
    memset(&server_addr,'\0',sizeof(server_addr));
    server_addr.sin_family=AF_INET;
    server_addr.sin_port=port;
    server_addr.sin_addr.s_addr = inet_addr(ip);
    serveraddr_len = sizeof(server_addr);
    n = bind(server_sock,(struct sockaddr*)&server_addr,serveraddr_len);
    if(n<0)
    {
        perror("Binding Error");
        exit(0);
    }
    printf("\n**Socket Bind at port %d succesfully\n",port);
    listen(server_sock,5);
    printf("**Listening....\n");
    
    

    while(1)
    {
        int i=0;
        int p=0,j=0,ack=0;
        int countframe=0;
        clientaddr_len = sizeof(client_addr);
        client_sock = accept(server_sock,(struct sockaddr*)&client_addr,&clientaddr_len);
        if(client_sock<0)
        {
            perror("Accept Error!");
            exit(0);
        }
        printf("\n**Accepted Connection from Client \n**Client Connected Successfully!!\n");
        bzero(buffer,1024);
        printf("\nEnter The message You want to Sent to Client : ");
        gets(buffer);
        while(1)
        {j=0;
            while(buffer[i]!=' ' && buffer[i]!='\0')
            {
                Frames[p].frame[j]=buffer[i];
                i++,j++;
            }

            countframe++;
            printf("\n");
            puts(Frames[p].frame);
            printf(" = ");
            
            Frames[p].charcount= strlen(Frames[p].frame);
            printf("%d",Frames[p].charcount);
            if(buffer[i]=='\0')
            {
                break;
            }
            p++;
            i++;
        }

        p=0;

        send(client_sock,&countframe,sizeof(int),0);
        printf("\n%d",countframe);
        while(p<countframe)
        {
            send(client_sock,Frames[p].frame,sizeof(Frames[p].frame),0);
            send(client_sock,&Frames[p].charcount,sizeof(Frames[p].charcount),0);
            recv(client_sock,&ack,sizeof(ack),0);
            if(ack==1)
            {
                p++;
            }
        }
        
        printf("**Client Disconnected");
        close(client_sock);

    }

    return 0;


    

}














//client

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/types.h>
#include<sys/socket.h>

struct Fra
{
    int charcount;
    char frame[100];
}
Frames[100];

int main()
{
    char *ip = "127.0.0.1";
    int port = 5544;
    int countframe=0;
    int p=0,ack=1;
    struct sockaddr_in client_addr;
    int client_sock;
    char buffer[1024];
    socklen_t clientaddr_len;
    int n=0,send_status;
    client_sock = socket(AF_INET,SOCK_STREAM,0);
    if(client_sock<0)
    {
        perror("Socket Error");
        exit(0);
    }
    printf("\nServer Socket Created Successfully! ");
    memset(&client_addr,'\0',sizeof(client_addr));
    client_addr.sin_family=AF_INET;
    client_addr.sin_port=port;
    client_addr.sin_addr.s_addr = inet_addr(ip);
    clientaddr_len= sizeof(client_addr);

    n = connect(client_sock,(struct sockaddr*)&client_addr,clientaddr_len);
    if(n<0)
    {
        perror("Connection Error!");
        exit(0);
    }
    printf("\nClient Connected to Server Successfully!");
    
        recv(client_sock,&countframe,sizeof(int),0);
        printf("\n%d",countframe);
        while(countframe!=0)
        {
            recv(client_sock,Frames[p].frame,sizeof(Frames[p].frame),0);
            
            recv(client_sock,&Frames[p].charcount,sizeof(int),0);
            printf("\n Frame %d : ",p+1);
            if(Frames[p].charcount == strlen(Frames[p].frame))
            {
                puts(Frames[p].frame);
                ack=1;
                send(client_sock,&ack,sizeof(ack),0);
                countframe--;
                p++;
            }
            else
            {
                ack=0;
                send(client_sock,&ack,sizeof(ack),0);
            }
            
        }

    printf("\nDisconnecting From Server.....");
    close(client_sock);

    return 0;

}