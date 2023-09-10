#include <iostream>
#include <fstream>
#include "containers/data_base.h"

int main(int argc, char* argv[])
{
    if(argc > 2)
    {
        std::cout << "Enter the filepath only!\n";
        return -1;
    }
    else if(argc < 2)
    {
        std::cout << "Enter the filepath!\n";
        return -1;
    }

    auto *db = new data_base();

    std::string command;

    std::ifstream file(argv[1]);

    if (file.is_open())
    {
        while (std::getline(file, command)) {
            db->handle_request(command);
        }
    }
    else
    {
        std::cout << "File with name:" << argv[1] << " can't be opened" << std::endl;
    }


    delete db;

//    getchar();

    return 0;
}