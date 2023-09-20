#ifndef KEY_COMPARER_H
#define KEY_COMPARER_H

#include "../owntype/meeting.h"
#include "string_comparer.h"

class key_comparer final
{
private:

    string_comparer _string_comparer;

public:

    int operator()(meeting* key1, meeting* key2);

};

#endif //KEY_COMPARER_H