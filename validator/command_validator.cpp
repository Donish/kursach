#include "command_validator.h"
#include "../command_handler/command_add_collection.h"
#include "../command_handler/command_add_data.h"
#include "../command_handler/command_add_pool.h"
#include "../command_handler/command_add_scheme.h"
#include "../command_handler/command_get_data.h"
#include "../command_handler/command_get_data_between.h"
#include "../command_handler/command_remove_collection.h"
#include "../command_handler/command_remove_data.h"
#include "../command_handler/command_remove_pool.h"
#include "../command_handler/command_remove_scheme.h"
#include "../command_handler/command_update_data.h"

command_validator::command_validator()
{
    _chain
        .add_handler(new command_add_pool())
        .add_handler(new command_remove_pool())
        .add_handler(new command_add_scheme())
        .add_handler(new command_remove_scheme())
        .add_handler(new command_add_collection())
        .add_handler(new command_remove_collection())
        .add_handler(new command_add_data())
        .add_handler(new command_remove_data())
        .add_handler(new command_get_data())
        .add_handler(new command_get_data_between())
        .add_handler(new command_update_data());
}

bool command_validator::operator()(std::string &command)
{
    delete_carriage_symbol_with_guard(command);

//    if(command.starts_with("GET"))
//    {
//        return false;
//    }
    if(!_chain.validate(command))
    {
        return false;
    }

    return true;
}