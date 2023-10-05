#include "pool_data.h"

pool_data::pool_data(memory *allocator, tree_types outer_tree_type):
//    _pool_data(new splay_tree<std::string, scheme_data, string_comparer>()),
    _allocator(allocator),
    _tree_type(outer_tree_type)
{
    if(_tree_type == tree_types::SPLAY_TREE)
    {
        _pool_data = new splay_tree<std::string, scheme_data, string_comparer>();
    }
    else if(_tree_type == tree_types::AVL_TREE)
    {
        _pool_data = new avl_tree<std::string, scheme_data, string_comparer>();
    }
    else if(_tree_type == tree_types::RED_BLACK_TREE)
    {
//        _pool_data = new red_black_tree<std::string, scheme_data, string_comparer>();
    }
}

void pool_data::add(std::string const &key, scheme_data &&target)
{
    _pool_data->insert(key, std::move(target));
}

void pool_data::remove(std::string const &key)
{
    _pool_data->remove(key);
}

scheme_data const& pool_data::find(std::string const& scheme_data_name) const
{
    return _pool_data->find(scheme_data_name);
}

bool pool_data::find_in(std::string const &scheme_data_name) const
{
    return _pool_data->find_in(scheme_data_name);
}

memory* pool_data::get_allocator() const
{
    return _allocator;
}

pool_data::pool_data(pool_data const &other):
//    _pool_data(new splay_tree<std::string, scheme_data, string_comparer>(*reinterpret_cast<splay_tree<std::string, scheme_data, string_comparer>*>(other._pool_data))),
    _allocator(other._allocator),
    _tree_type(other._tree_type)
{
    if(other._tree_type == tree_types::SPLAY_TREE)
    {
        _pool_data = new splay_tree<std::string, scheme_data, string_comparer>(*reinterpret_cast<splay_tree<std::string, scheme_data, string_comparer>*>(other._pool_data));
    }
    else if(other._tree_type == tree_types::AVL_TREE)
    {
        _pool_data = new avl_tree<std::string, scheme_data, string_comparer>(*reinterpret_cast<avl_tree<std::string, scheme_data, string_comparer>*>(other._pool_data));
    }
    else if(other._tree_type == tree_types::RED_BLACK_TREE)
    {
//        _pool_data = new red_black_tree<std::string, scheme_data, string_comparer>(*reinterpret_cast<red_black_tree<std::string, scheme_data, string_comparer>*>(other._pool_data));
    }
}

pool_data& pool_data::operator=(pool_data const &other)
{
    if(this == &other)
    {
        return *this;
    }

    delete this->_pool_data;

    if(this->_allocator != other._allocator)
    {
        delete this->_allocator;
        this->_allocator = other._allocator;
    }

    if(other._tree_type == tree_types::SPLAY_TREE)
    {
        this->_pool_data = new splay_tree<std::string, scheme_data, string_comparer>(*reinterpret_cast<splay_tree<std::string, scheme_data, string_comparer>*>(other._pool_data));
    }
    else if(other._tree_type == tree_types::AVL_TREE)
    {
        this->_pool_data = new avl_tree<std::string, scheme_data, string_comparer>(*reinterpret_cast<avl_tree<std::string, scheme_data, string_comparer>*>(other._pool_data));
    }
    else if(other._tree_type == tree_types::RED_BLACK_TREE)
    {
//        this->_pool_data = new red_black_tree<std::string, scheme_data, string_comparer>(*reinterpret_cast<red_black_tree<std::string, scheme_data, string_comparer>*>(other._pool_data));
    }
    this->_tree_type = other._tree_type;

    return *this;
}

pool_data::pool_data(pool_data &&other) noexcept
{
    this->_pool_data = other._pool_data;
    this->_allocator = other._allocator;
    this->_tree_type = other._tree_type;

    other._pool_data = nullptr;
    other._allocator = nullptr;
}

pool_data& pool_data::operator=(pool_data &&other) noexcept
{
    if(this == &other)
    {
        return *this;
    }

    delete this->_pool_data;
    delete this->_allocator;

    this->_pool_data = other._pool_data;
    this->_allocator = other._allocator;
    this->_tree_type = other._tree_type;

    other._pool_data = nullptr;
    other._allocator = nullptr;

    return *this;
}

pool_data::~pool_data()
{
    delete _pool_data;
}