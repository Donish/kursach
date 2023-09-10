#ifndef SORTED_LIST_MEMORY_H
#define SORTED_LIST_MEMORY_H

#include "memory.h"
#include "../logger/logger_builder_concrete.h"
#include "../logger/logger_concrete.h"
#include "memory_holder.h"
#include "../logger/logger_holder.h"

class sorted_list_memory final :
        public memory,
        public memory_holder,
        public logger_holder
{
private:

    void * _allocated_memory;

public:

    void * const allocate(size_t target_size) const override;

    void deallocate(void * target_to_dealloc) const override;

    explicit sorted_list_memory(
        size_t target_size,
        memory::allocation_mode mode = memory::allocation_mode::first_match,
        logger *logger = nullptr,
        memory *outer_allocator = nullptr);

    ~sorted_list_memory() override;

public:

    sorted_list_memory(sorted_list_memory const & other) = delete;

    sorted_list_memory & operator=(sorted_list_memory const & other) = delete;

    sorted_list_memory(sorted_list_memory && other) = delete;

    sorted_list_memory & operator=(sorted_list_memory && other) = delete;

protected:

    size_t get_available_block_service_block_size() const override;

    size_t get_occupied_block_service_block_size() const override;

    size_t get_allocator_service_block_size() const override;

    size_t get_allocated_memory_size() const override;

    size_t get_available_block_size(void * current_block) const override;

    size_t get_occupied_block_size(void * current_block) const override;

    size_t get_occupied_block_size_without_service_block(void * current_block) const override;

    void* get_first_available_block() const override;

    void **get_first_available_block_ptr() const override;

    void* get_next_available_block(void *current_block) const override;

    void * get_start_allocated_memory_address() const override;

    void * get_address_relative_to_allocator(void * current_block_address) const override;

    memory* get_allocator() const override;

    logger * get_logger() const override;

    memory::allocation_mode get_allocation_mode() const override;
};

#endif //SORTED_LIST_MEMORY_H