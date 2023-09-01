#include "command_remove_data.h"

bool command_remove_data::can_execute(std::string const &request) noexcept
{
    if (request.rfind("REMOVE_DATA", 0))
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

void command_remove_data::execute(std::string const &request) const noexcept
{
    key *data_key = new key();
    data_key->_id_meeting = _id_meeting;

    data_base::get_instance()->remove_data(_pool_name, _scheme_name, _collection_name, data_key);

    delete data_key;
}