// Client side C/C++ program to demonstrate Socket programming
#include <stdio.h>
#include <openssl/sha.h>
#include <cmath>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <string>
#include <list>
#include <fstream>
#include <cmath>
#include <vector>
#include <iostream>
#include <errno.h>
#include <unordered_map>
#include <queue>
#define SERVER_PORT 8080
using namespace std;

bool KEEP_LISTENING;
string trackerip = "127.0.0.1";
int tracker_port;
string self_ip="127.0.0.1";
string my_port;
int MY_PORT;
string self_gid;
string self_cid;


void* clientAsServer(void* arg)
{
    int server_fd;
    while(KEEP_LISTENING)
    {
        int new_socket, valread;
        struct sockaddr_in address;
        int opt = 1;
        int addrlen = sizeof(address);
        char buffer[1024] = {0};
        char hello[] = "Hello from server";
        pthread_t thread_id;
        // Creating socket file descriptor
        if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
        {
            perror("socket failed");
            exit(EXIT_FAILURE);
        }

        // Forcefully attaching socket to the port 8080
        if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,&opt, sizeof(opt)))
        {
            perror("setsockopt");
            exit(EXIT_FAILURE);
        }
        address.sin_family = AF_INET;
        address.sin_addr.s_addr = INADDR_ANY;
        address.sin_port = htons( MY_PORT );

        // Forcefully attaching socket to the port 8080
        if (bind(server_fd, (struct sockaddr *)&address,
                                    sizeof(address))<0)
        {
            perror("bind failed");
            exit(EXIT_FAILURE);
        }
        if (listen(server_fd, 3) < 0)
        {
            perror("listen");
            exit(EXIT_FAILURE);
        }

        while(1)
        {
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address,
                        (socklen_t*)&addrlen))<0)
        {
            perror("accept");
            exit(EXIT_FAILURE);
        }
        //pthread_create(&thread_id, NULL, parseInput, (void *)&new_socket);

        
        }
    }
    close(server_fd);
    return NULL;

}



int main(int argc, char const *argv[])
{
    int sock = 0, valread; 

    my_port = argv[1];
    MY_PORT = stoi(my_port);

    KEEP_LISTENING = true;
    
    pthread_t listenerThreadid;
    pthread_create(&listenerThreadid, NULL,clientAsServer,NULL);


    
    struct sockaddr_in serv_addr; 
    char *hello = "Hello from client"; 
    char buffer[1024] = {0}; 
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
    { 
        printf("\n Socket creation error \n"); 
        return -1; 
    } 
   
    serv_addr.sin_family = AF_INET; 
    serv_addr.sin_port = htons(SERVER_PORT); 
       
    // Convert IPv4 and IPv6 addresses from text to binary form 
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0)  
    { 
        printf("\nInvalid address/ Address not supported \n"); 
        return -1; 
    } 
   
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) 
    { 
        printf("\nConnection Failed \n"); 
        return -1; 
    } 
    send(sock , hello , strlen(hello) , 0 ); 
    printf("Hello message sent\n"); 
    valread = read( sock , buffer, 1024); 
    printf("%s\n",buffer ); 
    return 0; 
}