#ifndef FLYWEIGHT_FACTORY_H
#define FLYWEIGHT_FACTORY_H

#include <iostream>
#include <memory>
#include <unordered_map>
#include "flyweight_string.h"

class flyweight_factory
{
private:

    std::unordered_map<std::string, std::shared_ptr<flyweight_string>> _flyweight_string;

public:

    static flyweight_factory &get_instance();

    std::shared_ptr<flyweight_string> get_flyweight_string(const std::string &value);

};

#endif //FLYWEIGHT_FACTORY_H