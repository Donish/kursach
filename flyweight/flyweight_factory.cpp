#include "flyweight_factory.h"

flyweight_factory& flyweight_factory::get_instance()
{
    static flyweight_factory instance;

    return instance;
}

std::shared_ptr<flyweight_string> flyweight_factory::get_flyweight_string(const std::string &value)
{
    auto iter = _flyweight_string.find(value);

    if(iter != _flyweight_string.end())
    {
        return iter->second;
    }
    else
    {
        auto flyweight = std::make_shared<flyweight_string>();
        flyweight->set_value(value);
        _flyweight_string[value] = flyweight;

        return flyweight;
    }
}