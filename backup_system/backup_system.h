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

public:

    explicit backup_system(std::ifstream &file_with_filenames, const std::string &existing_files);

    void backup_data(std::ifstream &broker);

    void restore_data(data_base *&db);

public:

    void add_file_to_system(std::string file, const std::string &recover_filenames);

    void remove_file_from_system();

};

#endif //BACKUP_SYSTEM_H