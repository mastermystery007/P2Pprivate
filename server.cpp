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

class UserInfo
{
public:
	string  username;
	string  password;
	string ip;
	string port;
};

class fileinfo
{
public:
	string sha1;
	int size;
    string location;
};


unordered_map<string, string> user_password;
void *readAndwrite(void* new_socket_passed);
vector<string> extecuteCommands(vector<string> commands );



vector<string> convertStringToCommands(string bufferString)
{
    vector<string> commands;
    
    string tempcommand = "";
    
       for (int i= 0; i<bufferString.size();i++)
       {
           
           if(isspace(bufferString[i]))
           {
               commands.push_back(tempcommand);
               tempcommand="";
           }
           else
           {
               tempcommand+=bufferString[i];
           }
       }
       commands.push_back(tempcommand);
       


       
       return commands;
       
}
vector<string> extecuteCommands(vector<string> commands )
{   
        vector<string> responseV ; 
    

       for(int i=0;i<commands.size();i++)
       {
           for(int j=0;j<commands[i].size();j++)
           {
               if(isspace(commands[i][j])){cout<<"j = "<<j;}
           }
           cout<<"commands are ";
           cout<<commands[i]<<"\n";
       }
    
    
    char writebuffer[1024] = {0};
    int bytes_written;
    
    if(commands[0]=="create_user")
    {
       
            
            string response_string = "created user";
            responseV.push_back(response_string);
            cout<<"responseV is "<<responseV[0];
            return responseV ;
            
        //send the message to client
          
            
        

    }

    if(commands[0]=="upload_file")
    {

    }
    if(commands[0]=="download_file")
    {
        
        ifstream  inFile("text.txt");
        
        /*if(!fd)
        {
            perror("Error in opening file");
        }  */
        string lineFromFile;

        while( !inFile.eof() )
        {       
        getline( inFile, lineFromFile );
        cout<<lineFromFile<<endl;
    //process the line
        }
        string response_string = "created user";
        responseV.push_back(response_string);
        cout<<"responseV is "<<responseV[0];
        return responseV ;

    }

    if(commands[0]=="login"){;}
    if(commands[0]=="create_group"){;}
    if(commands[0]=="join_group"){;}
    if(commands[0]=="requests"){;}
    if(commands[0]=="accept_request"){;}
    
    
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
    vector<string> responses;
    
    
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
        cout << "Client: " << readbuffer << endl;
        cout << ">";

       string bufferString(readbuffer);
       commands = convertStringToCommands(bufferString);
       responses = extecuteCommands(commands);
       
       string response_string ="";
       for(int k=0;k<responses.size();k++)
       {
           cout<<"response is "<<endl;
           response_string+=responses[k];
           cout<<responses[k];
       }
        
        
       
        memset(&writebuffer, 0, sizeof(writebuffer)); //clear the buffer
        strcpy(writebuffer, response_string.c_str());
        if(response_string == "exit")
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