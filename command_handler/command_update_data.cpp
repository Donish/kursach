#include "command_update_data.h"

bool command_update_data::can_execute(const std::string &request) noexcept
{
    if (request.rfind("UPDATE_DATA", 0) == 0)
    {
        auto argc = split(request, ' ');

        if (argc.size() == 16)
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

            if(argc[5] == "daily")
            {
                _type = meeting_type::DAILY;
            }
            else if(argc[5] == "period")
            {
                _type = meeting_type::PERIOD;
            }
            else if(argc[5] == "interview")
            {
                _type = meeting_type::INTERVIEW;
            }
            else if(argc[5] == "corporative")
            {
                _type = meeting_type::CORPORATIVE;
            }
            else
            {
                return false;
            }

            if (argc[6] == "online")
            {
                _form = format::ONLINE;
            }
            else if (argc[6] == "offline")
            {
                _form = format::OFFLINE;
            }
            else
            {
                return false;
            }

            _description = argc[7];
            _link = argc[8];
            _creator_surname = argc[9];
            _creator_name = argc[10];
            _creator_patronymic = argc[11];

            if (_date_validator(argc[12]))
            {
                _date = argc[12];
            }
            else
            {
                return false;
            }

            if (_time_validator(argc[13]))
            {
                _start_time = argc[13];
            }
            else
            {
                return false;
            }

            if (_digit_validator(argc[14]))
            {
                _min_duration = argc[14];
            }
            else
            {
                return false;
            }

            _invited_people = argc[15];

            return true;
        }
    }

    return false;
}

void command_update_data::execute(const std::string &request) const noexcept
{
    key * data_key = new key();

    data_key->set_id(_id_meeting);

    data_base::get_instance()->update_data(
        _pool_name,
        _scheme_name,
        _collection_name,
        data_key,
        _type,
        _form,
        _description,
        _link,
        _creator_surname,
        _creator_name,
        _creator_patronymic,
        _date,
        _start_time,
        _min_duration,
        _invited_people);

    delete data_key;
}