#include "time_validator.h"
#include <sstream>

std::vector<std::string> time_validator::split(const std::string &text, char delim) const
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

bool time_validator::operator()(const std::string &time)
{
    if (time.size() == 5)
    {
        if (time[2] == ':')
        {
            auto result = split(time, ':');

            for (auto const &number : result)
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