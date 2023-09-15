#ifndef COLLECTION_DATA_H
#define COLLECTION_DATA_H

#include "../owntype/own_type.h"
#include "../allocator/memory_simple.h"
#include "../allocator/sorted_list_memory.h"
#include "../allocator/borders_descriptors_memory.h"
#include "../allocator/buddy_system_memory.h"
#include "../tree/binary_search_tree.h"
#include "../tree/splay_tree.h"
#include "../tree/avl_tree.h"
//#include "../tree/red_black_tree.h"
#include "../comparers/key_comparer.h"
#include "../tree/tree_types.h"

class collection_data final :
        protected memory_holder
{
private:

    memory *_allocator;

    associative_container<key*, value*> *_data;

    tree_types _tree_type;

public:

    collection_data(memory *allocator = nullptr, tree_types outer_tree_type = tree_types::SPLAY_TREE);

    ~collection_data();

public:

    void add(
        unsigned int id_meeting,
        meeting_type type,
        format form,
        const std::string &description,
        const std::string &link,
        const std::string &creator_surname,
        const std::string &creator_name,
        const std::string &creator_patronymic,
        const std::string &date,
        const std::string &start_time,
        const std::string &min_duration,
        const std::string &invited_people);

    value* get(key* const &data_key);

    value* remove(key* data_key);

    bool find_in(key* const &data_key) const;

    std::vector<value*> get_between_keys(key* const &min_bound, key* const &max_bound);

    void update(
        key* const &data_key,
        meeting_type type,
        format form,
        const std::string &description,
        const std::string &link,
        const std::string &creator_surname,
        const std::string &creator_name,
        const std::string &creator_patronymic,
        const std::string &date,
        const std::string &start_time,
        const std::string &min_duration,
        const std::string &invited_people);
    
public:

    collection_data(collection_data const &other);

    collection_data& operator=(collection_data const &other);

    collection_data(collection_data &&other) noexcept;

    collection_data& operator=(collection_data &&other) noexcept;

private:

    memory* get_allocator() const override;

};

#endif //COLLECTION_DATA_H