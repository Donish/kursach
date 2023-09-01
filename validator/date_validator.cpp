#include "date_validator.h"
#include <sstream>

std::vector<std::string> date_validator::split(const std::string &text, char delim) const
{
    std::vector<std::string> result;
    std::string token;
    std::istringstream tokenStream(text);
    while (std::getline(tokenStream, token, delim))
    {
        result.push_back(token);
    }
    return result;
}

bool date_validator::operator()(const std::string &date)
{
    if (date.size() == 10)
    {
        if (date[2] == '.' && date[5] == '.')
        {
            auto result = split(date, '.');

            for (auto const & number : result)
            {
                if (!_digit_validator(number))
                {
                    return false;
                }
            }

            return true;
        }
        else if (date[2] == '/' && date[5] == '/')
        {
            auto result = split(date, '/');

            for (auto const & number : result)
            {
                if (!_digit_validator(number))
                {
                    return false;
                }
            }

            return true;
        }
    }

    return false;
}