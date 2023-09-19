#ifndef FLYWEIGHT_STRING_H
#define FLYWEIGHT_STRING_H

#include <memory>
#include <unordered_map>
#include <iostream>

class flyweight_string
{
private:

    std::string _value;

public:

    const std::string &get_value() const;

    void set_value(const std::string &value);

};

#endif //FLYWEIGHT_STRING_H