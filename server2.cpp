// Server side C/C++ program to demonstrate Socket programming
#include <unistd.h>
#include <openssl/sha.h>
#include <stdio.h>
#include <cmath>
#include <iostream>
#include <fstream>
#include <sys/socket.h>
#include <bits/stdc++.h>
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

using namespace std;

void *readAndwrite(void* new_socket_passed);
void extecuteCommands(vector<string> commands);

map <string,string> user_password;

void convertStringToCommands(string bufferString)
{
    vector<string> commands;
    string tempcommand = "";
       for (int i= 0; i<bufferString.size();i++)
       {
           tempcommand+=bufferString[i];
           if(isspace(bufferString[i]))
           {
               commands.push_back(tempcommand);
               tempcommand="";
           }
       }
       commands.push_back(tempcommand);

       for(int i=0;i<commands.size();i++)
       {
           cout<<commands[i]<<"\n";
       }


       extecuteCommands(commands);
       commands.clear();
}
void extecuteCommands(vector<string> commands)
{   
    if(commands[0]=="create_user")
    {
        if(commands.size()==3)
        {
            string user = commands[1];
            string password = commands[2];
            user_password[user]=password;
            response.push_back("ok");
        }

    }
    if(commands[0]=="login"){;}
    if(commands[0]=="create_group"){;}
    if(commands[0]=="join_group"){;}
    if(commands[0]=="requests"){;}
    if(commands[0]=="accept_request"){;}
    if(commands[0]=="upload_file"){;}
    if(commands[0]=="download_file"){;}
   // if(commands[0]=="logout"){;}
    if(commands[0]=="show_downloads"){;}

}

void *readAndwrite(void* new_socket_passed)
{
    char readbuffer[1024] = {0};
    char writebuffer[1024] = {0};
    int bytes_read;
    int bytes_written;
    int *temp = (int *)new_socket_passed;
    int new_socket = *temp;
    int valread;
    char *ACK = "SERVER ACK ";
    vector<string> commands;
    vector<string> response;
      while(1)
    {
        //receive a message from the client (listen)
        cout << "Awaiting  response from "<<new_socket<< endl;
        memset(&readbuffer, 0, sizeof(readbuffer));//clear the buffer
        bytes_read += recv(new_socket, (char*)&readbuffer, sizeof(readbuffer), 0);
        if(!strcmp(readbuffer, "exit"))
        {
            cout << "Client has quit the session " << endl;
            break;
        }
        /*
        DO ALL STRING COMPARISIONS OVER HERE
        */
       
       string bufferString(readbuffer);
       convertStringToCommands(bufferString);
       
       
       
       
        cout << "Client: " << readbuffer << endl;
        cout << ">";
        string data;
        getline(cin, data);
        memset(&writebuffer, 0, sizeof(writebuffer)); //clear the buffer
        strcpy(writebuffer, data.c_str());
        if(data == "exit")
        {
            //send to the client that server has closed the connection
            printf("Sending data to %d \n",new_socket);
            send(new_socket, (char*)&writebuffer, strlen(writebuffer), 0);
            break;
        }
        //send the message to client
        printf("Sending data to %d \n",new_socket);
        bytes_written += send(new_socket, (char*)&writebuffer, strlen(writebuffer), 0);
    }
        printf("Exiting the loop ");
        return NULL;
}
void check(int success)
{
   if(success==0)
   {

   }
   else
   {

   }
}

int main(int argc, char const *argv[])
{
    
    int server_fd, new_socket, valread;
    char *ACK = "SERVER ACK ";
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
    
    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }else{printf("Created socket with id %d \n",server_fd);}

    // Forcefully attaching socket to the port 8080
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
                   &opt, sizeof(opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(TPORT);

    // Forcefully attaching socket to the port 8080
    if (bind(server_fd, (struct sockaddr *)&address,
             sizeof(address)) < 0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 3) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    while (1)
    {
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address,
                                 (socklen_t *)&addrlen)) < 0)
        {
            perror("accept");
            exit(EXIT_FAILURE);
        }
        else
        {
           printf("Accepting thread with id %d \n",new_socket);
           pthread_t thread_id;
           pthread_create(&thread_id, NULL, readAndwrite, (void *)&new_socket);
        }
        
        
        
    }
    return 0;
}