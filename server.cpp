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
#define CHUNKSIZE 512

using namespace std;

class Trackeruserinfo
{
public:
	string  username;
	string  password;
	string ip;
	string port;
};

class Trackerfileinfo
{
public:
	vector<string> sha1;
	int size;
    int chunks;
    string name;
};


//unordered_map<UserInfo, fileinfo> user_password;//check this
void *readAndwrite(void* new_socket_passed);
vector<string> extecuteCommands(vector<string> commands,int sockfd);



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



vector<string> extecuteCommands(vector<string> commands,int sockfd)
{   
    vector<string> responseV ; 
        
        
    
    
    char writebuffer[1024] = {0};
    int bytes_written;
    
    if(commands[0]=="create_user")
    {
        string response_string = "created user";
        responseV.push_back(response_string);
        cout<<"responseV is "<<responseV[0];
        return responseV ; 
    }

    if(commands[0]=="upload_file")
    {
        //receive the chunks
        int finfochunks;
        string finfoname = commands[1];
        char data[100];
        bzero(data,100);

        Trackerfileinfo * finfo = new Trackerfileinfo();
        
        string ack1 = "okkkkkkkkkkkkkk";
        strcpy(data, ack1.c_str());
        cout<<"CP6"<<endl;
        
        send(sockfd, (char*)&data, strlen(data), 0);
        string sendstr(data);
        cout<<"sending.."<<sendstr<<endl;       
       // finfo->chunks=finfochunks;
        //finfo->name = finfoname;
        //vector<string> tempsha(finfochunks);
        memset(&data, 0, sizeof(data));//clear the buffer
        cout<<"CP7"<<endl;
        recv(sockfd, (char *)&data, sizeof(data), 0);
        cout<<data;


         string response_string = "File uploaded";
        responseV.push_back(response_string);
        return responseV ;
       /* for(int i=0;i<finfochunks;i++)
        {
         //   tempsha.push_back(SHA1());
         recv(sockfd, (char *)&data, sizeof(data), 0);
         string shaval(data);
         tempsha.push_back(shaval);
        }*/
    }
    if(commands[0]=="download_file")
    {
        string filename=commands[1];
        char data[10];
        bzero(data,10);
     
        
        
        
        //GETTING SIZE OF FILE 
        FILE *p_file = NULL;
        p_file = fopen(filename.c_str(),"rb");
        if(p_file == NULL)
        {
        printf("ERROR: File  not found.\n");
        exit(1);
        }
        
        fseek(p_file,0,SEEK_END);
        int size = ftell(p_file);
        fclose(p_file);
        cout<<"The size of the file is "<<size<<endl;
        
        
        
        //SENDING FILE AS A STREAM
        ifstream  inFile(filename);
        while (inFile.read (data, 10)) 
        {
        send(sockfd, (char*)&data, strlen(data), 0);
        cout<<data;
        } 
        send(sockfd, (char*)&data, strlen(data), 0);
        cout<<data;
        inFile.close();
        
        string response_string = "File sent";
        responseV.push_back(response_string);
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
        cout<<"CP5"<<endl;
        bytes_read += recv(new_socket, (char*)&readbuffer, sizeof(readbuffer), 0);
        
        if(!strcmp(readbuffer, "exit")){cout << "Client has quit the session " << endl;break; }
        
       
        

       string bufferString(readbuffer);
       cout<<"received :"<<bufferString<<endl;
       commands = convertStringToCommands(bufferString);
       responses = extecuteCommands(commands,new_socket);
       
       string response_string ="";
       for(int k=0;k<responses.size();k++)
        {
            response_string+=responses[k];
        }
        
        
       
        memset(&writebuffer, 0, sizeof(writebuffer)); //clear the buffer
        strcpy(writebuffer, response_string.c_str());

        if(response_string == "exit"){printf("Sending data to %d \n",new_socket);send(new_socket, (char*)&writebuffer, strlen(writebuffer), 0);break;}
        
        printf("Sending data to %d \n",new_socket);
        cout<<"CP9"<<endl;
        bytes_written += send(new_socket, (char*)&writebuffer, strlen(writebuffer), 0);
        string send2str(writebuffer);
        cout<<send2str<<"\n";

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