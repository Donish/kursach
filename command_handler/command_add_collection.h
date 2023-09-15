#ifndef COMMAND_ADD_COLLECTION_H
#define COMMAND_ADD_COLLECTION_H

#include "command.h"
#include "../containers/data_base.h"

class command_add_collection final : public command
{
private:

    std::string _pool_name;
    std::string _scheme_name;
    std::string _collection_name;
    tree_types _tree_type;

public:

    bool can_execute(std::string const &request) noexcept final;

    void execute(std::string const &request) const noexcept final;

};

#endif //COMMAND_ADD_COLLECTION_H