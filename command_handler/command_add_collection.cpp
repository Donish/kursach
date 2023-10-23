#include "command_add_collection.h"

bool command_add_collection::can_execute(const std::string &request) noexcept
{
    if (request.rfind("ADD_COLLECTION", 0) == 0)
    {
        auto argc = split(request, ' ');

        if (argc.size() == 5)
        {
            _pool_name = argc[1];
            _scheme_name = argc[2];
            _collection_name = argc[3];

            if(argc[4] == "splay_tree")
            {
                _tree_type = tree_types::SPLAY_TREE;
            }
            else if(argc[4] == "avl_tree")
            {
                _tree_type = tree_types::AVL_TREE;
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

void command_add_collection::execute(const std::string &request) const noexcept
{
    data_base::get_instance()->add_collection(_pool_name, _scheme_name, _collection_name, _tree_type);
}
