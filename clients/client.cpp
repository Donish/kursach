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

#define SEMAPHORE_KEY 54321
#define SEMAPHORE_COUNT 1

#define SHARED_MEMORY_KEY 12345
#define SHARED_MEMORY_SIZE 4096

#define FILE_MAPPING_PATHNAME "../tmp/example.txt"

#define SERVER_KEY_PATHNAME "../tmp/mqueue_server_key"
#define PROJECT_ID 'M'

#define RECOVER_FILENAMES_PATH "../recover_files/existing_recover_files.txt"
#define RECOVER_DIRECTORY "../recover_files/"
#define RECOVER_BROKER "../recover_files/broker.txt"
#define RECOVER_TERMINATING_COMMANDS "../recover_files/terminating_commands.txt"
#define TRASH_COMMANDS "../recover_files/trash_commands.txt"

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

struct SharedData
{
    int count;
    char msg[999];
};

void shared_memory(backup_system &bs)
{
    char msg[1500];

    int shm_id = shmget(SHARED_MEMORY_KEY, SHARED_MEMORY_SIZE, IPC_CREAT | 0666);
    if(shm_id == -1)
    {
        perror("shmget error");
        exit(1);
    }

    auto *shared_data = (SharedData*) shmat(shm_id, nullptr, 0);
    if(shared_data == (SharedData*)(-1))
    {
        perror("shmat error");
        exit(1);
    }

    int sem_id = semget(SEMAPHORE_KEY, SEMAPHORE_COUNT, IPC_CREAT | 0666);
    if(sem_id == -1)
    {
        perror("semget error");
        exit(1);
    }

    std::string command;
    std::ifstream broker_in;
    std::ofstream broker_out;
    command_validator cmd_validate;
    std::string choice;
    std::vector<std::string> commands_to_restore;
    std::ofstream trash_commands(TRASH_COMMANDS);

    broker_out.open(RECOVER_BROKER);

    //region backup at start
    while(true)
    {
        struct sembuf sem_ops[1] = {0, -1, 0};
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
                else
                {
                    trash_commands << i << std::endl;
                }
                bs.check_add_terminating_commands(i);

                memset(&(shared_data->msg), 0, sizeof(shared_data->msg));
                strcpy(shared_data->msg, i.c_str());

                sem_ops[0].sem_op = 1;
                semop(sem_id, sem_ops, 1);
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
            memset(&(shared_data->msg), 0, sizeof(shared_data->msg));
            strcpy(shared_data->msg, "exit");
            sem_ops[0].sem_op = 1;
            semop(sem_id, sem_ops, 1);

            exit(0);
        }
        else
        {
            std::cout << "No such option!" << std::endl;
//            sem_ops[0].sem_op = 1;
//            semop(sem_id, sem_ops, 1);
        }
    }
    //endregion backup at start

    std::string sub_choice;
    std::string filename;
    std::ifstream commands_file;

    while(true)
    {
        struct sembuf sem_ops[1] = {0, -1, 0};
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

            memset(&(shared_data->msg), 0, sizeof(shared_data->msg));
            strcpy(shared_data->msg, command.c_str());
            sem_ops[0].sem_op = 1;
            semop(sem_id, sem_ops, 1);

            bs.check_add_terminating_commands(command);
            if(cmd_validate(command))
            {
                broker_out << command << std::endl;
            }
            else
            {
                trash_commands << command << std::endl;
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
                memset(&(shared_data->msg), 0, sizeof(shared_data->msg));
                strcpy(shared_data->msg, command.c_str());

                sem_ops[0].sem_op = 1;
                semop(sem_id, sem_ops, 1);

                delete_carriage_symbol_with_guard(command);
                if(cmd_validate(command))
                {
                    broker_out << command << std::endl;
                }
                else
                {
                    trash_commands << command << std::endl;
                }
                bs.check_add_terminating_commands(command);
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
                else
                {
                    trash_commands << i << std::endl;
                }
                bs.check_add_terminating_commands(i);

                memset(&(shared_data->msg), 0, sizeof(shared_data->msg));
                strcpy(shared_data->msg, i.c_str());

                sem_ops[0].sem_op = 1;
                semop(sem_id, sem_ops, 1);
            }

        }
        else if(choice == "5")
        {

            while(true)
            {
                std::cout << "Make backup before exit?(y/n)" << std::endl;
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

            memset(&(shared_data->msg), 0, sizeof(shared_data->msg));
            strcpy(shared_data->msg, "exit");
            sem_ops[0].sem_op = 1;
            semop(sem_id, sem_ops, 1);

            break;
        }
        else
        {
            std::cout << "No such choice!" << std::endl;
        }

    }

    trash_commands.close();
    shmdt(shared_data);
    shmctl(shm_id, IPC_RMID, nullptr);
    semctl(sem_id, 0, IPC_RMID);
}

void file_mapping(backup_system &bs)
{
    int shm_fd;
    void *shm_region;
    const int file_size = 100;

    int fd = open(FILE_MAPPING_PATHNAME, O_CREAT | O_RDWR);
    if(fd == -1)
    {
        perror("file error");
        exit(1);
    }

    char *addr = (char*) mmap(NULL, file_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if(addr == MAP_FAILED)
    {
        perror("file mapping error");
        close(fd);
        exit(1);
    }
    close(fd);

    int sem_id = semget(SEMAPHORE_KEY, SEMAPHORE_COUNT, IPC_CREAT | 0666);
    if(sem_id == -1)
    {
        perror("semget error");
        exit(1);
    }

    std::string command;
    std::ifstream broker_in;
    std::ofstream broker_out;
    command_validator cmd_validate;
    std::string choice;
    std::vector<std::string> commands_to_restore;
    std::ofstream trash_commands(TRASH_COMMANDS);

    broker_out.open(RECOVER_BROKER);

    //region backup at start
    while(true)
    {
        struct sembuf sem_ops[1] = {0, -1, 0};
//        semop(sem_id, sem_ops, 1);
        std::cout << std::endl << "Do you want to restore data?" << std::endl;
        std::cout << "1)Yes" << std::endl << "2)No" << std::endl << "3)Delete recover file" << std::endl << "4)Exit" << std::endl;
        std::getline(std::cin, choice);

        if(choice == "1")
        {
//
//            sem_ops[0].sem_op = 1;
//            semop(sem_id, sem_ops, 1);

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
                else
                {
                    trash_commands << i << std::endl;
                }
                bs.check_add_terminating_commands(i);

                sprintf(addr, "%s", i.c_str());
                sem_ops[0].sem_op = 1;
                semop(sem_id, sem_ops, 1);
            }
            break;

        }
        else if(choice == "2")
        {
//            sem_ops[0].sem_op = 1;
//            semop(sem_id, sem_ops, 1);
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

//            sem_ops[0].sem_op = 1;
//            semop(sem_id, sem_ops, 1);

        }
        else if(choice == "4")
        {
            sprintf(addr, "%s", "exit");
            sem_ops[0].sem_op = 1;
            semop(sem_id, sem_ops, 1);
            exit(0);
        }
        else
        {
            std::cout << "No such option!" << std::endl;
//            sem_ops[0].sem_op = 1;
//            semop(sem_id, sem_ops, 1);
        }
    }
    //endregion backup at start

    std::string sub_choice;
    std::string filename;
    std::ifstream commands_file;

    while(true)
    {
        struct sembuf sem_ops[1] = {0, -1, 0};
//        semop(sem_id, sem_ops, 1);
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

            sprintf(addr, "%s", command.c_str());
            sem_ops[0].sem_op = 1;
            semop(sem_id, sem_ops, 1);

            bs.check_add_terminating_commands(command);
            if(cmd_validate(command))
            {
                broker_out << command << std::endl;
            }
            else
            {
                trash_commands << command << std::endl;
            }

        }
        else if(choice == "2")
        {
//            sem_ops[0].sem_op = 1;
//            semop(sem_id, sem_ops, 1);

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
                sprintf(addr, "%s", command.c_str());

                sem_ops[0].sem_op = 1;
                semop(sem_id, sem_ops, 1);

                delete_carriage_symbol_with_guard(command);
//                sem_ops[0].sem_op = -1;
//                semop(sem_id, sem_ops, 1);
                if(cmd_validate(command))
                {
                    broker_out << command << std::endl;
                }
                else
                {
                    trash_commands << command << std::endl;
                }
                bs.check_add_terminating_commands(command);


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

//            sem_ops[0].sem_op = 1;
//            semop(sem_id, sem_ops, 1);

        }
        else if(choice == "4")
        {

//            sem_ops[0].sem_op = 1;
//            semop(sem_id, sem_ops, 1);

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
                else
                {
                    trash_commands << i << std::endl;
                }
                bs.check_add_terminating_commands(i);

//                sem_ops[0].sem_op = -1;
//                semop(sem_id, sem_ops, 1);
                sprintf(addr, "%s", i.c_str());

                sem_ops[0].sem_op = 1;
                semop(sem_id, sem_ops, 1);
            }

        }
        else if(choice == "5")
        {

            while(true)
            {
                std::cout << "Make backup before exit?(y/n)" << std::endl;
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

            sprintf(addr, "%s", "exit");
            sem_ops[0].sem_op = 1;
            semop(sem_id, sem_ops, 1);

            break;

        }
        else
        {
            std::cout << "No such choice!" << std::endl;
//            sem_ops[0].sem_op = 1;
//            semop(sem_id, sem_ops, 1);
        }
    }

    trash_commands.close();
    munmap(addr, file_size);
}

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
    std::ofstream trash_commands(TRASH_COMMANDS);

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
                else
                {
                    trash_commands << i << std::endl;
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
            strcpy(snd_message._message_text._buff, "exit");
            if(msgsnd(server_qid, &snd_message, sizeof(message_text), 0) == -1)
            {
                perror("msgsnd: file");
                exit(1);
            }
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
            else
            {
                trash_commands << command << std::endl;
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
                else
                {
                    trash_commands << command << std::endl;
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
                else
                {
                    trash_commands << i << std::endl;
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


            while(true)
            {
                std::cout << "Make backup before exit?(y/n)" << std::endl;
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
    broker_out.close();
    trash_commands.close();
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

    snd_message._message_type = 1;
    snd_message._message_text._qid = my_qid;
    std::cout << "Connected with server." << std::endl;
    std::string choice;

    while(true)
    {
        std::cout << std::endl << "Select IPC:" << std::endl;
        std::cout << "1)Message queues" << std::endl;
        std::cout << "2)File mapping" << std::endl;
        std::cout << "3)Shared memory" << std::endl;
        std::cout << "4)Exit" << std::endl;
        std::getline(std::cin, choice);

        if(choice != "1" && choice != "2" && choice != "3" && choice != "4")
        {
            std::cout << "No such option!" << std::endl;
            continue;
        }
        break;
    }

    if(choice == "1")
    {
        strcpy(snd_message._message_text._buff, "message queues");
        if(msgsnd(server_qid, &snd_message, sizeof(struct message_text), 0) == -1)
        {
            perror("msgsnd error: main");
            exit(1);
        }
        message_queues(bs);
    }
    else if(choice == "2")
    {
        strcpy(snd_message._message_text._buff, "file mapping");
        if(msgsnd(server_qid, &snd_message, sizeof(struct message_text), 0) == -1)
        {
            perror("msgsnd error: main");
            exit(1);
        }
        file_mapping(bs);
    }
    else if(choice == "3")
    {
        strcpy(snd_message._message_text._buff, "shared memory");
        if(msgsnd(server_qid, &snd_message, sizeof(struct message_text), 0) == -1)
        {
            perror("msgsnd error: main");
            exit(1);
        }
        shared_memory(bs);
    }
    else if(choice == "4")
    {
        strcpy(snd_message._message_text._buff, "exit");
        if(msgsnd(server_qid, &snd_message, sizeof(struct message_text), 0) == -1)
        {
            perror("msgsnd error: main");
            exit(1);
        }
    }


    if(msgctl(my_qid, IPC_RMID, NULL) == -1)
    {
        perror("msgctl error");
        exit(1);
    }

    return 0;
}