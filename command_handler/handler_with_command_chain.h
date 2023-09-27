#ifndef HANDLER_WITH_COMMAND_CHAIN
#define HANDLER_WITH_COMMAND_CHAIN

#include "handler_with_command.h"

class handler_with_command_chain final
{



private:

    handler * _first_handler;
    handler * _last_handler;

public:

    handler_with_command_chain();
    ~handler_with_command_chain();

    handler_with_command_chain(handler_with_command_chain const &) = delete;
    handler_with_command_chain(handler_with_command_chain &&) = delete;
    handler_with_command_chain &operator=(handler_with_command_chain const &) = delete;
    handler_with_command_chain &operator=(handler_with_command_chain &&) = delete;

public:

    bool handle(std::string const &request) const;

public:

    handler_with_command_chain &add_handler(command *cmd);

};

#endif //HANDLER_WITH_COMMAND_CHAIN