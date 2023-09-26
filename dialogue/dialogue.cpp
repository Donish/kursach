#include "dialogue.h"

void dialogue_with_user(data_base *&outer_data_base)
{
    std::string command;
    std::ifstream file;
    std::string filename;
    int choice = 0;
    while(choice != 3)
    {
        std::cout << "1)Command\n2)File\n3)Exit\n4)Help\n\n";
        std::cin >> choice;
        if(choice == 1)
        {
            std::cout << "Enter the command:\n";
            std::getline(std::cin, command);

            outer_data_base->handle_request(command);
        }
        else if(choice == 2)
        {
            std::cout << "Enter the path to file:\n";
            std::cin >> filename;
            file.open(filename);
            if(file.is_open())
            {
                while(std::getline(file, command))
                {
                    outer_data_base->handle_request(command);
                }
            }
            else
            {
                std::cout << "File with name:" << filename << " can't be opened" << std::endl;
            }
        }
        else if(choice == 3)
        {
            break;
        }
        else if(choice == 4)
        {
            std::cout << "Enter 1 to enter your command\nEnter 2 to send the file with commands\nEnter 3 to exit\n\n";
        }
        else
        {
            std::cout << "Wrong number!\n";
        }
    }
}