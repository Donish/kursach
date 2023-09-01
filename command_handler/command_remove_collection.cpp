#include "command_remove_collection.h"

bool command_remove_collection::can_execute(const std::string &request) noexcept
{
    if (request.rfind("REMOVE_COLLECTION", 0))
    {
        auto argc = split(request, ' ');

        if (argc.size() == 4)
        {
            _pool_name = argc[1];
            _scheme_name = argc[2];
            _collection_name = argc[3];

            return true;
        }
    }

    return false;
}

void command_remove_collection::execute(const std::string &request) const noexcept
{
    data_base::get_instance()->remove_collection(_pool_name, _scheme_name, _collection_name);
}