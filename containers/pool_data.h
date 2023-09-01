#ifndef POOL_DATA_H
#define POOL_DATA_H

#include "scheme_data.h"

class pool_data final
{
private:

    memory *_allocator;

    associative_container<std::string, scheme_data> *_pool_data;

public:

    pool_data(memory *allocator = nullptr);

    ~pool_data();

public:

    void add(std::string const &key, scheme_data &&target);

    void remove(std::string const &key);

    scheme_data const &find(std::string const &scheme_data_name) const;

    bool find_in(std::string const &scheme_data_name) const;

    memory *get_outer_allocator() const;

public:

    pool_data(pool_data const &other);

    pool_data &operator=(pool_data const &other);

    pool_data(pool_data &&other) noexcept;

    pool_data &operator=(pool_data &&other) noexcept;

};

#endif //POOL_DATA_H