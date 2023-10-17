#include "mystring.h"

bool has_carriage_symbol(const std::string &command)
{
    if(command[command.size() - 1] == '\r')
    {
        return true;
    }

    return false;
}

void delete_carriage_symbol(std::string &command)
{
    command.pop_back();
}

void delete_carriage_symbol_with_guard(std::string &command)
{
    if(has_carriage_symbol(command))
    {
        delete_carriage_symbol(command);
    }
}