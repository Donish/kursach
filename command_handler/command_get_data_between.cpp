#include "command_get_data_between.h"

bool command_get_data_between::can_execute(std::string const &request) noexcept
{
    if (request.rfind("GET_RANGE", 0) == 0)
    {
        auto argc = split(request, ' ');

        if (argc.size() == 6)
        {
            _pool_name = argc[1];
            _scheme_name = argc[2];
            _collection_name = argc[3];

            if (_digit_validator(argc[4]))
            {
                std::stringstream id_meeting(argc[4]);
                id_meeting >> _id_meeting_first;
            }
            else
            {
                return false;
            }

            if (_digit_validator(argc[5]))
            {
                std::stringstream id_meeting(argc[5]);
                id_meeting >> _id_meeting_second;
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

void command_get_data_between::execute(std::string const &request) const noexcept
{
    key *key_1 = new key();
    key_1->set_id(_id_meeting_first);

    key *key_2 = new key();
    key_2->set_id(_id_meeting_second);

    data_base::get_instance()->get_data_between_keys(_pool_name, _scheme_name, _collection_name, key_1, key_2);

    delete key_1;
    delete key_2;
}