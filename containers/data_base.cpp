#include <cmath>
#include <iostream>
#include "data_base.h"
#include "../command_handler/handler_with_command_chain.h"
#include "../command_handler/command_add_pool.h"
#include "../command_handler/command_remove_pool.h"
#include "../command_handler/command_add_scheme.h"
#include "../command_handler/command_remove_scheme.h"
#include "../command_handler/command_add_collection.h"
#include "../command_handler/command_remove_collection.h"
#include "../command_handler/command_add_data.h"
#include "../command_handler/command_get_data.h"
#include "../command_handler/command_get_data_between.h"
#include "../command_handler/command_update_data.h"
#include "../command_handler/command_remove_data.h"

data_base *data_base::_instance = nullptr;

data_base::data_base()
{
    _data_base = new splay_tree<std::string, pool_data, string_comparer>();

    _chain
        .add_handler(new command_add_pool())
        .add_handler(new command_remove_pool())
        .add_handler(new command_add_scheme())
        .add_handler(new command_remove_scheme())
        .add_handler(new command_add_collection())
        .add_handler(new command_remove_collection())
        .add_handler(new command_add_data())
        .add_handler(new command_remove_data())
        .add_handler(new command_get_data())
        .add_handler(new command_get_data_between())
        .add_handler(new command_update_data());

    _instance = this;
}

data_base::~data_base()
{
    delete _data_base;
}

void data_base::print_meeting_type(value const &data) const
{
    if(data.get_meeting_type() == meeting_type::DAILY)
    {
        std::cout << "daily";
    }
    else if(data.get_meeting_type() == meeting_type::PERIOD)
    {
        std::cout << "period";
    }
    else if(data.get_meeting_type() == meeting_type::INTERVIEW)
    {
        std::cout << "interview";
    }
    else if(data.get_meeting_type() == meeting_type::CORPORATIVE)
    {
        std::cout << "corporative";
    }
}

void data_base::print_format(value const &data) const
{
    if(data.get_format() == format::ONLINE)
    {
        std::cout << "online";
    }
    else if(data.get_format() == format::OFFLINE)
    {
        std::cout << "offline";
    }
}

void data_base::add_pool(std::string const &pool_name, allocator_types allocator_name, size_t request_size, memory::allocation_mode mode, tree_types outer_tree_type)
{
    try
    {
        memory *allocator = nullptr;

        switch (allocator_name) {
            case allocator_types::GLOBAL_HEAP:
                allocator = new memory_simple();
                break;

            case allocator_types::SORTED_LIST:
                allocator = new sorted_list_memory(request_size, mode);
                break;

            case allocator_types::BORDER_DESCRIPTORS:
                allocator = new borders_descriptors_memory(request_size, mode);
                break;

            case allocator_types::BUDDIES_SYSTEM:
                request_size = static_cast<size_t>(log2(request_size)) + 1;
                allocator = new buddy_system_memory(request_size, mode);
                break;
        }

        _data_base->insert(pool_name, std::move(pool_data(allocator, outer_tree_type)));

        std::cout << "[DATA BASE] pool with name:" << pool_name << " added to data base" << std::endl << std::endl;
    }
    catch (std::exception const &ex)
    {
        std::cout << ex.what() << std::endl;
    }
}

void data_base::add_scheme(std::string const &pool_name, std::string const &scheme_name, tree_types outer_tree_type)
{
    try
    {
        if (_data_base->find_in(pool_name))
        {
            const_cast<pool_data&>(_data_base->find(pool_name)).add(scheme_name, std::move(scheme_data(outer_tree_type)));
            std::cout << "[DATA BASE] scheme with name:" << scheme_name << " added to " << pool_name << std::endl << std::endl;
        }
        else
        {
            std::cout << "[DATA BASE] pool with name:" << pool_name << " not found" << std::endl << std::endl;
        }
    }
    catch (std::exception const &ex)
    {
        std::cout << ex.what() << std::endl;
    }
}

void data_base::add_collection(std::string const &pool_name, std::string const &scheme_name, std::string const &collection_name, tree_types outer_tree_type)
{
    try
    {
        if (_data_base->find_in(pool_name))
        {
            pool_data const &current_pool = _data_base->find(pool_name);

            memory *allocator = current_pool.get_allocator();

            if (current_pool.find_in(scheme_name))
            {
                scheme_data const &current_scheme = current_pool.find(scheme_name);

                const_cast<scheme_data&>(current_scheme).add(collection_name, std::move(collection_data(allocator, outer_tree_type)));

                std::cout << "[DATA BASE] collection with name:" << collection_name << " added to " << pool_name << " " << scheme_name << std::endl << std::endl;
            }
            else
            {
                std::cout << "[DATA BASE] scheme with name:" << scheme_name << " not found" << std::endl << std::endl;
            }
        }
        else
        {
            std::cout << "[DATA BASE] pool with name:" << pool_name << " not found" << std::endl << std::endl;
        }
    }
    catch (std::exception const &ex)
    {
        std::cout << ex.what() << std::endl;
    }
}

void data_base::add_data(
        const std::string &pool_name,
        const std::string &scheme_name,
        const std::string &collection_name,
        unsigned int id_meeting,
        meeting_type type,
        format form,
        const std::string &description,
        const std::string &link,
        const std::string &creator_surname,
        const std::string &creator_name,
        const std::string &creator_patronymic,
        const std::string &date,
        const std::string &start_time,
        const std::string &min_duration,
        const std::string &invited_people)
{
    try
    {
        if (_data_base->find_in(pool_name))
        {
            pool_data const &current_pool = _data_base->find(pool_name);

            if (current_pool.find_in(scheme_name))
            {
                scheme_data const &current_scheme = current_pool.find(scheme_name);

                if (current_scheme.find_in(collection_name))
                {
                    const_cast<collection_data&>(current_scheme.find(collection_name)).add(id_meeting, type, form, description, link, creator_surname, creator_name, creator_patronymic, date, start_time, min_duration, invited_people);

                    std::cout << "[DATA BASE] data added to " << pool_name << " " << scheme_name << " " << collection_name << std::endl << std::endl;
                }
                else
                {
                    std::cout << "[DATA BASE] collection with name:" << collection_name << " not found" << std::endl << std::endl;
                }
            }
            else
            {
                std::cout << "[DATA BASE] scheme with name:" << scheme_name << " not found" << std::endl << std::endl;
            }
        }
        else
        {
            std::cout << "[DATA BASE] pool with name:" << pool_name << " not found" << std::endl << std::endl;
        }
    }
    catch (std::exception const &ex)
    {
        std::cout << ex.what() << std::endl;
    }
}

void data_base::remove_pool(const std::string &pool_name)
{
    try
    {
        if (_data_base->find_in(pool_name))
        {
            _data_base->remove(pool_name);

            std::cout << "[DATA BASE] pool with name:" << pool_name << " removed from data base" << std::endl << std::endl;
        }
        else
        {
            std::cout << "[DATA BASE] pool with name:" << pool_name << " not found" << std::endl << std::endl;
        }
    }
    catch (std::exception const &ex)
    {
        std::cout << ex.what() << std::endl;
    }
}

void data_base::remove_scheme(const std::string &pool_name, const std::string &scheme_name)
{
    try
    {
        if (_data_base->find_in(pool_name))
        {
            pool_data const &current_pool = _data_base->find(pool_name);

            if (current_pool.find_in(scheme_name))
            {
                const_cast<pool_data&>(current_pool).remove(scheme_name);

                std::cout << "[DATA BASE] scheme with name:" << scheme_name << " removed from " << pool_name << std::endl << std::endl;
            }
            else
            {
                std::cout << "[DATA BASE] scheme with name:" << scheme_name << " not found" << std::endl << std::endl;
            }
        }
        else
        {
            std::cout << "[DATA BASE] pool with name:" << pool_name << " not found" << std::endl << std::endl;
        }
    }
    catch (std::exception const &ex)
    {
        std::cout << ex.what() << std::endl;
    }
}

void data_base::remove_collection(const std::string &pool_name, const std::string &scheme_name, const std::string &collection_name)
{
    try
    {
        if (_data_base->find_in(pool_name))
        {
            pool_data const &current_pool = _data_base->find(pool_name);

            if (current_pool.find_in(scheme_name))
            {
                scheme_data const &current_scheme = current_pool.find(scheme_name);

                if (current_scheme.find_in(collection_name))
                {
                    const_cast<scheme_data&>(current_scheme).remove(collection_name);

                    std::cout << "[DATA BASE] collection with name:" << collection_name << " removed from " << pool_name << " " << scheme_name << std::endl << std::endl;
                }
                else
                {
                    std::cout << "[DATA BASE] collection with name:" << collection_name << " not found" << std::endl << std::endl;
                }
            }
            else
            {
                std::cout << "[DATA BASE] scheme with name:" << scheme_name << " not found" << std::endl << std::endl;
            }
        }
        else
        {
            std::cout << "[DATA BASE] pool with name:" << pool_name << " not found" << std::endl << std::endl;
        }
    }
    catch (std::exception const &ex)
    {
        std::cout << ex.what() << std::endl;
    }
}

void data_base::remove_data(
        const std::string &pool_name,
        const std::string &scheme_name,
        const std::string &collection_name,
        key *data_key)
{
    try
    {
        if (_data_base->find_in(pool_name))
        {
            pool_data const &current_pool = _data_base->find(pool_name);

            if (current_pool.find_in(scheme_name))
            {
                scheme_data const &current_scheme = current_pool.find(scheme_name);

                if (current_scheme.find_in(collection_name))
                {
                    collection_data const &current_collection = current_scheme.find(collection_name);

                    if (current_collection.find_in(data_key))
                    {
                        const_cast<collection_data&>(current_collection).remove(data_key);

                        std::cout << "[DATA BASE] data removed from " << pool_name << " " << scheme_name << " " << collection_name << std::endl << std::endl;
                    }
                    else
                    {
                        std::cout << "[DATA BASE] data_key not found" << std::endl << std::endl;
                    }
                }
                else
                {
                    std::cout << "[DATA BASE] collection with name:" << collection_name << " not found" << std::endl << std::endl;
                }
            }
            else
            {
                std::cout << "[DATA BASE] scheme with name:" << scheme_name << " not found" << std::endl << std::endl;
            }
        }
        else
        {
            std::cout << "[DATA BASE] pool with name:" << pool_name << " not found" << std::endl << std::endl;
        }
    }
    catch (std::exception const &ex)
    {
        std::cout << ex.what() << std::endl;
    }
}

void data_base::get_data(
        const std::string &pool_name,
        const std::string &scheme_name,
        const std::string &collection_name,
        key* const &data_key)
{
    try
    {
        if (_data_base->find_in(pool_name))
        {
            pool_data const &current_pool = _data_base->find(pool_name);

            if (current_pool.find_in(scheme_name))
            {
                scheme_data const &current_scheme = current_pool.find(scheme_name);

                if (current_scheme.find_in(collection_name))
                {
                    collection_data const &current_collection = current_scheme.find(collection_name);

                    if (current_collection.find_in(data_key))
                    {
                        value *data = const_cast<collection_data&>(current_collection).get(data_key);

                        std::cout << "[DATA BASE] received value from " << pool_name << " " << scheme_name << " " << collection_name << std::endl << std::endl;

                        std::cout << "\tData" << std::endl;
                        std::cout << "meeting type: ";
                        print_meeting_type(*data);
                        std::cout << std::endl;
                        std::cout << "meeting format: ";
                        print_format(*data);
                        std::cout << std::endl;
                        std::cout << "description: " << data->get_description()->get_value() << std::endl;
                        std::cout << "link: " << data->get_link()->get_value() << std::endl;
                        std::cout << "creator's surname: " << data->get_creator_surname()->get_value() << std::endl;
                        std::cout << "creator's name: " << data->get_creator_name()->get_value() << std::endl;
                        std::cout << "creator's patronymic: " << data->get_creator_patronymic()->get_value() << std::endl;
                        std::cout << "date: " << data->get_date()->get_value() << std::endl;
                        std::cout << "start time: " << data->get_start_time()->get_value() << std::endl;
                        std::cout << "minimal duration: " << data->get_min_duration()->get_value() << " min" << std::endl;
                        std::cout << "invited people: " << data->get_invited_people()->get_value() << std::endl << std::endl;
                    }
                    else
                    {
                        std::cout << "[DATA BASE] data_key not found" << std::endl << std::endl;
                    }
                }
                else
                {
                    std::cout << "[DATA BASE] collection with name:" << collection_name << " not found" << std::endl << std::endl;
                }
            }
            else
            {
                std::cout << "[DATA BASE] scheme with name:" << scheme_name << " not found" << std::endl << std::endl;
            }
        }
        else
        {
            std::cout << "[DATA BASE] pool with name:" << pool_name << " not found" << std::endl << std::endl;
        }
    }
    catch (std::exception const &ex)
    {
        std::cout << ex.what() << std::endl;
    }
}

void data_base::get_data_between_keys(
        const std::string &pool_name,
        const std::string &scheme_name,
        const std::string &collection_name,
        key* const &min_key,
        key* const &max_key)
{
    try
    {
        if (_data_base->find_in(pool_name))
        {
            pool_data const &current_pool = _data_base->find(pool_name);

            if (current_pool.find_in(scheme_name))
            {
                scheme_data const &current_scheme = current_pool.find(scheme_name);

                if (current_scheme.find_in(collection_name))
                {
                    std::vector<value*> data = const_cast<collection_data&>(current_scheme.find(collection_name)).get_between_keys(min_key, max_key);

                    std::cout << "[DATA BASE] received values from " << pool_name << " " << scheme_name << " " << collection_name << std::endl << std::endl;

                    int index = 1;

                    for (auto const & data_value : data)
                    {
                        std::cout << "\tData:" << index << std::endl;

                        //TODO: разыменовать указатели
                        print_meeting_type(*data_value);
                        std::cout << std::endl;
                        print_format(*data_value);
                        std::cout << std::endl;
                        std::cout << "description: " << data_value->get_description()->get_value() << std::endl;
                        std::cout << "link: " << data_value->get_link()->get_value() << std::endl;
                        std::cout << "creator's surname: " << data_value->get_creator_surname()->get_value() << std::endl;
                        std::cout << "creator's name: " << data_value->get_creator_name()->get_value() << std::endl;
                        std::cout << "creator's patronymic: " << data_value->get_creator_patronymic()->get_value() << std::endl;
                        std::cout << "date: " << data_value->get_date()->get_value() << std::endl;
                        std::cout << "start time: " << data_value->get_start_time()->get_value() << std::endl;
                        std::cout << "minimal duartion: " << data_value->get_min_duration()->get_value() << std::endl;
                        std::cout << "invited people: " << data_value->get_invited_people()->get_value() << std::endl << std::endl;

                        index++;
                    }
                }
                else
                {
                    std::cout << "[DATA BASE] collection with name:" << collection_name << " not found" << std::endl << std::endl;
                }
            }
            else
            {
                std::cout << "[DATA BASE] scheme with name:" << scheme_name << " not found" << std::endl << std::endl;
            }
        }
        else
        {
            std::cout << "[DATA BASE] pool with name:" << pool_name << " not found" << std::endl << std::endl;
        }
    }
    catch (std::exception const &ex)
    {
        std::cout << ex.what() << std::endl;
    }
}

void data_base::update_data(
        const std::string &pool_name,
        const std::string &scheme_name,
        const std::string &collection_name,
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
        std::string const &invited_people)
{
    try
    {
        if (_data_base->find_in(pool_name))
        {
            pool_data const &current_pool = _data_base->find(pool_name);

            if (current_pool.find_in(scheme_name))
            {
                scheme_data const &current_scheme = current_pool.find(scheme_name);

                if (current_scheme.find_in(collection_name))
                {
                    collection_data const &current_collection = current_scheme.find(collection_name);

                    if (current_collection.find_in(data_key))
                    {
                        const_cast<collection_data&>(current_collection).update(data_key, type, form, description, link, creator_surname, creator_name, creator_patronymic, date, start_time, min_duration, invited_people);

                        std::cout << "[DATA BASE] " << "data from " << pool_name << " " << scheme_name << " " << collection_name <<  " updated" << std::endl << std::endl;
                    }
                    else
                    {
                        std::cout << "[DATA BASE] data_key not found" << std::endl << std::endl;
                    }
                }
                else
                {
                    std::cout << "[DATA BASE] collection with name:" << collection_name << " not found" << std::endl << std::endl;
                }
            }
            else
            {
                std::cout << "[DATA BASE] scheme with name:" << scheme_name << " not found" << std::endl << std::endl;
            }
        }
        else
        {
            std::cout << "[DATA BASE] pool with name:" << pool_name << " not found" << std::endl << std::endl;
        }
    }
    catch (std::exception const &ex)
    {
        std::cout << ex.what() << std::endl;
    }
}

data_base *data_base::get_instance()
{
    if (_instance == nullptr)
    {
        _instance = new data_base();
    }

    return _instance;
}

void data_base::handle_request(
        std::string const &request)
{

    //TODO: добавить проверку на наличие /r: если есть, то удалить его

    if(!_chain.handle(request))
    {
        std::cout << "[DATA BASE] command can't be executed" << std::endl << std::endl;
        throw std::invalid_argument("invalid command!");
    }

}