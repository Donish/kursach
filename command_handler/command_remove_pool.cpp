#include "command_remove_pool.h"

bool command_remove_pool::can_execute(const std::string &request) noexcept
{
    if (request.rfind("REMOVE_POOL", 0) == 0)
    {
        auto argc = split(request, ' ');

        if (argc.size() == 2)
        {
            _pool_name = argc[1];

            return true;
        }
    }

    return false;
}

void command_remove_pool::execute(const std::string &request) const noexcept
{
    data_base::get_instance()->remove_pool(_pool_name);
}