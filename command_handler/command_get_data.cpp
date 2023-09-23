#include "command_get_data.h"

bool command_get_data::can_execute(const std::string &request)noexcept
{
    if (request.rfind("GET_DATA", 0) == 0)
    {
        auto argc = split(request, ' ');

        if (argc.size() == 5)
        {
            _pool_name = argc[1];
            _scheme_name = argc[2];
            _collection_name = argc[3];

            if (_digit_validator(argc[4]))
            {
                std::stringstream id_meeting(argc[4]);
                id_meeting >> _id_meeting;
            }
            else
            {
                return false;
            }

            return true;
        }
    }

    return false;
}

void command_get_data::execute(const std::string &request) const noexcept
{
    key * data_key = new key();

    data_key->set_id(_id_meeting);

    data_base::get_instance()->get_data(_pool_name, _scheme_name, _collection_name, data_key);

    delete data_key;
}