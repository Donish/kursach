#ifndef DATA_BASE_H
#define DATA_BASE_H

#include "pool_data.h"
#include "../allocator/allocator_types.h"
#include "../command_handler/handler_with_command_chain.h"
#include "../string_methods/mystring.h"

class data_base
{

private:

    static data_base * _instance;

private:

    handler_with_command_chain _chain;

private:

    associative_container<std::string, pool_data>  * _data_base;

public:

    data_base();

    ~data_base();

private:

    void print_meeting_type(value const &data) const;

    void print_format(value const &data) const;

public:

    static data_base *get_instance();

public:

    void add_pool(std::string const &name_pool, allocator_types allocator_name, size_t request_size, memory::allocation_mode mode, tree_types outer_tree_type);

    void add_scheme(std::string const &pool_name, std::string const &scheme_name, tree_types outer_tree_type);

    void add_collection(std::string const &pool_name, std::string const &scheme_name, std::string const &collection_name, tree_types outer_tree_type);

    void add_data(std::string const &pool_name,
        std::string const &scheme_name,
        std::string const &collection_name,
        unsigned int id_meeting,
        meeting_type type,
        format form,
        std::string const &description,
        std::string const &link,
        std::string const &creator_surname,
        std::string const &creator_name,
        std::string const &creator_patronymic,
        std::string const &date,
        std::string const &start_time,
        std::string const &min_duration,
        std::string const &invited_people);

    void remove_pool(std::string const &name);

    void remove_scheme(std::string const &pool_name, std::string const &scheme_name);

    void remove_collection(std::string const &pool_name, std::string const &scheme_name, std::string const &collection_name);

    void remove_data(
            std::string const &pool_name,
            std::string const &scheme_name,
            std::string const &collection_name,
            key *data_key);

    void update_data(
            std::string const &pool_name,
            std::string const &scheme_name,
            std::string const &collection_name,
            key* const &data_key,
            meeting_type type,
            format form,
            std::string const &description,
            std::string const &link,
            std::string const &creator_surname,
            std::string const &creator_name,
            std::string const &creator_patronymic,
            std::string const &date,
            std::string const &start_time,
            std::string const &min_duration,
            std::string const &invited_people);

    void get_data(std::string const &pool_name, std::string const &scheme_name, std::string const &collection_name, key* const &data_key);

    void get_data_between_keys(std::string const &pool_name, std::string const &scheme_name, std::string const &collection_name, key* const &min_key, key* const &max_key);

public:

    void handle_request(std::string const &request);

public:

    data_base(data_base const &other) = delete;

    data_base &operator=(data_base const &other) = delete;

    data_base(data_base &&other) = delete;

    data_base &operator=(data_base &&other) = delete;

};

#endif //DATA_BASE_H