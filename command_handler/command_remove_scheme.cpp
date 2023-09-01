#include "command_remove_scheme.h"

bool command_remove_scheme::can_execute(const std::string &request) noexcept
{
    if (request.rfind("REMOVE_SCHEME", 0))
    {
        auto argc = split(request, ' ');

        if (argc.size() == 3)
        {
            _pool_name = argc[1];
            _scheme_name = argc[2];

            return true;
        }
    }
    return false;
}

void command_remove_scheme::execute(const std::string &request) const noexcept
{
    data_base::get_instance()->remove_scheme(_pool_name, _scheme_name);
}