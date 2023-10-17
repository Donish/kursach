#include "backup_system.h"

backup_system::backup_system(std::ifstream &file_with_filenames, const std::string &existing_files, const std::string &recover_directory):
    _existing_recover_files_path(existing_files),
    _recover_directory(recover_directory)
{
    std::string filename;
    while(std::getline(file_with_filenames, filename))
    {
        delete_carriage_symbol_with_guard(filename);
        _filenames.push_back(filename);
    }
}

void backup_system::backup_data(std::ifstream &broker)
{
    std::string choice;
    std::cout << std::endl << "Create new file or backup into existing?" << std::endl;
    std::cout << "1)Create" << std::endl;
    std::cout << "2)Existing" << std::endl;
    while(true)
    {
        std::getline(std::cin, choice);
        if(choice == "1" || choice == "2")
        {
            break;
        }
        else
        {
            std::cout << "Wrong number!" << std::endl;
        }
    }

    std::string command;
    std::string input_filename;
    int file_idx = -1;
    std::ofstream backup_file;
    if(choice == "1")
    {

        while(true)
        {
            std::cout << std::endl << "Name the file:" << std::endl;
            std::getline(std::cin, input_filename);
            if(!is_valid_filename(input_filename))
            {
                std::cout << "Error:" << std::endl;
                std::cout << "Filename must include letters and numbers only!" << std::endl;
                std::cout << "You can't create file with name 'broker', 'existing_recover_files' and 'terminating_commands'!" << std::endl;
                continue;
            }
            input_filename += ".txt";
            input_filename = _recover_directory + input_filename;
            if(file_exists(input_filename))
            {
                std::cout << "File already exists!" << std::endl;
                continue;
            }
            add_file_to_system(input_filename);
            break;
        }
        backup_file.open(input_filename);

    }
    else if(choice == "2")
    {
        if(_filenames.empty())
        {
            std::cout << "No files to backup!" << std::endl;
            return;
        }
        std::cout << std::endl << "Select file for backup:" << std::endl;
        for (int i = 0; i < _filenames.size(); ++i) {
            std::cout << i + 1 << ") " << _filenames[i] << std::endl;
        }

        while (true) {
            std::getline(std::cin, command);
            if(!is_valid_number(command))
            {
                std::cout << "Enter the number!" << std::endl;
                continue;
            }
            file_idx = std::stoi(command);
            if (file_idx > _filenames.size() || file_idx < 1) {
                std::cout << "Wrong number!" << std::endl;
                continue;
            }
            break;
        }

        backup_file.open(_filenames[file_idx - 1]);
        input_filename = _filenames[file_idx - 1];

    }

    //заполнение бэкап файла из брокер файла
    while(std::getline(broker, command))
    {
        delete_carriage_symbol_with_guard(command);
        backup_file << command << std::endl;
    }
    backup_file.close();
    std::cout << "Backup was made into file:" << input_filename << std::endl;
}

//TODO: сделать так, чтобы перед восстановлением данных удалялись существующие данные
std::vector<std::string> backup_system::restore_data()
{
    std::vector<std::string> result_commands;
    if(_filenames.empty())
    {
        std::cout << "No files to restore!" << std::endl;
        return result_commands;
    }
    std::cout << std::endl << "Select file to restore from: " << std::endl;
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
        if(!is_valid_number(command))
        {
            std::cout << "Enter the number!" << std::endl;
            continue;
        }
        file_idx = std::stoi(command);
        if (file_idx > _filenames.size() || file_idx < 1) {
            std::cout << "Wrong number!" << std::endl;
            continue;
        }
        break;
    }

    std::ifstream terminator(_terminating_commands_filepath);
    if(!terminator.is_open())
    {
        throw std::invalid_argument("can't open the file!");
    }
    while(std::getline(terminator, command))
    {
        delete_carriage_symbol_with_guard(command);
        result_commands.push_back(command);
    }
    terminator.close();
    std::ofstream terminator_out(_terminating_commands_filepath);
    terminator.close();

    restore_file.open(_filenames[file_idx - 1]);
    if(!restore_file.is_open())
    {
        throw std::invalid_argument("Can't open the restore file!");
    }

    while(std::getline(restore_file, command))
    {
        delete_carriage_symbol_with_guard(command);
        result_commands.push_back(command);
    }

    restore_file.close();
    std::cout << "Data restored from file: " << _filenames[file_idx - 1] << std::endl;
    return result_commands;
}

void backup_system::add_file_to_system(const std::string &file)
{
    _filenames.push_back(file);
    std::ofstream list_of_files(_existing_recover_files_path, std::ios::app);
    if(!list_of_files.is_open())
    {
        throw std::invalid_argument("Can't open the file with existing recover files!");
    }
    list_of_files << file << std::endl;
}

void backup_system::remove_file_from_system()
{
    if(_filenames.empty())
    {
        std::cout << "No files to delete!" << std::endl;
        return;
    }
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
        if(!is_valid_number(command))
        {
            std::cout << "Enter the number!" << std::endl;
            continue;
        }
        file_idx = std::stoi(command);
        if(file_idx > _filenames.size() || file_idx < 1)
        {
            std::cout << "Wrong number!" << std::endl;
            continue;
        }
        break;
    }
    file_idx--;

    if(remove(_filenames[file_idx].c_str())) // удаляем сам файл из директории
    {
        throw std::invalid_argument("Can't delete the file!");
    }
    _filenames.erase(_filenames.begin() + file_idx); // удаляем файл из вектора
    std::ofstream list_of_files(_existing_recover_files_path);
    for(auto &filename : _filenames) // удаляем файл из файла со списком файлов
    {
        delete_carriage_symbol_with_guard(filename);
        list_of_files << filename << std::endl;
    }
    list_of_files.close();

    std::cout << "File was succesfully deleted." << std::endl;
}

void backup_system::remove_file(const int &index)
{
    if(!remove(_filenames[index].c_str()))
    {
        throw std::invalid_argument("Can't delete the file!");
    }

    _filenames.erase(_filenames.begin() + index);

    std::ofstream list_of_files(_existing_recover_files_path);
    for(const auto &filename : _filenames)
    {
        list_of_files << filename << std::endl;
    }
    list_of_files.close();
}

bool backup_system::is_valid_filename(const std::string &filename)
{
    if(filename == "broker" || filename == "existing_recover_files" || filename == "terminating_commands")
    {
        return false;
    }
    for(const auto &symbol : filename)
    {
        if(!isalnum(symbol)) return false;
    }
    return true;
}

bool backup_system::file_exists(std::string &filename)
{
    for(const auto &file : _filenames)
    {
        if(filename == file) return true;
    }
    return false;
}

bool backup_system::is_valid_number(const std::string &number)
{
    for(const auto &i : number)
    {
        if(!isdigit(i))
        {
            return false;
        }
    }

    return true;
}

void backup_system::check_add_terminating_commands(std::string &command)
{
    if(command.starts_with("ADD_POOL"))
    {
        std::ofstream terminating_commands(_terminating_commands_filepath, std::ios::app);
        if(!terminating_commands.is_open())
        {
            throw std::invalid_argument("can't open the file!");
        }

        auto *cmd_add_pool = new command_add_pool();

        if(cmd_add_pool->can_execute(command))
        {
            auto argv = split(command, ' ');
            terminating_commands << "REMOVE_POOL " + argv[1] << std::endl;
        }

        terminating_commands.close();
        delete cmd_add_pool;
    }
}

std::vector<std::string> backup_system::split(const std::string &command, char delim)
{
    std::vector<std::string> result;
    std::string token;
    std::istringstream tokenStream(command);
    while (std::getline(tokenStream, token, delim))
    {
        result.push_back(token);
    }
    return result;
}