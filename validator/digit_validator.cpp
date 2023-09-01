#include "digit_validator.h"

bool digit_validator::operator()(const std::string &number)
{
    for (auto const &letter : number)
    {
        if (!std::isdigit(letter))
        {
            return false;
        }
    }
    
    return true;
}