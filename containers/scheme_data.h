#ifndef SCHEME_DATA_H
#define SCHEME_DATA_H

#include "collection_data.h"

class scheme_data final
{
private:

    associative_container<std::string, collection_data> *_scheme_data;

public:

    scheme_data();

    ~scheme_data();

public:

    void add(std::string const &key, collection_data &&target);

    void remove(std::string const &key);

    collection_data const& find(std::string const &collection_name) const;

    bool find_in(std::string const& collection_name) const;

public:

    scheme_data(scheme_data const &other);

    scheme_data &operator=(scheme_data const &other);

    scheme_data(scheme_data &&other) noexcept;

    scheme_data &operator=(scheme_data &&other) noexcept;

};

#endif //SCHEME_DATA_H