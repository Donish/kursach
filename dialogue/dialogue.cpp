#include "dialogue.h"

void dialogue_with_user(data_base *&outer_data_base)
{
    std::string command;
    std::ifstream file;
    std::string filename;
    std::string choice;
    while(choice != "3")
    {
        std::cout << std::endl << "1)Command" << std::endl;
        std::cout << "2)File" << std::endl;
        std::cout << "3)Exit" << std::endl;
        std::cout << "4)Help" << std::endl;
        std::getline(std::cin, choice);
        if(choice == "1")
        {
            std::cout << "Enter the command:" << std::endl;
            std::getline(std::cin, command);

            outer_data_base->handle_request(command);
        }
        else if(choice == "2")
        {
            std::cout << "Enter the path to file:" << std::endl;
            std::getline(std::cin, filename);
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
            file.close();
        }
        else if(choice == "3")
        {
            break;
        }
        else if(choice == "4")
        {
            std::cout << "Enter 1 to enter your command" << std::endl << "Enter 2 to send the file with commands" << std::endl
            << "Enter 3 to exit" << std::endl;
        }
        else
        {
            std::cout << "Wrong number!" << std::endl;
        }
    }
}