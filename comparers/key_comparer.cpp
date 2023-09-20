#include "key_comparer.h"

int key_comparer::operator()(meeting* key1, meeting* key2)
{
    if(key1->get_id() > key2->get_id())
    {
        return 1;
    } else if(key1->get_id() < key2->get_id())
    {
        return -1;
    } else{
        return 0;
    }
}