#include "collection_data.h"

collection_data::collection_data(memory *allocator, tree_types outer_tree_type):
//    _data(new splay_tree<key*, value*, key_comparer>(allocator, nullptr)),
    _tree_type(outer_tree_type),
    _allocator(allocator)
{
    if(_tree_type == tree_types::SPLAY_TREE)
    {
        _data = new splay_tree<key*, value*, key_comparer>(allocator, nullptr);
    }
    else if(_tree_type == tree_types::AVL_TREE)
    {
        _data = new avl_tree<key*, value*, key_comparer>(allocator, nullptr);
    }
    else if(_tree_type == tree_types::RED_BLACK_TREE)
    {
//        _data = new red_black_tree<key*, value*, key_comparer>(allocator, nullptr);
    }
}

void collection_data::add(
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
    key *data_key = reinterpret_cast<key*>(_allocator->allocate(sizeof(key)));
    new (data_key) key;

    data_key->_id_meeting = id_meeting;
    
    value *data_value = reinterpret_cast<value*>(_allocator->allocate(sizeof(value)));
    new (data_value) value;

    data_value->_type = type;
    data_value->_format = form;
    data_value->_description = description;
    data_value->_link = link;
    data_value->_creator_surname = creator_surname;
    data_value->_creator_name = creator_name;
    data_value->_creator_patronymic = creator_patronymic;
    data_value->_date = date;
    data_value->_start_time = start_time;
    data_value->_min_duration = min_duration;
    data_value->_invited_people = invited_people;

    _data->insert(data_key, std::move(data_value));
}

value* collection_data::get(key* const &data_key)
{
    return _data->get_value(data_key);
}

value* collection_data::remove(key* data_key)
{
    return _data->remove(data_key);
}

bool collection_data::find_in(key* const &data_key) const
{
    return _data->find_in(data_key);
}

std::vector<value*> collection_data::get_between_keys(key* const &min_bound, key* const &max_bound)
{
    return _data->find_in_range(min_bound, max_bound);
}

void collection_data::update(
    key* const &data_key,
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
    value* data_value = reinterpret_cast<value*>(_allocator->allocate(sizeof(value)));
    new (data_value) value;

    data_value->_type = type;
    data_value->_format = form;
    data_value->_description = description;
    data_value->_link = link;
    data_value->_creator_surname = creator_surname;
    data_value->_creator_name = creator_name;
    data_value->_creator_patronymic = creator_patronymic;
    data_value->_date = date;
    data_value->_start_time = start_time;
    data_value->_min_duration = min_duration;
    data_value->_invited_people = invited_people;

    _data->update(data_key, std::move(data_value));
}

collection_data::collection_data(collection_data const &other):
//    _data(new splay_tree<key*, value*, key_comparer>(*reinterpret_cast<splay_tree<key*, value*, key_comparer>*>(other._data))),
    _allocator(other._allocator),
    _tree_type(other._tree_type)
{
    if(other._tree_type == tree_types::SPLAY_TREE)
    {
        _data = new splay_tree<key*, value*, key_comparer>(*reinterpret_cast<splay_tree<key*, value*, key_comparer>*>(other._data));
    }
    else if(other._tree_type == tree_types::AVL_TREE)
    {
        _data = new avl_tree<key*, value*, key_comparer>(*reinterpret_cast<avl_tree<key*, value*, key_comparer>*>(other._data));
    }
    else if(other._tree_type == tree_types::RED_BLACK_TREE)
    {
//        _data = new red_black_tree<key*, value*, key_comparer>(*reinterpret_cast<red_black_tree<key*, value*, key_comparer>*>(other._data));
    }
}

collection_data& collection_data::operator=(collection_data const &other)
{
    if(this == &other)
    {
        return *this;
    }

    delete _data;

    if(_allocator != other._allocator)
    {
        delete this->_allocator;
        this->_allocator = other._allocator;
    }

    if(other._tree_type == tree_types::SPLAY_TREE)
    {
        this->_data = new splay_tree<key *, value *, key_comparer>(*reinterpret_cast<splay_tree<key *, value *, key_comparer>*>(other._data));
    }
    else if(other._tree_type == tree_types::AVL_TREE)
    {
        this->_data = new avl_tree<key*, value*, key_comparer>(*reinterpret_cast<avl_tree<key*, value*, key_comparer>*>(other._data));
    }
    else if(other._tree_type == tree_types::RED_BLACK_TREE)
    {
//        this->_data = new red_black_tree<key*, value*, key_comparer>(*reinterpret_cast<red_black_tree<key*, value*, key_comparer>*>(other._data));
    }
    this->_tree_type = other._tree_type;

    return *this;
}

collection_data::collection_data(collection_data &&other) noexcept
{
    this->_data = other._data;
    this->_allocator = other._allocator;
    this->_tree_type = other._tree_type;
    other._data = nullptr;
    other._allocator = nullptr;
}

collection_data& collection_data::operator=(collection_data &&other) noexcept
{
    if(this == &other)
    {
        return *this;
    }

    delete this->_data;
    delete this->_allocator;

    this->_data = other._data;
    this->_allocator = other._allocator;
    this->_tree_type = other._tree_type;

    other._data = nullptr;
    other._allocator = nullptr;

    return *this;
}

memory* collection_data::get_allocator() const
{
    return _allocator;
}

collection_data::~collection_data()
{
    delete _data;
}