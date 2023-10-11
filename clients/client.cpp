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
#include <strings.h>
#include <string.h>

#define SERVER_KEY_PATHNAME "../server/tmp/mqueue_server_key"
#define PROJECT_ID 'M'

enum STATUS_CODE
{
    SUCCESS,
    ARGS_ERROR,
    IPC_ERROR
};

struct message_text
{
    int _qid;
    char _buff[500];
//    std::string _buff;
};

struct message
{
    long int _message_type;
    struct message_text _message_text;
};

void message_queues()
{
    key_t server_queue_key;
    int server_qid, myqid;
    struct message snd_message, rcv_messsage;

    if((myqid = msgget(IPC_PRIVATE, 0660)) == -1)
    {
        perror("msgget: myqid error");
        exit(1);
    }
    if((server_queue_key = ftok(SERVER_KEY_PATHNAME, PROJECT_ID)) == -1)
    {
        perror("ftok: server_queue_key error");
        exit(1);
    }
    if((server_qid = msgget(server_queue_key, 0)) == -1)
    {
        perror("msgget: server_qid error");
        exit(1);
    }

    snd_message._message_type = 1;
    snd_message._message_text._qid = myqid;
    std::string choice;
    std::string filename;
    std::string command;
    std::ifstream commands_file;

    while(true)
    {
        std::cout << std::endl << "1)Command" << std::endl;
        std::cout << "2)File" << std::endl;
        std::cout << "3)Exit" << std::endl;
        std::getline(std::cin, choice);

        if(choice == "1")
        {
            std::cout << "Enter the command:" << std::endl;
//            std::getline(std::cin, snd_message._message_text._buff);
            std::getline(std::cin, command);
            strcpy(snd_message._message_text._buff, command.c_str());

            if(msgsnd(server_qid, &snd_message, sizeof(struct message_text), 0) == -1)
            {
                perror("msgsnd: command error");
                exit(1);
            }
        }
        else if(choice == "2")
        {
            std::cout << "Enter the path to file:" << std::endl;
            std::getline(std::cin, filename);
            commands_file.open(filename);
            if(!commands_file.is_open())
            {
                std::cerr << "Can't open the file with commands!" << std::endl;
                continue;
            }

            while(std::getline(commands_file, command))
            {
                strcpy(snd_message._message_text._buff, command.c_str());
                if(msgsnd(server_qid, &snd_message, sizeof(struct message_text), 0) == -1)
                {
                    perror("msgsnd: file error");
                    exit(1);
                }
            }

            commands_file.close();
        }
        else if(choice == "3")
        {
            strcpy(snd_message._message_text._buff, "exit");
            if(msgsnd(server_qid, &snd_message, sizeof(message_text), 0) == -1)
            {
                perror("msgsnd: file");
                exit(1);
            }

            break;
        }
        else
        {
            std::cout << "No such choice!" << std::endl;
        }
    }

    if(msgctl(myqid, IPC_RMID, NULL) == -1)
    {
        perror("msgctl error");
        exit(1);
    }

    std::cout << "message queue is deleted." << std::endl;
}

int main()
{
    key_t server_queue_key;
    int server_qid, my_qid;
    struct message snd_message, rcv_message;

    if((my_qid = msgget(IPC_PRIVATE, 0660)) == -1)
    {
        perror("msgget: main, my_qid error");
        exit(1);
    }
    if((server_queue_key = ftok(SERVER_KEY_PATHNAME, PROJECT_ID)) == -1)
    {
        perror("ftok: main, server_qid error");
        exit(1);
    }
    if((server_qid = msgget(server_queue_key, 0)) == -1)
    {
        perror("msgget: main server_qid error");
        exit(1);
    }


    std::cout << "Connected with server." << std::endl;

    snd_message._message_type = 1;
    snd_message._message_text._qid = my_qid;
    message_queues();

    if(msgctl(my_qid, IPC_RMID, NULL) == -1)
    {
        perror("msgctl error");
        exit(1);
    }

    //connect
//    int status = connect(cliend_sd, (sockaddr*)&send_socket_addr, sizeof(send_socket_addr)); //help
//    if(status < 0)
//    {
//        std::cerr << "Socket connectiong error!" << std::endl;
//        return IPC_ERROR;
//    }
//    std::cout << "Connected to server!" << std::endl;
//
//    struct timeval start1, end1; //help
//    gettimeofday(&start1, nullptr); //help
//
//    message_queues();
//
//    gettimeofday(&end1, nullptr); //help
//
//    close(cliend_sd);
//    std::cout << "--------------------session--------------------" << std::endl;
//    std::cout << "Elapsed time: " << (end1.tv_sec = start1.tv_sec) << " sec." << std::endl;
//    std::cout << "Connection closed." << std::endl;

    return SUCCESS;
}