#include "scheme_data.h"

scheme_data::scheme_data(tree_types outer_tree_type):
//    _scheme_data(new splay_tree<std::string, collection_data, string_comparer>()),
    _tree_type(outer_tree_type)
{
    if(_tree_type == tree_types::SPLAY_TREE)
    {
        _scheme_data = new splay_tree<std::string, collection_data, string_comparer>();
    }
    else if(_tree_type == tree_types::AVL_TREE)
    {
        _scheme_data = new avl_tree<std::string, collection_data, string_comparer>();
    }
    else if(_tree_type == tree_types::RED_BLACK_TREE)
    {
//        _scheme_data = new red_black_tree<std::string, collection_data, string_comparer>();
    }
}

void scheme_data::add(std::string const &key, collection_data &&target)
{
    _scheme_data->insert(key, std::move(target));
}

void scheme_data::remove(std::string const &key)
{
    _scheme_data->remove(key);
}

collection_data const& scheme_data::find(std::string const &collection_name) const
{
    return _scheme_data->find(collection_name);
}

bool scheme_data::find_in(std::string const&  collection_name) const
{
    return _scheme_data->find_in(collection_name);
}

scheme_data::scheme_data(scheme_data const &other):
    _tree_type(other._tree_type)
//    _scheme_data(new splay_tree<std::string, collection_data, string_comparer>(*reinterpret_cast<splay_tree<std::string, collection_data, string_comparer>*>(other._scheme_data)))
{
    if(other._tree_type == tree_types::SPLAY_TREE)
    {
        _scheme_data = new splay_tree<std::string, collection_data, string_comparer>(*reinterpret_cast<splay_tree<std::string, collection_data, string_comparer>*>(other._scheme_data));
    }
    else if(other._tree_type == tree_types::AVL_TREE)
    {
        _scheme_data = new avl_tree<std::string, collection_data, string_comparer>(*reinterpret_cast<avl_tree<std::string, collection_data, string_comparer>*>(other._scheme_data));
    }
    else if(other._tree_type == tree_types::RED_BLACK_TREE)
    {
//        _scheme_data = new red_black_tree<std::string, collection_data, string_comparer>(*reinterpret_cast<red_black_tree<std::string, collection_data, string_comparer>*>(other._scheme_data));
    }
}

scheme_data& scheme_data::operator=(const scheme_data &other)
{
    if(this == &other)
    {
        return *this;
    }

    delete this->_scheme_data;

    if(other._tree_type == tree_types::SPLAY_TREE)
    {
        this->_scheme_data = new splay_tree<std::string, collection_data, string_comparer>(*reinterpret_cast<splay_tree<std::string, collection_data, string_comparer>*>(other._scheme_data));
    }
    else if(other._tree_type == tree_types::AVL_TREE)
    {
        this->_scheme_data = new avl_tree<std::string, collection_data, string_comparer>(*reinterpret_cast<avl_tree<std::string, collection_data, string_comparer>*>(other._scheme_data));
    }
    else if(other._tree_type == tree_types::RED_BLACK_TREE)
    {
//        this->_scheme_data = new red_black_tree<std::string, collection_data, string_comparer>(*reinterpret_cast<red_black_tree<std::string, collection_data, string_comparer>*>(other._scheme_data));
    }
    this->_tree_type = other._tree_type;

    return *this;
}

scheme_data::scheme_data(scheme_data &&other) noexcept
{
    this->_scheme_data = other._scheme_data;
    this->_tree_type = other._tree_type;
    other._scheme_data = nullptr;
}

scheme_data& scheme_data::operator=(scheme_data &&other) noexcept
{
    if(this == &other)
    {
        return *this;
    }

    delete this->_scheme_data;

    if(other._tree_type == tree_types::SPLAY_TREE)
    {
        this->_scheme_data = new splay_tree<std::string, collection_data, string_comparer>(*reinterpret_cast<splay_tree<std::string, collection_data, string_comparer>*>(other._scheme_data));
    }
    else if(other._tree_type == tree_types::AVL_TREE)
    {
        this->_scheme_data = new avl_tree<std::string, collection_data, string_comparer>(*reinterpret_cast<avl_tree<std::string, collection_data, string_comparer>*>(other._scheme_data));
    }
    else if(other._tree_type == tree_types::RED_BLACK_TREE)
    {
//        this->_scheme_data = new red_black_tree<std::string, collection_data, string_comparer>(*reinterpret_cast<red_black_tree<std::string, collection_data, string_comparer>*>(other._scheme_data));
    }
    this->_tree_type = other._tree_type;

    return *this;
}

scheme_data::~scheme_data()
{
    delete _scheme_data;
}