
// Server side C/C++ program to demonstrate Socket programming 
#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h> 
#define PORT 8080
#define BUFSIZE 1024 
#define PATH_MAX 200
void handle_connection(int client_socket)
{
    char buffer[BUFSIZE];
    size_t bytes_read;
    int msgsize = 0;
    char actual_path[PATH_MAX+1];

    while((bytes_read = read(client_socket,buffer+msgsize,sizeof(buffer)-msgsize-1)) >0)
    {
        msgsize=msgsize+bytes_read;
        if(msgsize>BUFSIZE-1 ||buffer[msgsize-1]=='\n')
        break;
    }

    printf("request ",buffer,"\n");
    fflush(stdout);
}
void create_connection()
{
    int server_fd, new_socket, valread; 
    struct sockaddr_in address; 
    int opt = 1; 
    int addrlen = sizeof(address); 
    char buffer[1024] = {0}; 
    char *hello = "Hello from server"; 
       
    // STREAM SOCKET  = TCP 
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) 
    { 
        perror("socket failed"); 
        exit(EXIT_FAILURE); 
    } else{printf("Socket created");}
       
    // Forcefully attaching socket to the port 8080 
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, 
                                                  &opt, sizeof(opt))) 
    { 
        perror("setsockopt"); 
        exit(EXIT_FAILURE); 
    } 
    address.sin_family = AF_INET; 
    address.sin_addr.s_addr = INADDR_ANY; 
    address.sin_port = htons( PORT ); 
       
    // Forcefully attaching socket to the port 8080 
    if (bind(server_fd, (struct sockaddr *)&address,  
                                 sizeof(address))<0) 
    { 
        perror("bind failed"); 
        exit(EXIT_FAILURE); 
    } 
    else{printf("Binding done");}
    if (listen(server_fd, 3) < 0) 
    { 
        perror("listen"); 
        exit(EXIT_FAILURE);
    }
    else
    {    
        
        while(1)
        {
            printf("Waiting for connection ");
            if ((new_socket = accept(server_fd, (struct sockaddr *)&address,(socklen_t*)&addrlen))<0) 
            { 
            perror("accept"); 
            exit(EXIT_FAILURE); 
            }
            else
            {
            //valread = read( new_socket , buffer, 1024); 
            printf("%s\n",buffer ); 
            //send(new_socket , hello , strlen(hello) , 0 );
            handle_connection(new_socket); 
            printf("Hello message sent\n");
            } 
        }
    }
}

int main(int argc, char const *argv[]) 
{ 
    create_connection();
          return 0; 
    } 
   
   
