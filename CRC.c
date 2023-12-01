// server
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 8000
#define N strlen(crc_key)

int i, j;
int count = 0;
char data[20];
char crc_key[20];
char checksum[20];

void XOR()
{
    for(j=1; j<N; j++)
        checksum[j] = ((checksum[j] == crc_key[j]) ? '0' : '1');
}

int main()
{
    int serversocket, clientsocket, bindstatus;
    char servermessage[256];
    struct sockaddr_in serveraddress, clientaddress;
    int client_address_len = sizeof(clientaddress);

    serversocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serversocket < 0)
    {
        printf("socket failed\n");
        return -1;
    }

    serveraddress.sin_family = AF_INET;
    serveraddress.sin_port = htons(PORT);
    serveraddress.sin_addr.s_addr = INADDR_ANY;

    bindstatus = bind(serversocket, (const struct sockaddr *)&serveraddress, sizeof(serveraddress));
    if (bindstatus < 0)
    {
        printf("binding failed\n");
        return -1;
    }
    else
    {
        printf("binding is successful\n");
    }

    listen(serversocket, 3);
    printf("Waiting for client connection...\n");

    clientsocket = accept(serversocket, (struct sockaddr *)&clientaddress, (socklen_t *)&client_address_len);
    if (clientsocket < 0)
    {
        printf("connection is rejected by server\n");
        return -1;
    }
    else
    {
        printf("connection is accepted\n");
    }


    recv(clientsocket, data, sizeof(data), 0);
    recv(clientsocket, crc_key, sizeof(crc_key), 0);
    recv(clientsocket, checksum, sizeof(checksum), 0);


    printf("Received data is: %s\n", data);

    int data_length = strlen(data);

    for(i=0; i<N; i++)
        checksum[i] = data[i];

    do{
        if(checksum[0] == '1')
            XOR();
        for(j=0; j<N-1; j++)
            checksum[j] = checksum[j+1];
        checksum[j] = data[i++];
    }while(i <= data_length+N-1);

    for(i = 0; i<N; i++)
    {
        if(checksum[i] == '1')
            count++;
        else
            count = 0;
    }

    if(count < 0)
        printf("error detected\n");
    else
        printf("no error detected\n");

    for(i = data_length; i >= data_length-N+1; i--)
    {
        data[i] = '\0';
    }

    printf("The original data is: %s\n", data);

    close(serversocket);
    return 0;
}

























//client
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 8000
#define N strlen(crc_key)

void XOR();
int i, j;
char data[20];
char crc_key[20];
char checksum[20];

int main()
{
    struct sockaddr_in serveraddress;

    int clientsocket;
    clientsocket = socket(AF_INET, SOCK_STREAM, 0);
    if(clientsocket<0)
    {
        printf("socket creation failed\n");
        return -1;
    }

    serveraddress.sin_family = AF_INET;
    serveraddress.sin_port = htons(PORT);
    serveraddress.sin_addr.s_addr = INADDR_ANY;

    int connection_status;
    connection_status = connect(clientsocket, (struct sockaddr *)&serveraddress, sizeof(serveraddress));
    if(connection_status < 0)
    {
        printf("connection failed\n");
        return -1;
    }
    else
        printf("connection established\n");


    printf("Enter the data to be transmitted: ");
    scanf("%s", data);

    printf("Enter the CRC key: ");
    scanf("%s", crc_key);

    int data_length = strlen(data);

    for(i = data_length; i<data_length+N-1; i++)
        data[i] = '0';
    printf("new data = %s\n", data);

    for(i = 0; i<N; i++)
        checksum[i] = data[i];
    
    do
    {
        if(checksum[0] == '1')
            XOR();
        for(j = 0; j<N-1; j++)
            checksum[j] = checksum[j+1];
        checksum[j] = data[i++];
    } while (i <= data_length+N-1);

    printf("The check value is: %s\n", checksum);
    
    for(i=data_length; i<data_length+N-1; i++)
        data[i] = checksum[i-data_length];
    
    printf("The final data to be sent is: %s\n", data);

    send(clientsocket, data, sizeof(data), 0);
    send(clientsocket, crc_key, sizeof(crc_key), 0);
    send(clientsocket, checksum, sizeof(checksum), 0);

    /*char error_msg[30];
    recv(clientsocket, error_msg, sizeof(error_msg), 0);
    printf("Error status: %s\n", error_msg);*/


    close(clientsocket);

    return 0;
}

void XOR()
{
    for(j=1; j<N; j++)
        checksum[j] = ((checksum[j] == crc_key[j]) ? '0' : '1');
}
