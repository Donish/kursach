#include "memory_simple.h"

memory_simple::memory_simple(logger* const log_memory)
    : _log_memory(log_memory) {}

void* memory_simple::get_address_relative_to_allocator(void * current_block_address) const
{
    return current_block_address;
}

logger* memory_simple::get_logger() const
{
    return _log_memory;
}

memory* memory_simple::get_outer_allocator() const
{
    return nullptr;
}

size_t memory_simple::get_occupied_block_size_without_service_block(void *current_block) const
{
    return *(reinterpret_cast<size_t*>(current_block) - 1);
}

void* const memory_simple::allocate(size_t target_size) const
{
    try
    {
        auto *block_of_memory = ::operator new(target_size + sizeof(size_t));

        auto *block_size = reinterpret_cast<size_t*>(block_of_memory);
        *block_size = target_size;


        _log_memory->log("[GLOBAL HEAP ALLOCATOR] Memory allocated success at address: " + address_to_string(block_size + 1) + " success.", logger::severity::trace);

        return reinterpret_cast<void*>(block_size + 1);
    }
    catch (const std::bad_alloc &ex)
    {
        std::string message = "Memory allocated error";
        // warning_with_guard("[GLOBAL HEAP ALLOCATOR] " + message + ".");
        _log_memory->log("[GLOBAL HEAP ALLOCATOR] " + message + ".", logger::severity::warning);

        throw memory_exception("Memory allocated error");
    }
}

void memory_simple::deallocate(void * target_to_dealloc) const
{
    memory_state_before_deallocation(target_to_dealloc, get_logger());

    auto * target_block = reinterpret_cast<void*>(reinterpret_cast<size_t*>(target_to_dealloc) - 1);

    ::operator delete(target_block);

    _log_memory->log("[GLOBAL HEAP ALLOCATOR] Memory at address: " + address_to_string(target_to_dealloc) + "was deallocated", logger::severity::trace);
}