#include "scheme_data.h"

scheme_data::scheme_data():
    _scheme_data(new splay_tree<std::string, collection_data, string_comparer>())
{}

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

//конструкторы
scheme_data::scheme_data(scheme_data const &other):
    _scheme_data(new splay_tree<std::string, collection_data, string_comparer>(*reinterpret_cast<splay_tree<std::string, collection_data, string_comparer>*>(other._scheme_data)))
{}

scheme_data& scheme_data::operator=(const scheme_data &other)
{
    if(this == &other)
    {
        return *this;
    }

    delete this->_scheme_data;

    this->_scheme_data = new splay_tree<std::string, collection_data, string_comparer>(*reinterpret_cast<splay_tree<std::string, collection_data, string_comparer>*>(other._scheme_data));

    return *this;
}

scheme_data::scheme_data(scheme_data &&other) noexcept
{
    this->_scheme_data = other._scheme_data;
    other._scheme_data = nullptr;
}

scheme_data& scheme_data::operator=(scheme_data &&other) noexcept
{
    if(this == &other)
    {
        return *this;
    }

    delete this->_scheme_data;

    this->_scheme_data = new splay_tree<std::string, collection_data, string_comparer>(*reinterpret_cast<splay_tree<std::string, collection_data, string_comparer>*>(other._scheme_data));

    return *this;
}

scheme_data::~scheme_data()
{
    delete _scheme_data;
}