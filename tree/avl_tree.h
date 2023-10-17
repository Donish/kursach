#ifndef AVL_TREE_H
#define AVL_TREE_H

#include "binary_search_tree.h"

template<
        typename tkey,
        typename tvalue,
        typename tkey_comparer>
class avl_tree final : public binary_search_tree<tkey, tvalue, tkey_comparer>
{
protected:

    struct avl_node : public binary_search_tree<tkey, tvalue, tkey_comparer>::node
    {
        size_t _height = 0;
    };

private:

    class avl_tree_insertion_template_method final : public binary_search_tree<tkey, tvalue, tkey_comparer>::insertion_template_method
    {

    private:

        avl_tree<tkey, tvalue, tkey_comparer> * _tree;

    public:

        explicit avl_tree_insertion_template_method(avl_tree<tkey, tvalue, tkey_comparer> * tree);

    protected:

        void after_insert_inner(
                tkey const &key,
                typename binary_search_tree<tkey, tvalue, tkey_comparer>::node *&subtree_root_address,
                std::stack<typename binary_search_tree<tkey, tvalue, tkey_comparer>::node **> &path_to_subtree_root_exclusive) override;

        void initialize_memory_with_node(typename binary_search_tree<tkey, tvalue, tkey_comparer>::node * const node_address) override;

        size_t get_node_size() const override;

    };

    class avl_tree_removing_template_method final : public binary_search_tree<tkey, tvalue, tkey_comparer>::removing_template_method
    {

    private:

        avl_tree<tkey, tvalue, tkey_comparer> * _tree;

    public:

        explicit avl_tree_removing_template_method(avl_tree<tkey, tvalue, tkey_comparer> * tree);

    protected:

        void after_remove_inner(
                tkey const &key,
                typename binary_search_tree<tkey, tvalue, tkey_comparer>::node *&subtree_root_address,
                std::list<typename binary_search_tree<tkey, tvalue, tkey_comparer>::node **> &path_to_subtree_root_exclusive) override;

    };

    class avl_tree_reading_template_method final : public binary_search_tree<tkey, tvalue, tkey_comparer>::reading_template_method
    {
    private:

        avl_tree<tkey, tvalue, tkey_comparer> * _tree;

    public:

        explicit avl_tree_reading_template_method(avl_tree<tkey, tvalue, tkey_comparer> * tree);

    };

private:

    void clear();

    typename binary_search_tree<tkey, tvalue, tkey_comparer>::node * copy() const;

    typename binary_search_tree<tkey, tvalue, tkey_comparer>::node * copy_inner(typename binary_search_tree<tkey, tvalue, tkey_comparer>::node * to_copy) const;

    void move(avl_tree<tkey, tvalue, tkey_comparer> && other);

private:

    size_t get_node_height(typename binary_search_tree<tkey, tvalue, tkey_comparer>::node *node_address) const;

    void fix_node_height(typename binary_search_tree<tkey, tvalue, tkey_comparer>::node **subtree_node_address);

    size_t balance_factor(typename binary_search_tree<tkey, tvalue, tkey_comparer>::node *subtree_node_address) const;

    void balance_with_stack(typename binary_search_tree<tkey, tvalue, tkey_comparer>::node *&subtree_node_address,
                 std::stack<typename binary_search_tree<tkey, tvalue, tkey_comparer>::node **> &path_to_subtree_root_exclusive);

    void balance_with_list(typename binary_search_tree<tkey, tvalue, tkey_comparer>::node *&subtree_node_address,
                 std::list<typename binary_search_tree<tkey, tvalue, tkey_comparer>::node **> &path_to_subtree_root_exclusive);

public:

    explicit avl_tree(memory *allocator = nullptr, logger *logger = nullptr);

    avl_tree(avl_tree const & other);

    avl_tree(avl_tree && other) noexcept;

    avl_tree &operator=(avl_tree const & other);

    avl_tree &operator=(avl_tree && other) noexcept;

    ~avl_tree() final = default;

private:

    memory *_allocator;
    logger *_logger;

};

template<
        typename tkey,
        typename tvalue,
        typename tkey_comparer>
size_t avl_tree<tkey, tvalue, tkey_comparer>::get_node_height(typename binary_search_tree<tkey, tvalue, tkey_comparer>::node *node_address) const
{
    return node_address != nullptr ? reinterpret_cast<avl_node*>(node_address)->_height : 0;
}

template<
        typename tkey,
        typename tvalue,
        typename tkey_comparer>
void avl_tree<tkey, tvalue, tkey_comparer>::fix_node_height(typename binary_search_tree<tkey, tvalue, tkey_comparer>::node **subtree_node_address)
{
    size_t left_node_height = get_node_height((*subtree_node_address)->left_subtree_address);
    size_t right_node_height = get_node_height((*subtree_node_address)->right_subtree_address);

    reinterpret_cast<avl_node*>(*subtree_node_address)->_height = (left_node_height > right_node_height ? left_node_height : right_node_height) + 1;
}

template<
        typename tkey,
        typename tvalue,
        typename tkey_comparer>
size_t avl_tree<tkey, tvalue, tkey_comparer>::balance_factor(typename binary_search_tree<tkey, tvalue, tkey_comparer>::node *subtree_node_address) const
{
    return get_node_height(subtree_node_address->left_subtree_address) - get_node_height(subtree_node_address->right_subtree_address);
}

template<
        typename tkey,
        typename tvalue,
        typename tkey_comparer>
void avl_tree<tkey, tvalue, tkey_comparer>::balance_with_list(
        typename binary_search_tree<tkey, tvalue, tkey_comparer>::node *&subtree_node_address,
        std::list<typename binary_search_tree<tkey, tvalue, tkey_comparer>::node **> &path_to_subtree_root_exclusive)
{
    typename binary_search_tree<tkey, tvalue, tkey_comparer>::node **parent = nullptr;

    if(!path_to_subtree_root_exclusive.empty())
    {
        parent = path_to_subtree_root_exclusive.back();
        path_to_subtree_root_exclusive.pop_back();
    }

    fix_node_height(&subtree_node_address);
    size_t bf = balance_factor(subtree_node_address);

    if(bf == 2)
    {
        if(balance_factor(subtree_node_address->left_subtree_address) < 0)
        {
            this->left_rotation(subtree_node_address->left_subtree_address);
            fix_node_height(&(subtree_node_address->left_subtree_address->left_subtree_address));
            fix_node_height(&(subtree_node_address->left_subtree_address->right_subtree_address));
            fix_node_height(&(subtree_node_address->left_subtree_address));
        }

        this->right_rotation(subtree_node_address);
        fix_node_height(&(subtree_node_address->right_subtree_address));
        fix_node_height(&(subtree_node_address->left_subtree_address));
        fix_node_height(&subtree_node_address);
    }
    if(bf == -2)
    {
        if(balance_factor(subtree_node_address->right_subtree_address) > 0)
        {
            this->right_rotation(subtree_node_address->right_subtree_address);
            fix_node_height(&(subtree_node_address->right_subtree_address->right_subtree_address));
            fix_node_height(&(subtree_node_address->right_subtree_address->left_subtree_address));
            fix_node_height(&(subtree_node_address->right_subtree_address));
        }

        this->left_rotation(subtree_node_address);
        fix_node_height(&(subtree_node_address->right_subtree_address));
        fix_node_height(&(subtree_node_address->left_subtree_address));
        fix_node_height(&(subtree_node_address));
    }

    if(parent != nullptr)
    {
        path_to_subtree_root_exclusive.push_back(parent);
    }
}

template<
        typename tkey,
        typename tvalue,
        typename tkey_comparer>
void avl_tree<tkey, tvalue, tkey_comparer>::balance_with_stack(
        typename binary_search_tree<tkey, tvalue, tkey_comparer>::node *&subtree_node_address,
        std::stack<typename binary_search_tree<tkey, tvalue, tkey_comparer>::node **> &path_to_subtree_root_exclusive)
{
    typename binary_search_tree<tkey, tvalue, tkey_comparer>::node **parent = nullptr;

    if(!path_to_subtree_root_exclusive.empty())
    {
        parent = path_to_subtree_root_exclusive.top();
        path_to_subtree_root_exclusive.pop();
    }

    fix_node_height(&subtree_node_address);
    size_t bf = balance_factor(subtree_node_address);

    if(bf == 2)
    {
        if(balance_factor(subtree_node_address->left_subtree_address) < 0)
        {
            this->left_rotation(subtree_node_address->left_subtree_address);
            fix_node_height(&(subtree_node_address->left_subtree_address->left_subtree_address));
            fix_node_height(&(subtree_node_address->left_subtree_address->right_subtree_address));
            fix_node_height(&(subtree_node_address->left_subtree_address));
        }

        this->right_rotation(subtree_node_address);
        fix_node_height(&(subtree_node_address->right_subtree_address));
        fix_node_height(&(subtree_node_address->left_subtree_address));
        fix_node_height(&subtree_node_address);
    }
    if(bf == -2)
    {
        if(balance_factor(subtree_node_address->right_subtree_address) > 0)
        {
            this->right_rotation(subtree_node_address->right_subtree_address);
            fix_node_height(&(subtree_node_address->right_subtree_address->right_subtree_address));
            fix_node_height(&(subtree_node_address->right_subtree_address->left_subtree_address));
            fix_node_height(&(subtree_node_address->right_subtree_address));
        }

        this->left_rotation(subtree_node_address);
        fix_node_height(&(subtree_node_address->right_subtree_address));
        fix_node_height(&(subtree_node_address->left_subtree_address));
        fix_node_height(&(subtree_node_address));
    }

    if(parent != nullptr)
    {
        path_to_subtree_root_exclusive.push(parent);
    }
}

template<
        typename tkey,
        typename tvalue,
        typename tkey_comparer>
avl_tree<tkey, tvalue, tkey_comparer> &avl_tree<tkey, tvalue, tkey_comparer>::operator=(avl_tree &&other) noexcept
{
    if (this == &other)
    {
        return *this;
    }

    clear();

    delete this->_insertion;
    delete this->_reading;
    delete this->_removing;

    move(std::move(other));

    return *this;
}

template<
        typename tkey,
        typename tvalue,
        typename tkey_comparer>
avl_tree<tkey, tvalue, tkey_comparer> &avl_tree<tkey, tvalue, tkey_comparer>::operator=(const avl_tree<tkey, tvalue, tkey_comparer> &other)
{
    if (this == &other)
    {
        return *this;
    }

    clear();
    this->_root = other.copy();

    return *this;
}

template<
        typename tkey,
        typename tvalue,
        typename tkey_comparer>
avl_tree<tkey, tvalue, tkey_comparer>::avl_tree(avl_tree<tkey, tvalue, tkey_comparer> &&other) noexcept
{
    move(std::move(other));

    this->trace_with_guard("The tree has been moved.");
}

template<typename tkey, typename tvalue, typename tkey_comparer>
avl_tree<tkey, tvalue, tkey_comparer>::avl_tree(const avl_tree<tkey, tvalue, tkey_comparer> &other)
        : avl_tree<tkey, tvalue, tkey_comparer>(other.get_allocator(), other.get_logger())
{
    this->_root = other.copy();
}

template<typename tkey, typename tvalue, typename tkey_comparer>
void avl_tree<tkey, tvalue, tkey_comparer>::move(avl_tree<tkey, tvalue, tkey_comparer> &&other)
{
    this->_root = other._root;
    other._root = nullptr;

    this->_allocator = other._allocator;
    other._allocator = nullptr;

    this->_logger = other._logger;
    other._logger = nullptr;

    this->_insertion = other._insertion;
    other._insertion = nullptr;

    this->_reading = other._reading;
    other._reading = nullptr;

    this->_removing = other._removing;
    other._removing = nullptr;
}

template<typename tkey, typename tvalue, typename tkey_comparer>
typename binary_search_tree<tkey, tvalue, tkey_comparer>::node *
avl_tree<tkey, tvalue, tkey_comparer>::copy_inner(typename binary_search_tree<tkey, tvalue, tkey_comparer>::node *to_copy) const
{
    if (to_copy == nullptr)
    {
        return nullptr;
    }

    auto * node_copy = reinterpret_cast<typename binary_search_tree<tkey, tvalue, tkey_comparer>::node*>(this->allocate_with_guard(this->get_node_size()));

    new (node_copy) typename binary_search_tree<tkey, tvalue, tkey_comparer>::node;

    node_copy->key_and_value._key = to_copy->key_and_value._key;
    node_copy->key_and_value._value = to_copy->key_and_value._value;
    node_copy->left_subtree_address = copy_inner(to_copy->left_subtree_address);
    node_copy->right_subtree_address = copy_inner(to_copy->right_subtree_address);

    return node_copy;
}

template<typename tkey, typename tvalue, typename tkey_comparer>
typename binary_search_tree<tkey, tvalue, tkey_comparer>::node *avl_tree<tkey, tvalue, tkey_comparer>::copy() const
{
    return copy_inner(this->_root);
}

template<typename tkey, typename tvalue, typename tkey_comparer>
void avl_tree<tkey, tvalue, tkey_comparer>::clear()
{
    auto it_end = reinterpret_cast<binary_search_tree<tkey, tvalue, tkey_comparer> *>(this)->end_postfix();
    for (auto it = reinterpret_cast<binary_search_tree<tkey, tvalue, tkey_comparer>* >(this)->begin_postfix(); it != it_end; ++it)
    {
        it.get_current_node()->~node();
        this->deallocate_with_guard(it.get_current_node());
    }
}

template<
        typename tkey,
        typename tvalue,
        typename tkey_comparer>
void avl_tree<tkey, tvalue, tkey_comparer>::avl_tree_insertion_template_method::after_insert_inner(
        const tkey &key,
        typename binary_search_tree<tkey, tvalue, tkey_comparer>::node *&subtree_root_address,
        std::stack<typename binary_search_tree<tkey, tvalue, tkey_comparer>::node **> &path_to_subtree_root_exclusive)
{
    typename binary_search_tree<tkey, tvalue, tkey_comparer>::node *current_node = subtree_root_address;

    while(!path_to_subtree_root_exclusive.empty())
    {
        _tree->balance_with_stack(current_node, path_to_subtree_root_exclusive);
        current_node = *path_to_subtree_root_exclusive.top();
        path_to_subtree_root_exclusive.pop();
    }

    this->trace_with_guard("[AVL TREE] Node inserted.");
}

template<
        typename tkey,
        typename tvalue,
        typename tkey_comparer>
void avl_tree<tkey, tvalue, tkey_comparer>::avl_tree_insertion_template_method::initialize_memory_with_node(
        typename binary_search_tree<tkey, tvalue, tkey_comparer>::node * const node_address)
{
    new (node_address) avl_node;
}

template<
        typename tkey,
        typename tvalue,
        typename tkey_comparer>
size_t avl_tree<tkey, tvalue, tkey_comparer>::avl_tree_insertion_template_method::get_node_size() const
{
    return sizeof(avl_node);
}

template<
        typename tkey,
        typename tvalue,
        typename tkey_comparer>
void avl_tree<tkey, tvalue, tkey_comparer>::avl_tree_removing_template_method::after_remove_inner(
        const tkey &key,
        typename binary_search_tree<tkey, tvalue, tkey_comparer>::node *&subtree_root_address,
        std::list<typename binary_search_tree<tkey, tvalue, tkey_comparer>::node **> &path_to_subtree_root_exclusive)
{
    typename binary_search_tree<tkey, tvalue, tkey_comparer>::node *current_node = subtree_root_address;

    while(!path_to_subtree_root_exclusive.empty())
    {
        _tree->balance_with_list(current_node, path_to_subtree_root_exclusive);
        current_node = *path_to_subtree_root_exclusive.back();
        path_to_subtree_root_exclusive.pop_back();
    }
}

template<
        typename tkey,
        typename tvalue,
        typename tkey_comparer>
avl_tree<tkey, tvalue, tkey_comparer>::avl_tree_insertion_template_method::avl_tree_insertion_template_method(
        avl_tree<tkey, tvalue, tkey_comparer> *tree):
        _tree(tree),
        binary_search_tree<tkey, tvalue, tkey_comparer>::insertion_template_method(tree)
{

}

template<
        typename tkey,
        typename tvalue,
        typename tkey_comparer>
avl_tree<tkey, tvalue, tkey_comparer>::avl_tree_removing_template_method::avl_tree_removing_template_method(
        avl_tree<tkey, tvalue, tkey_comparer> *tree):
        _tree(tree),
        binary_search_tree<tkey, tvalue, tkey_comparer>::removing_template_method(tree)
{

}

template<
        typename tkey,
        typename tvalue,
        typename tkey_comparer>
avl_tree<tkey, tvalue, tkey_comparer>::avl_tree_reading_template_method::avl_tree_reading_template_method(
        avl_tree<tkey, tvalue, tkey_comparer> *tree):
        _tree(tree),
        binary_search_tree<tkey, tvalue, tkey_comparer>::reading_template_method(tree)
{

}

template<
        typename tkey,
        typename tvalue,
        typename tkey_comparer>
avl_tree<tkey, tvalue, tkey_comparer>::avl_tree(memory *allocator, logger *logger):
        binary_search_tree<tkey, tvalue, tkey_comparer>(
                new avl_tree_insertion_template_method(this),
                new avl_tree_reading_template_method(this),
                new avl_tree_removing_template_method(this),
                allocator,
                logger)
{
    this->trace_with_guard("[AVL TREE] The tree has been created.");
}

#endif //AVL_TREE_H