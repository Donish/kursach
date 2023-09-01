#ifndef HANDLER_WITH_COMMAND
#define HANDLER_WITH_COMMAND

#include "handler.h"
#include "command.h"

class handler_with_command : public handler
{
private:

    command *_target_action;

public:

    handler_with_command(command *target_action);

    ~handler_with_command();

    handler_with_command(handler_with_command const &) = delete;
    handler_with_command(handler_with_command &&) = delete;
    handler_with_command &operator=(handler_with_command const &) = delete;
    handler_with_command &operator=(handler_with_command &&) = delete;

public:

    bool handle(std::string const & request) const noexcept;

};

#endif //HANDLER_WITH_COMMAND