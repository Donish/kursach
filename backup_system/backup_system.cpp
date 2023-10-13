#include "backup_system.h"

backup_system::backup_system(std::ifstream &file_with_filenames, const std::string &existing_files):
    _existing_recover_files_path(existing_files)
{
    std::string filename;
    while(std::getline(file_with_filenames, filename))
    {
        _filenames.push_back(filename);
    }
}

void backup_system::backup_data(std::ifstream &broker)
{
    std::cout << "Select file for backup:" << std::endl;
    for(int i = 0; i < _filenames.size(); ++i)
    {
        std::cout << i + 1 << ") " << _filenames[i] << std::endl;
    }

    std::string command;
    int file_idx = -1;
    std::ofstream backup_file;
    while(true)
    {
        std::getline(std::cin, command);
        file_idx = std::stoi(command);
        if (file_idx > _filenames.size() || file_idx < 1) {
            std::cout << "Wrong number!" << std::endl;
            continue;
        }
        break;
    }

    //выбор файла
    backup_file.open(_filenames[file_idx - 1], std::ios::app);
    if(!backup_file.is_open())
    {
        throw std::invalid_argument("Can't open the backup file!");
    }

    //заполнение бэкап файла из брокер файла
    while(std::getline(broker, command))
    {
        if(has_carriage_symbol(command))
        {
            delete_carriage_symbol(command);
        }

        backup_file << command << std::endl;
    }
    backup_file.close();
}

void backup_system::restore_data(data_base *&db)
{
    std::cout << "Select file to restore from: " << std::endl;
    for(int i = 0; i < _filenames.size(); ++i)
    {
        std::cout << i + 1 << ") " << _filenames[i] << std::endl;
    }

    std::string command;
    int file_idx = -1;
    std::ifstream restore_file;
    while(true)
    {
        std::getline(std::cin, command);
        file_idx = std::stoi(command);
        if (file_idx > _filenames.size() || file_idx < 1) {
            std::cout << "Wrong number!" << std::endl;
            continue;
        }
        break;
    }

    restore_file.open(_filenames[file_idx - 1]);
    if(!restore_file.is_open())
    {
        throw std::invalid_argument("Can't open the restore file!");
    }

    while(std::getline(restore_file, command))
    {
        try
        {
            db->handle_request(command);
        }
        catch(std::exception &ex)
        {
            throw std::invalid_argument(ex.what());
        }
    }

    restore_file.close();
}

void backup_system::add_file_to_system(std::string file, const std::string &recover_filenames)
{
    _filenames.push_back(file);
    std::ofstream list_of_files(recover_filenames, std::ios::app);
    if(!list_of_files.is_open())
    {
        throw std::invalid_argument("Can't open the file with existing recover files!");
    }
    list_of_files << recover_filenames << std::endl;
}

void backup_system::remove_file_from_system()
{
    std::cout << "Select file to remove:" << std::endl;
    for(int i = 0; i < _filenames.size(); ++i)
    {
        std::cout << i + 1 << ") " << _filenames[i] << std::endl;
    }

    std::string command;
    int file_idx = -1;
    while(true)
    {
        std::getline(std::cin, command);
        file_idx = std::stoi(command);
        if(file_idx > _filenames.size() || file_idx < 1)
        {
            std::cout << "Wrong number!" << std::endl;
            continue;
        }
        break;
    }
    file_idx--;

    //TODO: доделать удаление файла из вектора и из файла со списком рекавер файлов
}