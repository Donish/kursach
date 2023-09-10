#ifndef MEMORY_SIMPLE_H
#define MEMORY_SIMPLE_H

#include "memory.h"
#include "../logger/logger_concrete.h"
#include "../logger/logger_builder_concrete.h"
#include "../logger/logger_holder.h"
#include "memory_holder.h"

class memory_simple final :
        public memory,
        public memory_holder,
        public logger_holder
{
private:

    logger *_log_memory = nullptr;

public:

    void* const allocate(size_t target_size) const override;

    void deallocate(void *target_to_dealloc) const override;

    memory_simple(logger* const log_memory = nullptr);

public:

    memory_simple(memory_simple const &other) = delete;

    memory_simple &operator=(memory_simple const &other) = delete;

    memory_simple(memory_simple &&other) = delete;

    memory_simple &operator=(memory_simple &&other) = delete;

protected:

    size_t get_occupied_block_size_without_service_block(void *current_block) const override;

    void *get_address_relative_to_allocator(void *current_block_address) const override;

    logger *get_logger() const override;

    memory *get_allocator() const override;
};

#endif //MEMORY_SIMPLE_H