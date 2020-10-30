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
#define CAPACITY 4096
typedef unsigned long long int ulli;
using namespace std;

bool KEEP_LISTENING;
string trackerip = "127.0.0.1";
int tracker_port;
string self_ip="127.0.0.1";
string self_port;
int CPORT;
string self_gid;
string self_cid;
unsigned long long int chunksize = 524288;
//524288        512kb
//1024          1kb
struct add_request{
    string cid;
    string gid;
    string ip;
    string port;
};

unordered_map<string,string> filetohash;
unordered_map<string,string> hashtofile;
queue<add_request> request_queue;

unordered_map<string,vector<char>> master;
/*
     msg_type
    client - client ids 31 to 50 are reserved
    convention : i for message, i+1 for acknowledgment
    31s      add_me_to_group
    32t      ack_add_me_to_group         status
    33u      get_file_avail              gid filename    hash
    34v      ack_get_file_avail          gid filename    hash    array_of_cnos
    35w      get_file_chunk              gid filename    cno
    36x      ack_get_file_chunk          gid filename    cno     chunk_of_size_512
*/
/*
    msg_type
    client - tracker ids 1 to 30 are reserved
    convention : i for message I'll receive, i+1 for acknowledgment
    2 fields are always there.
    status
    error(if status is nonzero)
    id      name                        params
    1a       acc_creation                id  pass
    2b       ack_acc_creation            status
    3c       acc_login                   id  pass
    4d       ack_acc_login               status
    5e       group_list
    6f       ack_group_list              list_of_gids
    7g       get_group_owner             gid
    8h       ack_get_group_owner         gid  port
    9i       get_file_list               gid
    10j      ack_get_file_list           gid list_of_filenames
    11k      get_clients_for_file        gid filename
    12l      ack_get_clients_for_file    gid filename    hash    list_of_ports
    13m      upload_file                 gid filename    hash
    14n      ack_upload_file             status
*/
void createConnectionToServer(char argv[])
{
int sockfd,n;
    int sendbytes;
    struct sockaddr_in servaddr;
    char sendline[CAPACITY];    
    char recvline[CAPACITY];    

    if(sockfd=socket(AF_INET,SOCK_STREAM,0)<0){perror("Failed to create socket");}

    bzero(&servaddr,sizeof(servaddr));
    servaddr.sin_family=AF_INET;
    servaddr.sin_port = htons(SERVER_PORT);

    if((inet_pton(AF_INET,argv,&servaddr.sin_addr)<=0)){perror("Failed to convert ip to string");}

    if(connect(sockfd,(sockaddr*)&servaddr,sizeof(servaddr))<0){perror("Failed to make cinnection");}
    
    sprintf(sendline,"GET /HTTP/1.1\r\n\r\n");
    sendbytes = strlen(sendline);

    if(write(sockfd,sendline,sendbytes)!=sendbytes)
    {
        perror("Write error");
    }
    memset(recvline,0,CAPACITY);
    while((n=read(sockfd,recvline,CAPACITY-1))>0)
    {
        printf("%s",recvline);
    }
}


int main(int argc,char** argv)
{

    createConnectionToServer(argv[1]);
    return 0;
}