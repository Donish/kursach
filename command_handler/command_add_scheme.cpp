#include "command_add_scheme.h"

bool command_add_scheme::can_execute(const std::string &request) noexcept
{
    if (request.rfind("ADD_SCHEME", 0) == 0)
    {
        auto argc = split(request, ' ');

        if (argc.size() == 4)
        {
            _pool_name = argc[1];
            _scheme_name = argc[2];

            if(argc[3] == "splay_tree")
            {
                _tree_type = tree_types::SPLAY_TREE;
            }
            else if(argc[3] == "avl_tree")
            {
                _tree_type = tree_types::AVL_TREE;
            }
            else if(argc[3] == "black_red_tree")
            {
                _tree_type = tree_types::RED_BLACK_TREE;
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

void command_add_scheme::execute(const std::string &request) const noexcept
{
    data_base::get_instance()->add_scheme(_pool_name, _scheme_name);
}