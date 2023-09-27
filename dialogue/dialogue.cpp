#include "dialogue.h"

void dialogue_with_user(data_base *&outer_data_base)
{
    std::string command;
    std::ifstream file;
    std::string filename;
    std::string choice;
    std::fstream fout_for_reboot("../file_system_recover/file_for_recover.txt");
    if(!fout_for_reboot.is_open())
    {
        std::cout << "Could't open file for recover!";
        return;
    }

    while(choice != "3")
    {
        std::cout << std::endl << "1)Command" << std::endl;
        std::cout << "2)File" << std::endl;
        std::cout << "3)Exit" << std::endl;
        std::cout << "4)Recover" << std::endl;
        std::cout << "5)Help" << std::endl;
        std::getline(std::cin, choice);
        if(choice == "1")
        {
            std::cout << "Enter the command:" << std::endl;
            std::getline(std::cin, command);

            try
            {
                outer_data_base->handle_request(command);
                fout_for_reboot << command << std::endl;
            }
            catch(std::exception &ex)
            {
                //TODO: cout log
            }
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
                    try
                    {
                        outer_data_base->handle_request(command);
                        fout_for_reboot << command << std::endl;
                    }
                    catch(std::exception &ex)
                    {
                        //TODO: cout log
                    }
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
            while(std::getline(fout_for_reboot, command))
            {
                try
                {
                    outer_data_base->handle_request(command);
                }
                catch(std::exception &ex)
                {
                    //TODO: cout log
                }
            }
        }
        else if(choice == "5")
        {
            std::cout << "Enter 1 to enter your command" << std::endl;
            std::cout << "Enter 2 to send the file with commands" << std::endl;
            std::cout << "Enter 3 to exit" << std::endl;
            std::cout << "Enter 4 to recover data from file (You can recover only when starting programm!)" << std::endl;
        }
        else
        {
            std::cout << "Wrong number!" << std::endl;
        }
    }
}