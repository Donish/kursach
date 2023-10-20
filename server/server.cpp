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
#include <string.h>
#include <fstream>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/mman.h>

#include "../containers/data_base.h"
#include "../ipc_connections/message_queue.h"
#include "../ipc_connections/ipc_type.h"

#define SEMAPHORE_KEY 54321
#define SEMAPHORE_COUNT 1

#define FILE_MAPPING_PATHNAME "../tmp/example.txt"

#define SERVER_KEY_PATHNAME "../tmp/mqueue_server_key"
#define PROJECTD_ID 'M'

struct message_text
{
    int _qid;
    char _buff[500];
};

struct message
{
    long int _message_type;
    struct message_text _message_text;
};

void file_mapping(data_base *&db)
{
    ipc_type connection = ipc_type::FILE_MAPPING;
    int shm_fd;
    void *shm_region;

    const int file_size = 100;
    int fd = open(FILE_MAPPING_PATHNAME, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
    if(fd == -1)
    {
        perror("file error");
        exit(1);
    }

    //размер файла
    lseek(fd, file_size - 1, SEEK_SET);
    write(fd, "", 1);

    //отобрадение файла в памяти
    char *addr = (char*) mmap(NULL, file_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if(addr == MAP_FAILED)
    {
        perror("file mapping error");
        close(fd);
        exit(1);
    }
    close(fd);

    int sem_id = semget(SEMAPHORE_KEY, SEMAPHORE_COUNT, 0666);
    if(sem_id == -1)
    {
        perror("semget error");
        exit(1);
    }

    std::string message;
    std::string command;
    char msg[1500];

    while(true)
    {
        struct sembuf sem_ops[1] = {0, 1, 0};
//        semop(sem_id, sem_ops, 1);
        sem_ops[0].sem_op = -1;
        semop(sem_id, sem_ops, 1);


        command = addr;
//        sem_ops[0].sem_op = 1;
//        semop(sem_id, sem_ops, 1);
//        sem_ops[0].sem_op = -1;
//        semop(sem_id, sem_ops, 1);
        if(command == "exit")
        {
            break;
        }
        std::cout << command << std::endl;

        db->handle_request(command);
    }

    munmap(addr, file_size);
}

void message_queues(data_base *&db)
{
    ipc_type connection = ipc_type::MSG_QUEUE;
    message_queue msq;

    std::string command;

    std::string command_todo;
    while(true)
    {

        try
        {
            msq.receive_message(command);
            if(command == "exit")
            {
                break;
            }
            std::cout << command << std::endl;
        }
        catch(std::exception &ex)
        {
            perror(ex.what());
            exit(1);
        }
        db->handle_request(command);

//        std::cout << "message received." << std::endl;
    }
}

int main()
{
    key_t msg_queue_key;
    int qid;
    struct message message_rcv;
    auto *db = new data_base();
    int clients_count = 0;

    if((msg_queue_key = ftok(SERVER_KEY_PATHNAME, PROJECTD_ID)) == -1)
    {
        perror("ftok error");
        exit(1);
    }
    if((qid = msgget(msg_queue_key, IPC_CREAT | 0660)) == -1)
    {
        perror("msgget error");
        exit(1);
    }

    std::cout << "Connected with client." << std::endl;

    std::string connection;
    if(msgrcv(qid, &message_rcv, sizeof(struct message_text), 0, 0) == -1)
    {
        perror("msgrcv error");
        exit(1);
    }

    connection = message_rcv._message_text._buff;
    if(connection == "message queues")
    {
        message_queues(db);
    }
    else if(connection == "file mapping")
    {
        file_mapping(db);
    }


    delete db;
    return 0;
}