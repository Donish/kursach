#include "flyweight_string.h"

const std::string& flyweight_string::get_value() const
{
    return _value;
}

void flyweight_string::set_value(const std::string &value)
{
    _value = value;
}