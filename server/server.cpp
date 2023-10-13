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
#include "../backup_system/backup_system.h"

#define SERVER_KEY_PATHNAME "../tmp/mqueue_server_key"
#define PROJECTD_ID 'M'
#define RECOVERFILE_PATH "../recoverfile.txt"
#define RECOVER_FILENAMES_PATH "../recover_files/existing_recover_files.txt"

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

void message_queues(backup_system &bs)
{
    key_t msq_key;
    int qid;
    struct message message;

    if((msq_key = ftok(SERVER_KEY_PATHNAME, PROJECTD_ID)) == -1)
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
    std::ifstream fin_recover;
    std::string recover_choice;

    while(true)
    {
        std::cout << "Do you want to recover data?" << std::endl;
        std::cout << "1)Yes" << std::endl << "2)No" << std::endl << "3)Exit" << std::endl;
        std::getline(std::cin, recover_choice);

        if(recover_choice == "1")
        {
            fin_recover.open(RECOVERFILE_PATH);
            if(!fin_recover.is_open())
            {
                std::cerr << "Recover file can't be opened!" << std::endl;
                exit(1);
            }

            while(std::getline(fin_recover, command))
            {
                try
                {
                    db->handle_request(command);
                }
                catch(std::exception &ex)
                {
                    //TODO: cout
                }
            }

            fin_recover.close();
            break;
        }
        else if(recover_choice == "2")
        {
            if(!remove(RECOVERFILE_PATH))
            {
                std::cout << "Recover file has been cleared." << std::endl;
            }
            break;
        }
        else if(recover_choice == "3")
        {
            exit(0);
        }
        else
        {
            std::cout << "No such choice!" << std::endl;
        }
    }

    std::ofstream fout_recover(RECOVERFILE_PATH, std::ios::app);
    if(!fout_recover.is_open())
    {
        std::cerr << "Recover file can't be opened!" << std::endl;
        exit(1);
    }
    std::string command_todo;
    while(true)
    {
        if(msgrcv(qid, &message, sizeof(struct message_text), 0, 0) == -1)
        {
            perror("msgrcv error");
            exit(1);
        }
        if(strcmp(message._message_text._buff, "exit") == 0)
        {
            break;
        }

        std::cout << message._message_text._buff << std::endl;

        try
        {
            command_todo = std::string(message._message_text._buff);
            db->handle_request(command_todo);
            if(command_todo.rfind("GET", 0))
            {
                fout_recover << command_todo << std::endl;
            }

        }
        catch(std::exception &ex)
        {
            // TODO: cout
        }

        std::cout << "message received." << std::endl;
    }
}

int main()
{
    key_t msg_queue_key;
    int qid;
    struct message message;
    std::ifstream recover_filenames(RECOVER_FILENAMES_PATH);
    if(!recover_filenames.is_open())
    {
        perror("file open: main error");
        exit(1);
    }
    backup_system bs(recover_filenames);
    recover_filenames.close();

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

    message_queues(bs);

    return 0;
}