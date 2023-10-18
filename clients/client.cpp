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

#include "../backup_system/backup_system.h"
#include "../validator/command_validator.h"

#define SERVER_KEY_PATHNAME "../tmp/mqueue_server_key"
#define PROJECT_ID 'M'
#define RECOVER_FILENAMES_PATH "../recover_files/existing_recover_files.txt"
#define RECOVER_DIRECTORY "../recover_files/"
#define RECOVER_BROKER "../recover_files/broker.txt"
#define RECOVER_TERMINATING_COMMANDS "../recover_files/terminating_commands.txt"

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
    std::ifstream broker_in;
    std::ofstream broker_out;
    key_t server_queue_key;
    int server_qid, myqid;
    command_validator cmd_validate;
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
    std::vector<std::string> commands_to_restore;

    broker_out.open(RECOVER_BROKER);
    //region backup at the start
    while(true)
    {
        std::cout << std::endl << "Do you want to restore data?" << std::endl;
        std::cout << "1)Yes" << std::endl << "2)No" << std::endl << "3)Delete recover file" << std::endl << "4)Exit" << std::endl;
        std::getline(std::cin, choice);

        if(choice == "1")
        {

            try
            {
                commands_to_restore = bs.restore_data();
            }
            catch(std::exception &ex)
            {
                perror(ex.what());
                exit(1);
            }
            if(commands_to_restore.empty())
            {
                continue;
            }
            for(auto &i : commands_to_restore)
            {
                if(cmd_validate(i))
                {
                    broker_out << i << std::endl;
                }
                bs.check_add_terminating_commands(i);
                strcpy(snd_message._message_text._buff, i.c_str());
                if(msgsnd(server_qid, &snd_message, sizeof(struct message_text), 0) == -1)
                {
                    perror("msgsnd error");
                    exit(1);
                }
            }
            break;

        }
        else if(choice == "2")
        {
            break;
        }
        else if(choice == "3")
        {

            try
            {
                bs.remove_file_from_system();
            }
            catch(std::exception &ex)
            {
                perror(ex.what());
                exit(1);
            }

        }
        else if(choice == "4")
        {
            exit(0);
        }
        else
        {
            std::cout << "No such option!" << std::endl;
        }
    }
    //endregion backup at the start

    std::string sub_choice;
    //region main menu
    while(true)
    {

        std::cout << std::endl << "1)Command" << std::endl;
        std::cout << "2)File" << std::endl;
        std::cout << "3)Backup data" << std::endl;
        std::cout << "4)Restore data" << std::endl;
        std::cout << "5)Exit" << std::endl;
        std::getline(std::cin, choice);

        if(choice == "1")
        {

            std::cout << "Enter the command:" << std::endl;

            std::getline(std::cin, command);
            strcpy(snd_message._message_text._buff, command.c_str());

            if(msgsnd(server_qid, &snd_message, sizeof(struct message_text), 0) == -1)
            {
                perror("msgsnd: command error");
                exit(1);
            }

            bs.check_add_terminating_commands(command);
            if(cmd_validate(command))
            {
                broker_out << command << std::endl;
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
                delete_carriage_symbol_with_guard(command);
                strcpy(snd_message._message_text._buff, command.c_str());
                if(msgsnd(server_qid, &snd_message, sizeof(struct message_text), 0) == -1)
                {
                    perror("msgsnd: file error");
                    exit(1);
                }
                bs.check_add_terminating_commands(command);
                if(cmd_validate(command))
                {
                    broker_out << command << std::endl;
                }
            }
            commands_file.close();

        }
        else if(choice == "3")
        {

            broker_out.close();
            broker_in.open(RECOVER_BROKER);
            if(!broker_in.is_open())
            {
                perror("can't open the file");
                exit(1);
            }
            try
            {
                bs.backup_data(broker_in);
            }
            catch(std::exception &ex)
            {
                perror(ex.what());
                exit(1);
            }
            broker_in.close();
            broker_out.open(RECOVER_BROKER, std::ios::app);
            if(!broker_out.is_open())
            {
                perror("can't open the file");
                exit(1);
            }

        }
        else if(choice == "4")
        {

            try
            {
                commands_to_restore = bs.restore_data();
            }
            catch(std::exception &ex)
            {
                perror(ex.what());
                exit(1);
            }

            if(commands_to_restore.empty())
            {
                continue;
            }
            broker_out.close();
            broker_out.open(RECOVER_BROKER);

            for(auto &i : commands_to_restore)
            {
                if(cmd_validate(i))
                {
                    broker_out << i << std::endl;
                }
                bs.check_add_terminating_commands(i);
                strcpy(snd_message._message_text._buff, i.c_str());
                if(msgsnd(server_qid, &snd_message, sizeof(struct message_text), 0) == -1)
                {
                    perror("msgsnd error");
                    exit(1);
                }
            }

        }
        else if(choice == "5")
        {

            strcpy(snd_message._message_text._buff, "exit");
            if(msgsnd(server_qid, &snd_message, sizeof(message_text), 0) == -1)
            {
                perror("msgsnd: file");
                exit(1);
            }

            std::cout << "Make backup before exit?(y/n)" << std::endl;
            while(true)
            {
                std::getline(std::cin, sub_choice);
                if(sub_choice != "y" && sub_choice != "n")
                {
                    std::cout << "No such choice!" << std::endl;
                    continue;
                }
                break;
            }
            if(sub_choice == "y")
            {

                broker_out.close();
                broker_in.open(RECOVER_BROKER);
                if(!broker_in.is_open())
                {
                    perror("can't open the file!");
                    exit(1);
                }
                try
                {
                    bs.backup_data(broker_in);
                }
                catch(std::exception &ex)
                {
                    perror(ex.what());
                    exit(1);
                }
                broker_in.close();
                broker_out.open(RECOVER_BROKER, std::ios::app);
                if(!broker_out.is_open())
                {
                    perror("can't open the file!");
                    exit(1);
                }

            }

            break;

        }
        else
        {
            std::cout << "No such choice!" << std::endl;
        }
    }
    broker_out.close();
    //endregion main menu

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

    std::ifstream existing_filenames(RECOVER_FILENAMES_PATH);
    if(!existing_filenames.is_open())
    {
        perror("can't open the file");
        exit(1);
    }
    backup_system bs(existing_filenames, RECOVER_FILENAMES_PATH, RECOVER_DIRECTORY);
    existing_filenames.close();

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
    message_queues(bs);

    if(msgctl(my_qid, IPC_RMID, NULL) == -1)
    {
        perror("msgctl error");
        exit(1);
    }

    return 0;
}