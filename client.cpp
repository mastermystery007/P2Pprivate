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

using namespace std;

string trackerip = "127.0.0.1";
int tracker_port;
string self_ip = "127.0.0.1";

int MY_PORT;
int SERVER_PORT;
string self_gid;
string self_cid;
void *readAndwrite(void *new_socket_passed);

void *readAndwrite(void *new_socket_passed)
{
    char buffer[1024] = {0};
    int bytes_read;
    int bytes_received;
    int *temp = (int *)new_socket_passed;
    int new_socket = *temp;
    int valread;
    char *ACK = "SERVER ACK ";
    while (1)
    {
        //receive a message from the client (listen)
        cout << "Awaiting response from " << new_socket << endl;

        memset(&buffer, 0, sizeof(buffer)); //clear the buffer
        bytes_read += recv(new_socket, (char *)&buffer, sizeof(buffer), 0);
        if (!strcmp(buffer, "exit"))
        {
            cout << "Client has quit the session " << endl;
            break;
        }
        printf("Getting data from %d \n", new_socket);
        cout << "Client: " << buffer << endl;
        cout << ">";
        string data;
        getline(cin, data);
        memset(&buffer, 0, sizeof(buffer)); //clear the buffer
        strcpy(buffer, data.c_str());
        if (data == "exit")
        {
            //send to the client that server has closed the connection
            printf("Sending data to %d \n", new_socket);
            send(new_socket, (char *)&buffer, strlen(buffer), 0);
            break;
        }
        //send the message to client
        printf("Sending data to %d \n", new_socket);
        bytes_received += send(new_socket, (char *)&buffer, strlen(buffer), 0);
    }
    printf("Exiting the loop ");
    return NULL;
}

void *clientAsServer(void *arg)
{
    bool KEEP_LISTENING;
    KEEP_LISTENING = true;
    int server_fd;
    while (KEEP_LISTENING)
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
        else
        {
            printf("socket made .server_fd value is %d \n", server_fd);
        }

        // Forcefully attaching socket to the port 8080
        if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
        {
            perror("setsockopt");
            exit(EXIT_FAILURE);
        }
        address.sin_family = AF_INET;
        address.sin_addr.s_addr = INADDR_ANY;
        address.sin_port = htons(MY_PORT);

        // Forcefully attaching socket to the port 8080
        if (bind(server_fd, (struct sockaddr *)&address,
                 sizeof(address)) < 0)
        {
            perror("bind failed");
            exit(EXIT_FAILURE);
        }
        else
        {
            printf("Bind success on port %d \n", MY_PORT);
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
                printf("Accepting thread with new_socket val as %d \n", new_socket);
                pthread_t thread_id;
                pthread_create(&thread_id, NULL, readAndwrite, (void *)&new_socket);
            }
        }
    }
    close(server_fd);
    return NULL;
}
vector<string> breakInput(string input)
{
    vector<string> commands;

    string tempcommand = "";

    for (int i = 0; i < input.size(); i++)
    {

        if (isspace(input[i]))
        {
            commands.push_back(tempcommand);
            tempcommand = "";
        }
        else
        {
            tempcommand += input[i];
        }
    }
    commands.push_back(tempcommand);
    return commands;
}

int main(int argc, char const *argv[])
{
    int sock = 0, valread;
    bool continueL = true;
    string my_port = argv[1];
    MY_PORT = atoi(argv[1]);
    SERVER_PORT = atoi(argv[2]);
    char sending_buffer[1500];
    char receiving_buffer[1500];

    pthread_t listenerThreadid;
    pthread_create(&listenerThreadid, NULL, clientAsServer, NULL);

    struct sockaddr_in serv_addr;
    bzero((char *)&serv_addr, sizeof(serv_addr));

    char buffer[1024] = {0};
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Socket creation error \n");
        return -1;
    }
    else
    {
        printf("\n Socket successfully created with value %d \n", sock);
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(SERVER_PORT);

    // Convert IPv4 and IPv6 addresses from text to binary form
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0)
    {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("\nConnection Failed \n");
        return -1;
    }
    else
    {
        printf("\nConnection Established \n");
    }

    while (1)
    {

        string inputToSend;
        int bytesRead = 0;
        int bytesWritten = 0;
        cout << "Enter the input to send to server ";
        getline(cin, inputToSend);
        vector<string> commands;
        commands = breakInput(inputToSend);

        

        memset(&sending_buffer, 0, sizeof(sending_buffer)); //clear the buffer
        strcpy(sending_buffer, inputToSend.c_str());
        if (commands[0] == "create_user")
        {
            send(sock, (char *)&sending_buffer, strlen(sending_buffer), 0);
        }

        else if (commands[0] == "download_file")
        {
            send(sock, (char *)&sending_buffer, strlen(sending_buffer), 0);
            

            memset(&receiving_buffer, 0, sizeof(receiving_buffer));
            int n = 1;
            ofstream file;
            file.open ("copied.txt");
            while (n > 0)
            {
                n = recv(sock, (char *)&receiving_buffer, sizeof(receiving_buffer), 0);
                
                cout << "file is " << receiving_buffer << endl;
                
                file << receiving_buffer << flush; ;
                
                memset(&receiving_buffer, 0, sizeof(receiving_buffer));
            }
            
            file.close();
        }

        else if(commands[0] == "upload_file")
        {
            
            cout<<"CP1"<<endl;
            send(sock , (char*)&sending_buffer, strlen(sending_buffer), 0);
            string s1(sending_buffer);
            cout<<"Sending "<<s1<<endl;


            
            memset(&receiving_buffer,0,sizeof(receiving_buffer));
            cout<<"CP2"<<endl;
            
            
            recv(sock, (char *)&receiving_buffer, sizeof(receiving_buffer), 0);
            cout<<"CP2 rb is "<<receiving_buffer<<flush;
            cout<<"ACK 1 is "<<receiving_buffer<<flush<<"\n";

            FILE *p_file = NULL;
            string filename = commands[1];
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
            string sizestring = "The size of the file is "+to_string(size);
            
            memset(&sending_buffer, 0, sizeof(sending_buffer));
            strcpy(sending_buffer, sizestring.c_str());
            cout<<"CP3"<<endl;
            send(sock , (char*)&sending_buffer, strlen(sending_buffer), 0);



            
            string ack_string(receiving_buffer);
            if(ack_string == "ok")
            {cout<<"YIPPEEE";}
        }


        /*if(inputToSend == "download_file")
        {
            send(sock , (char*)&sending_buffer, strlen(sending_buffer), 0);
            
        }
        if(inputToSend == "upload_file")
        {
            send(sock , (char*)&sending_buffer, strlen(sending_buffer), 0);
            
        }
        if(inputToSend == "create_user")
        {
            send(sock , (char*)&sending_buffer, strlen(sending_buffer), 0);
            
        }
        if(inputToSend == "log_in")
        {
            send(sock , (char*)&sending_buffer, strlen(sending_buffer), 0);
            
        }
        
        cout << "Sending to "<<sock << endl;
        bytesWritten += send(sock, (char*)&sending_buffer, strlen(sending_buffer), 0);*/
        cout << "Awaiting response from " << sock << endl;
        memset(&receiving_buffer, 0, sizeof(receiving_buffer)); //clear the buffer
        cout<<"CP4"<<endl;
        bytesRead += recv(sock, (char *)&receiving_buffer, sizeof(receiving_buffer), 0);
        cout << "Got response from " << sock << endl;
        if (!strcmp(receiving_buffer, "exit"))
        {
            cout << "Server has quit the session" << endl;
            break;
        }
        cout << "Server: " << receiving_buffer << endl;
    }

    return 0;
}
