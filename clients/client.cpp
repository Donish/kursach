#include <iostream>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/uio.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/sem.h>
#include <fstream>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/msg.h>

#define SERVER_KEY_PATHNAME "/tmp/mqueue_server_key"
#define PROJECT_ID "M"

struct message_text
{
    int _qid;
    std::string _buff;
};

struct message
{
    long int _message_type;
    struct message_text _message_text;
};

void message_queues()
{

}

int main(int argc, char* argv[])
{
    if(argc != 3)
    {
        std::cerr << "Enter the server ID and the port!" << std::endl;
        exit(0);
    }



}