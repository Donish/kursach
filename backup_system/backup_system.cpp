#include "backup_system.h"

backup_system::backup_system(std::ifstream &file_with_filenames, const std::string &existing_files, const std::string &recover_directory):
    _existing_recover_files_path(existing_files),
    _recover_directory(recover_directory)
{
    std::string filename;
    while(std::getline(file_with_filenames, filename))
    {
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
                std::cout << "Required at least one letter!" << std::endl;
                continue;
            }
            check_txt_format(input_filename);
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

        std::cout << std::endl << "Select file for backup:" << std::endl;
        for (int i = 0; i < _filenames.size(); ++i) {
            std::cout << i + 1 << ") " << _filenames[i] << std::endl;
        }

        while (true) {
            std::getline(std::cin, command);
            file_idx = std::stoi(command);
            if (file_idx > _filenames.size() || file_idx < 1) {
                std::cout << "Wrong number!" << std::endl;
                continue;
            }
            break;
        }

        backup_file.open(_filenames[file_idx - 1], std::ios::app);
        if(!backup_file.is_open())
        {
            throw std::invalid_argument("Can't open the backup file!");
        }

    }

    //заполнение бэкап файл из брокер файла
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

    while(true)
    {
        std::cout << "Do you want to delete this recover file?(y/n): ";
        std::getline(std::cin, command);
        if(command == "y" || command == "Y")
        {
            restore_file.close();
            remove_file(file_idx - 1);
            break;
        }
        else if(command == "n" || command == "N")
        {
            restore_file.close();
            break;
        }
        else
        {
            std::cout << "No such choice!" << std::endl;
        }
    }

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


    if(!remove(_filenames[file_idx].c_str())) // удаляем сам файл из директории
    {
        throw std::invalid_argument("Can't delete the file!");
    }
    _filenames.erase(_filenames.begin() + file_idx); // удаляем файл из вектора
    std::ofstream list_of_files(_existing_recover_files_path);
    for(const auto &filename : _filenames) // удаляем файл из файла со списком файлов
    {
        list_of_files << filename << std::endl;
    }
    list_of_files.close();
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
    if(filename == ".txt")
    {
        return false;
    }
    for(const auto &symbol : filename)
    {
        if(isalpha(symbol)) return true;
    }
    return false;
}

void backup_system::check_txt_format(std::string &filename)
{
    int length = (int)filename.length();
    if(length < 5)
    {
        filename += ".txt";
    }
    else
    {
        if(filename.substr(length - 3) != ".txt")
        {
            filename += ".txt";
        }
    }
}

bool backup_system::file_exists(std::string &filename)
{
    for(const auto &file : _filenames)
    {
        if(filename == file) return true;
    }
    return false;
}