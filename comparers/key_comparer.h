#ifndef KEY_COMPARER_H
#define KEY_COMPARER_H

#include "../owntype/own_type.h"
#include "string_comparer.h"

class key_comparer final
{
private:

    string_comparer _string_comparer;

public:

    int operator()(key* key1, key* key2);

};

#endif //KEY_COMPARER_H