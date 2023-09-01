#include "key_comparer.h"

int key_comparer::operator()(key* key1, key* key2)
{
    if(key1->_id_meeting > key2->_id_meeting)
    {
        return 1;
    } else if(key1->_id_meeting < key2->_id_meeting)
    {
        return -1;
    } else{
        return 0;
    }
}