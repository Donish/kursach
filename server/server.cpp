#include <iostream>
#include <string>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/uio.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <strings.h>

#include "../containers/data_base.h"

#define SERVER_KEY_PATHNAME "/tmp/mqueue_server_key"
#define PROJECTD_ID 'M'

struct message_text
{
    int _qid;
//    char _buff[500];
    std::string _buff;
};

struct message
{
    long int _message_type;
    struct message_text _message_text;
};

void message_queues()
{
    key_t msq_key;
    int qid;
    struct message message;

    if((msq_key = ftok(SERVER_KEY_PATHNAME, PROJECTD_ID)) == -1) //help
    {
        perror("ftok error");
        exit(1);
    }

    if((qid = msgget(msq_key, IPC_CREAT | 0660)) == -1)
    {
        perror("msgget error");
        exit(1);
    }

    auto *db = new data_base();
    std::string command;

    //TODO: recover data

    while(true)
    {
        if(msgrcv(qid, &message, sizeof(struct message_text), 0, 0) == -1)
        {
            perror("msgrcv error!");
            exit(1);
        }
        if(message._message_text._buff == "exit")
        {
            break;
        }

        try
        {
            db->handle_request(message._message_text._buff);
            //TODO: recover
        }
        catch(std::exception &ex)
        {
            // TODO: cout
        }

        std::cout << "message received." << std::endl;

    }

}

int main(int argc, char* argv[])
{
    if(argc != 2)
    {
        std::cerr << "Enter the port!" << std::endl;
        exit(0);
    }

    int port = std::stoi(argv[1]);
    char msg[BUFSIZ];

    //setup socket and tools
    sockaddr_in server_addr;
    bzero((char*)&server_addr, sizeof(server_addr)); //help
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(port);

    int server_sd = socket(AF_INET, SOCK_STREAM, 0); //help
    if(server_sd < 0)
    {
        std::cerr << "Socket creating error!" << std::endl;
        exit(0);
    }

    int bind_status = bind(server_sd, (struct sockaddr*)&server_addr, sizeof(server_addr)); //help
    if(bind_status < 0)
    {
        std::cerr << "Socket binding error!" << std::endl;
        exit(0);
    }

    std::cout << "Waiting for a client..." << std::endl;
    listen(server_sd, 5); //help

    //new address to connect with client
    sockaddr_in new_socket_address;
    socklen_t new_socket_address_size = sizeof(new_socket_address); //help

    int new_sd = accept(server_sd, (sockaddr*)&new_socket_address, &new_socket_address_size); //help
    if(new_sd < 0)
    {
        std::cerr << "Accepting client request error!" << std::endl;
        exit(1); //help
    }
    std::cout << "Connected with client." << std::endl;

    struct timeval start1, end1;
    gettimeofday(&start1, nullptr); //help

    //марафет
    message_queues();
    //марафет

    gettimeofday(&end1, nullptr);
    close(new_sd);
    close(server_sd);
    std::cout << "Elapsed time: " << (end1.tv_sec - start1.tv_sec) << " sec." << std::endl;
    std::cout << "Connection closes..." << std::endl;

    return 0;
}