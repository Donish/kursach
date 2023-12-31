cmake_minimum_required(VERSION 3.26)
project(kursach)

set(CMAKE_CXX_STANDARD 20)

add_executable(kursach
        main.cpp
        comparers/key_comparer.cpp
        containers/data_base.cpp
        comparers/string_comparer.cpp
        allocator/borders_descriptors_memory.cpp
        allocator/buddy_system_memory.cpp
        allocator/memory.cpp
        allocator/memory_simple.cpp
        allocator/sorted_list_memory.cpp
        command_handler/command.cpp
        command_handler/command_add_collection.cpp
        command_handler/command_add_data.cpp
        command_handler/command_add_pool.cpp
        command_handler/command_add_scheme.cpp
        command_handler/command_get_data.cpp
        command_handler/command_get_data_between.cpp
        command_handler/command_remove_collection.cpp
        command_handler/command_remove_data.cpp
        command_handler/command_remove_pool.cpp
        command_handler/command_remove_scheme.cpp
        command_handler/command_update_data.cpp
        command_handler/handler_with_command.cpp
        command_handler/handler_with_command_chain.cpp
        containers/scheme_data.cpp
        containers/collection_data.cpp
        containers/pool_data.cpp
        logger/logger.cpp
        logger/logger_builder.cpp
        logger/logger_builder_concrete.cpp
        logger/logger_concrete.cpp
        validator/date_validator.cpp
        validator/digit_validator.cpp
        validator/time_validator.cpp
        allocator/allocator_types.h
        command_handler/handler.h
        owntype/own_type.h
        tree/associative_container.h
        tree/binary_search_tree.h
        tree/splay_tree.h
        allocator/memory_holder.cpp
        allocator/memory_holder.h
        logger/logger_holder.cpp
        logger/logger_holder.h
        tree/avl_tree.h
#        tree/red_black_tree.h
        tree/tree_types.h)
