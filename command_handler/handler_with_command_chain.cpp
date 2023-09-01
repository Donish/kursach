#include "handler_with_command_chain.h"

handler_with_command_chain::handler_with_command_chain() : _first_handler(nullptr), _last_handler(nullptr)
{}

handler_with_command_chain::~handler_with_command_chain()
{
    while (_first_handler != nullptr)
    {
        _last_handler = _first_handler;
        _first_handler = _first_handler->_next_handler;
        delete _last_handler;
    }
}

bool handler_with_command_chain::handle(const std::string &request) const noexcept
{
    if (_first_handler == nullptr)
    {
        return false;
    }

    return _first_handler->handle(request);
}

handler_with_command_chain &handler_with_command_chain::add_handler(command *cmd)
{
    _last_handler = _first_handler == nullptr ?
            _first_handler = new handler_with_command(cmd) :
            _last_handler->_next_handler = new handler_with_command(cmd);

    return *this;
}

