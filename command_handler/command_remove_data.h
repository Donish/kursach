#ifndef COMMAND_REMOVE_DATA_H
#define COMMAND_REMOVE_DATA_H

#include "command.h"
#include "../containers/data_base.h"

class command_remove_data : public command
{
private:

    digit_validator _digit_validator;

    std::string _pool_name;
    std::string _scheme_name;
    std::string _collection_name;
    unsigned int _id_meeting;

public:

    bool can_execute(std::string const &request) noexcept final;

    void execute(std::string const &request) const noexcept final;

};

#endif //COMMAND_REMOVE_DATA_H