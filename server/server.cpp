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

#include "../containers/data_base.h"
#include "../ipc_connections/message_queue.h"
#include "../ipc_connections/ipc_type.h"

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

void message_queues(data_base *&db)
{
    ipc_type connection = ipc_type::MSG_QUEUE;
    message_queue msq;
//    key_t msq_key;
//    int qid;
//    struct message message;

//    if((msq_key = ftok(SERVER_KEY_PATHNAME, PROJECTD_ID)) == -1)
//    {
//        perror("ftok error");
//        exit(1);
//    }
//
//    if((qid = msgget(msq_key, IPC_CREAT | 0660)) == -1)
//    {
//        perror("msgget error");
//        exit(1);
//    }

    std::string command;

//    //region backup at the start
//    try
//    {
//        msq.receive_message(recover_choice);
//    }
//    catch(std::exception &ex)
//    {
//        perror(ex.what());
//        exit(1);
//    }
//
//    if(recover_choice == "restore")
//    {
//
//        try
//        {
//            bs.restore_data(db, connection);
//        }
//        catch(std::exception &ex)
//        {
//            perror(ex.what());
//            exit(1);
//        }
//
//    }
//    else if(recover_choice == "delete")
//    {
//
//        while(true)
//        {
//            std::cout << "Do you want to delete recover file?(y/n):" << std::endl;
//            std::getline(std::cin, command);
//            if(command == "y" || command == "Y")
//            {
//                bs.remove_file_from_system();
//            }
//            else if(command == "n" || command == "N")
//            {
//                break;
//            }
//            else
//            {
//                std::cout << "No such choice!" << std::endl;
//            }
//        }
//
//    }
//    //endregion backup at the start

    //переделать рекавер тут
//    std::ofstream broker(RECOVER_BROKER, std::ios::app);
//    if(!broker.is_open())
//    {
//        std::cerr << "Broker file can't be opened!" << std::endl;
//        exit(1);
//    }
//    std::ofstream fout_recover(RECOVERFILE_PATH, std::ios::app);
//    if(!fout_recover.is_open())
//    {
//        std::cerr << "Recover file can't be opened!" << std::endl;
//        exit(1);
//    }
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
            else if(command == "restore")
            {

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
    struct message message;
    auto *db = new data_base();

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

    message_queues(db);

    delete db;
    return 0;
}