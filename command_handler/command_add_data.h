#ifndef COMMAND_ADD_DATA_H
#define COMMAND_ADD_DATA_H

#include "command.h"
#include "../containers/data_base.h"

class command_add_data final : public command
{

private:

    digit_validator _digit_validator;
    date_validator _date_validator;
    time_validator _time_validator;

    std::string _pool_name;
    std::string _scheme_name;
    std::string _collection_name;
    unsigned int _id_meeting;
    meeting_type _type;
    format _form;
    std::string _description;
    std::string _link;
    std::string _creator_surname;
    std::string _creator_name;
    std::string _creator_patronymic;
    std::string _date;
    std::string _start_time;
    std::string _min_duration;
    std::string _invited_people;

public:

    bool can_execute(std::string const &request) noexcept final;

    void execute(std::string const &request) const noexcept final;

};
#endif //COMMAND_ADD_DATA_H