#ifndef COMMAND_VALIDATOR_H
#define COMMAND_VALIDATOR_H

#include "../command_handler/handler_with_command_chain.h"
#include "../string_methods/mystring.h"

class command_validator
{
private:

    static command_validator *_instance;

    handler_with_command_chain _chain;

public:

    command_validator();

    ~command_validator() = default;

    bool operator()(std::string &command);

};


#endif //COMMAND_VALIDATOR_H
