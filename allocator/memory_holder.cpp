#include "memory_holder.h"

void *const memory_holder::allocate_with_guard(size_t request_size) const
{
    memory *allocator = get_allocator();

    try
    {
        auto *block = allocator == nullptr ? ::operator new(request_size) : allocator->allocate(request_size);
        return block;
    }
    catch(std::exception const &ex)
    {
        throw memory::memory_exception("Allocation error!");
    }
}

void memory_holder::deallocate_with_guard(void *block_to_delete) const
{
    memory *allocator = get_allocator();

    try
    {
        allocator == nullptr ? ::operator delete(block_to_delete) : allocator->deallocate(block_to_delete);
    }
    catch(std::exception const &ex)
    {
        throw memory::memory_exception("Deallocation error!");
    }
}