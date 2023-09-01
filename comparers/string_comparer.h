#ifndef STRING_COMPARER_H
#define STRING_COMPARER_H

#include "../owntype/own_type.h"

class string_comparer final
{
public:

    int operator()(std::string const &str1, std::string const &str2);

};

#endif