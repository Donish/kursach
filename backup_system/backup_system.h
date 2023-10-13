#ifndef BACKUP_SYSTEM_H
#define BACKUP_SYSTEM_H

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include "../string_methods/mystring.h"
#include "../containers/data_base.h"

class backup_system final
{
private:

    std::vector<std::string> _filenames;
    std::string _existing_recover_files_path;
    std::string _recover_directory;

public:

    explicit backup_system(std::ifstream &file_with_filenames, const std::string &existing_files, const std::string &recover_directory);

    void backup_data(std::ifstream &broker);

    void restore_data(data_base *&db);

public:

    void remove_file_from_system();

private:

    void remove_file(const int &index);

    void add_file_to_system(const std::string &file);

    bool is_valid_filename(const std::string &filename);

    void check_txt_format(std::string &filename);

    bool file_exists(std::string &filename);

};

#endif //BACKUP_SYSTEM_H