#include "dialogue.h"

const std::string filepath_recover = "../files_to_recover/file_for_recover.txt";

void dialogue_with_user(data_base *&outer_data_base)
{
    std::string command;
    std::ifstream file;
    std::string filename_with_commands;
    std::string choice;
    std::string recover_choice;
    std::ofstream fout_for_recover;
    std::ifstream fin_for_recover;

    while(true)
    {
        std::cout << "Do you want to recover data?" << std::endl;
        std::cout << "1)Yes" << std::endl << "2)No" << std::endl << "3)Exit" << std::endl;
        std::getline(std::cin, recover_choice);
        if(recover_choice == "1")
        {
            fin_for_recover.open(filepath_recover);
            if(!fin_for_recover.is_open())
            {
                std::cout << "Recover file can't be opened" << std::endl;
                return;
            }

            while(std::getline(fin_for_recover, command))
            {
                try
                {
                    outer_data_base->handle_request(command);
                }
                catch(std::exception &ex)
                {
                    //TODO: cout
                }
            }

            fin_for_recover.close();
            break;
        }
        else if(recover_choice == "2")
        {
            if(!remove("../files_to_recover/file_for_recover.txt"))
            {
                std::cout << "Recover file has been cleared" << std::endl;
            }
            break;
        }
        else if(recover_choice == "3")
        {
            return;
        }
        else
        {
            std::cout << "Wrong number!" << std::endl;
        }

    }

    fout_for_recover.open(filepath_recover, std::ios::app);
    if(!fout_for_recover.is_open())
    {
        std::cout << "Recover file can't be opened!" << std::endl;
        return;
    }

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

            try
            {
                outer_data_base->handle_request(command);
                if(command.rfind("GET", 0))
                {
                    fout_for_recover << command << std::endl;
                }
            }
            catch(std::exception &ex)
            {
//                std::cout << "exception\n";
            }
        }
        else if(choice == "2")
        {
            std::cout << "Enter the path to file:" << std::endl;
            std::getline(std::cin, filename_with_commands);
            file.open(filename_with_commands);
            if(file.is_open())
            {
                while(std::getline(file, command))
                {
                    try
                    {
                        outer_data_base->handle_request(command);
                        if(command.rfind("GET", 0))
                        {
                            fout_for_recover << command << std::endl;
                        }
                    }
                    catch(std::exception &ex)
                    {
//                        std::cout << "exception\n";
                    }
                }
            }
            else
            {
                std::cout << "File with name:" << filename_with_commands << " can't be opened" << std::endl;
            }
            file.close();
        }
        else if(choice == "3")
        {
            break;
        }
        else if(choice == "4")
        {
            std::cout << "Enter 1 to enter your command" << std::endl;
            std::cout << "Enter 2 to send the file with commands" << std::endl;
            std::cout << "Enter 3 to exit" << std::endl;
        }
        else
        {
            std::cout << "Wrong number!" << std::endl;
        }
    }

    fout_for_recover.close();
}