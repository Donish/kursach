#ifndef MEMORY_HOLDER_H
#define MEMORY_HOLDER_H

#include "memory.h"

class memory_holder
{
public:

    void *const allocate_with_guard(size_t) const;

    void deallocate_with_guard(void*) const;

protected:

    virtual memory *get_allocator() const = 0;

public:

    virtual ~memory_holder() = default;

};

#endif //MEMORY_HOLDER_H