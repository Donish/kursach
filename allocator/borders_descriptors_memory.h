#ifndef BORDERS_DESCRIPTORS_MEMORY_H
#define BORDERS_DESCRIPTORS_MEMORY_H

#include "memory.h"
#include "../logger/logger_concrete.h"
#include "../logger/logger_builder_concrete.h"

class borders_descriptors_memory final :
        public memory
{
private:

    void * _allocated_memory;

public:

    void * const allocate(size_t target_size) const override;

    void deallocate(void * target_to_dealloc) const override;

    explicit borders_descriptors_memory(
        size_t target_size,
        memory::allocation_mode mode,
        logger *logger = nullptr,
        memory *outer_allocator = nullptr);

    ~borders_descriptors_memory() override;

public:

    borders_descriptors_memory(borders_descriptors_memory const & other) = delete;

    borders_descriptors_memory & operator=(borders_descriptors_memory const & other) = delete;

    borders_descriptors_memory(borders_descriptors_memory && other) = delete;

    borders_descriptors_memory & operator=(borders_descriptors_memory && other) = delete;

protected:

    size_t get_allocator_service_block_size() const override; 

    size_t get_allocated_memory_size() const override;

    void * get_start_allocated_memory_address() const override;

    void * get_end_allocated_memory_address() const override;

    size_t get_available_block_size(void * next_occupied_block_to_target_block) const override; 

    size_t get_occupied_block_service_block_size() const override; 

    size_t get_occupied_block_size(void * current_block) const override; 

    size_t get_occupied_block_size_without_service_block(void * current_block) const override; 

    void * get_first_occupied_block() const override; 

    void ** get_first_occupied_block_ptr() const override; 

    void * get_next_occupied_block(void * current_block) const; 

    void * get_previous_occupied_block(void * current_block) const; 

    void * get_address_relative_to_allocator(void * current_block_address) const override;

    memory* get_outer_allocator() const override; 

    logger * get_logger() const override; 

    memory::allocation_mode get_allocation_mode() const override; 
};

#endif