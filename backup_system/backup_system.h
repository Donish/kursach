#ifndef BACKUP_SYSTEM_H
#define BACKUP_SYSTEM_H

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include "../string_methods/mystring.h"
#include "../command_handler/command_add_pool.h"

class backup_system final
{
private:

    std::vector<std::string> _filenames;
    std::string _existing_recover_files_path;
    std::string _recover_directory;
    std::string _terminating_commands_filepath = "../recover_files/terminating_commands.txt";

public:

    explicit backup_system(std::ifstream &file_with_filenames, const std::string &existing_files, const std::string &recover_directory);

    void backup_data(std::ifstream &broker);

    std::vector<std::string> restore_data();

public:

    void remove_file_from_system();

    void check_add_terminating_commands(std::string &command);

private:

    void remove_file(const int &index);

    void add_file_to_system(const std::string &file);

    bool is_valid_filename(const std::string &filename);

    bool file_exists(std::string &filename);

    bool is_valid_number(const std::string &number);

    std::vector<std::string> split(const std::string &command, char delim);

};

#endif //BACKUP_SYSTEM_H