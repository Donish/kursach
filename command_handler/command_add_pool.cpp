#include "command_add_pool.h"

bool command_add_pool::can_execute(const std::string &request) noexcept
{
    if (request.rfind("ADD_POOL", 0) == 0)
    {
        auto argc = split(request, ' ');

        if (argc.size() == 4 && argc[2] == "global_heap")
        {
            _pool_name = argc[1];
            _pool_allocator_type = allocator_types::GLOBAL_HEAP;
            _pool_allocator_size = 0;
            _pool_allocator_allocation_mode = memory::allocation_mode::first_match;

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

        if (argc.size() == 6)
        {
            _pool_name = argc[1];

            if (argc[2] == "sorted_list")
            {
                _pool_allocator_type = allocator_types::SORTED_LIST;
            }
            else if (argc[2] == "border_descriptors")
            {
                _pool_allocator_type = allocator_types::BORDER_DESCRIPTORS;
            }
            else if (argc[2] == "buddies_system")
            {
                _pool_allocator_type = allocator_types::BUDDIES_SYSTEM;
            }
            else
            {
                return false;
            }

            if (_digit_validator(argc[3]))
            {
                std::stringstream size(argc[3]);
                size >> _pool_allocator_size;
            }
            else
            {
                return false;
            }

            if (argc[4] == "first_match")
            {
                _pool_allocator_allocation_mode = memory::allocation_mode::first_match;
            }
            else if (argc[4] == "the_best_match")
            {
                _pool_allocator_allocation_mode = memory::allocation_mode::the_best_match;
            }
            else if (argc[4] == "the_worst_match")
            {
                _pool_allocator_allocation_mode = memory::allocation_mode::the_worst_match;
            }
            else
            {
                return false;
            }

            if(argc[5] == "splay_tree")
            {
                _tree_type = tree_types::SPLAY_TREE;
            }
            else if(argc[5] == "avl_tree")
            {
                _tree_type = tree_types::AVL_TREE;
            }
            else if(argc[5] == "black_red_tree")
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

void command_add_pool::execute(const std::string &request) const noexcept
{
    data_base::get_instance()->add_pool(_pool_name, _pool_allocator_type, _pool_allocator_size, _pool_allocator_allocation_mode, _tree_type);
}