#ifndef COMMAND_GET_DATA_BETWEEN_H
#define COMMAND_GET_DATA_BETWEEN_H

#include "command.h"
#include "../containers/data_base.h"

class command_get_data_between : public command
{
private:

    digit_validator _digit_validator;

    std::string _pool_name;
    std::string _scheme_name;
    std::string _collection_name;
    unsigned int _id_meeting_first;
    unsigned int _id_meeting_second;

public:

    bool can_execute(std::string const &request) noexcept final;

    void execute(std::string const &request) const noexcept final;

};

#endif //COMMAND_GET_DATA_BETWEEN_H