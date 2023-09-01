#ifndef ASSOCIATIVE_CONTAINER_H
#define ASSOCIATIVE_CONTAINER_H

#include <iostream>
#include <vector>

template<
    typename tkey,
    typename tvalue>
class associative_container
{
public:

    enum class mode
    {
        INSERT,
        UPDATE
    };

    struct key_value_pair final
    {
        tkey _key;
        tvalue _value;
    };


public:

    virtual ~associative_container() = default;

public:

    virtual void insert(
        tkey const &key,
        tvalue &&value) = 0;

    virtual void update(
        tkey const &key,
        tvalue &&value) = 0;
    
    virtual bool find_in(
        tkey const &key) = 0;

    virtual tvalue const &find(
        tkey const &key) = 0;

    tvalue get_value(
        tkey const &key);

    virtual std::vector<tvalue> find_in_range(
        tkey const &min_bound,
        tkey const &max_bound) = 0;

    virtual std::tuple<tkey, tvalue> remove_node(
        tkey const &key) = 0;

    virtual tvalue remove(
        tkey const &key) = 0;

public:

    void operator+=(key_value_pair pair);

    bool operator[](key_value_pair* target_key_value);

    tvalue &&operator-=(tkey const &key);

};

template<
    typename tkey,
    typename tvalue>
tvalue associative_container<tkey, tvalue>::get_value(tkey const &key)
{
    return find(key);
}

template<
    typename tkey,
    typename tvalue>
void associative_container<tkey, tvalue>::operator+=(key_value_pair pair)
{
    insert(pair._key, std::move(pair._value));
}

template<
    typename tkey,
    typename tvalue>
bool associative_container<tkey, tvalue>::operator[](key_value_pair* target_key_value)
{
    return find(target_key_value);
}

template<
    typename tkey,
    typename tvalue>
tvalue&& associative_container<tkey, tvalue>::operator-=(tkey const &key)
{
    return std::move(remove(key));
}

#endif //ASSOCIATIVE_CONTAINER_H
