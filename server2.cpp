// Server side C/C++ program to demonstrate Socket programming
#include <unistd.h>
#include <openssl/sha.h>
#include <stdio.h>
#include <cmath>
#include <iostream>
#include <fstream>
#include <sys/socket.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>  
#include <pthread.h>
#include <string>
#include <unordered_map>
#include <arpa/inet.h>
#include <list>

#define TPORT 8080
typedef unsigned long long int ulli;
using namespace std;
bool KEEP_LISTENING;

void * listenerThread(void * arg)
{
    int server_fd, new_socket, valread;
    char *hello = "Hello from server"; 
	struct sockaddr_in address;
	int opt = 1;
	int addrlen = sizeof(address);
	char buffer[1024] = {0};
	pthread_t thread_id;
	// Creating socket file descriptor
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
	{
		perror("socket failed");
		exit(EXIT_FAILURE);
	}

	// Forcefully attaching socket to the port 8080
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
												&opt, sizeof(opt)))
	{
		perror("setsockopt");
		exit(EXIT_FAILURE);
	}
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons( TPORT );

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
    valread = read( new_socket , buffer, 1024); 
    printf("%s\n",buffer ); 
    send(new_socket , hello , strlen(hello) , 0 ); 
    printf("Hello message sent\n"); 
	}
}

int main(int argc, char const *argv[])
{
    //insertDummyargs();
    KEEP_LISTENING = true;
    pthread_t listenerThreadid;
    pthread_create(&listenerThreadid, NULL, listenerThread,NULL);
    pthread_join(listenerThreadid,NULL);
    return 0;
}