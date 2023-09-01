#include "string_comparer.h"

int string_comparer::operator()(std::string const &str1, std::string const &str2)
{
    if(str1 > str2)
    {
        return 1;
    } 
    else if(str1 < str2)
    {
        return -1;
    } 
    else
    {
        return 0;
    }
}