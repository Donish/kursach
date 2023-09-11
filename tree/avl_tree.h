#ifndef AVL_TREE_H
#define AVL_TREE_H

#include "binary_search_tree.h"

template<
        typename tkey,
        typename tvalue,
        typename tkey_comparer>
class avl_tree final : public binary_search_tree<tkey, tvalue, tkey_comparer>
{
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

    class avl_tree_reading_template_method final : public binary_search_tree<tkey, tvalue, tkey_comparer>::reading_template_method
    {

    private:

        avl_tree<tkey, tvalue, tkey_comparer> * _tree;

    public:

        explicit avl_tree_reading_template_method(avl_tree<tkey, tvalue, tkey_comparer> * tree);

    protected:

        void after_read_inner(
                typename binary_search_tree<tkey, tvalue, tkey_comparer>::node *&subtree_root_address,
                std::stack<typename binary_search_tree<tkey, tvalue, tkey_comparer>::node **> &path_to_subtree_root_exclusive) override;

    };

    class avl_tree_removing_template_method final : public binary_search_tree<tkey, tvalue, tkey_comparer>::removing_template_method
    {

    private:

        avl_tree<tkey, tvalue, tkey_comparer> * _tree;

    public:

        explicit avl_tree_removing_template_method(avl_tree<tkey, tvalue, tkey_comparer> * tree);

    protected:

        std::tuple<tkey, tvalue> remove_node_inner(
                tkey const &key,
                typename binary_search_tree<tkey, tvalue, tkey_comparer>::node *& subtree_root_address,
                std::list<typename binary_search_tree<tkey, tvalue, tkey_comparer>::node **> &path_to_subtree_root_exclusive) override;

        tvalue remove_inner(
                tkey const &key,
                typename binary_search_tree<tkey, tvalue, tkey_comparer>::node *& subtree_root_address,
                std::list<typename binary_search_tree<tkey, tvalue, tkey_comparer>::node **> &path_to_subtree_root_exclusive) override;
    };

protected:

//    void splay(
//            typename binary_search_tree<tkey, tvalue, tkey_comparer>::node *&subtree_root_address,
//            std::stack<typename binary_search_tree<tkey, tvalue, tkey_comparer>::node **> &path_to_subtree_root_exclusive) const;
//
//    typename binary_search_tree<tkey, tvalue, tkey_comparer>::node * merge(
//            typename binary_search_tree<tkey, tvalue, tkey_comparer>::node *&left_subtree,
//            typename binary_search_tree<tkey, tvalue, tkey_comparer>::node *&right_subtree);

private:

    void clear();

    typename binary_search_tree<tkey, tvalue, tkey_comparer>::node * copy() const;

    typename binary_search_tree<tkey, tvalue, tkey_comparer>::node * copy_inner(typename binary_search_tree<tkey, tvalue, tkey_comparer>::node * to_copy) const;

    void move(avl_tree<tkey, tvalue, tkey_comparer> && other);

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

//template<
//        typename tkey,
//        typename tvalue,
//        typename tkey_comparer>
//void avl_tree<tkey, tvalue, tkey_comparer>::splay(
//        typename binary_search_tree<tkey, tvalue, tkey_comparer>::node *&subtree_root_address,
//        std::stack<typename binary_search_tree<tkey, tvalue, tkey_comparer>::node **> &path_to_subtree_root_exclusive) const
//{
//    typename binary_search_tree<tkey, tvalue, tkey_comparer>::node * current_node = subtree_root_address;
//    typename binary_search_tree<tkey, tvalue, tkey_comparer>::node ** parent = nullptr;
//    typename binary_search_tree<tkey, tvalue, tkey_comparer>::node ** grand_parent = nullptr;
//
//    while (!path_to_subtree_root_exclusive.empty())
//    {
//        if (path_to_subtree_root_exclusive.size() == 1)
//        {
//            parent = path_to_subtree_root_exclusive.top();
//
//            if ((*parent)->left_subtree_address == current_node)
//            {
//                this->right_rotation(*parent);
//            }
//            else
//            {
//                this->left_rotation(*parent);
//            }
//
//            path_to_subtree_root_exclusive.pop();
//        }
//        else
//        {
//            parent = path_to_subtree_root_exclusive.top();
//            path_to_subtree_root_exclusive.pop();
//
//            grand_parent = path_to_subtree_root_exclusive.top();
//            path_to_subtree_root_exclusive.pop();
//
//            if ((*parent)->left_subtree_address == current_node && (*grand_parent)->left_subtree_address == *parent)
//            {
//                this->right_rotation(*grand_parent);
//                this->right_rotation(*grand_parent);
//                current_node = *grand_parent;
//            }
//            else if ((*parent)->right_subtree_address == current_node && (*grand_parent)->right_subtree_address == *parent)
//            {
//                this->left_rotation(*grand_parent);
//                this->left_rotation(*grand_parent);
//                current_node = *grand_parent;
//            }
//            else if ((*parent)->right_subtree_address == current_node && (*grand_parent)->left_subtree_address == *parent)
//            {
//                this->left_rotation(*parent);
//                this->right_rotation(*grand_parent);
//                current_node = *grand_parent;
//            }
//            else
//            {
//                this->right_rotation(*parent);
//                this->left_rotation(*grand_parent);
//                current_node = *grand_parent;
//            }
//        }
//    }
//}

//template<
//        typename tkey,
//        typename tvalue,
//        typename tkey_comparer>
//typename binary_search_tree<tkey, tvalue, tkey_comparer>::node *avl_tree<tkey, tvalue, tkey_comparer>::merge(
//        typename binary_search_tree<tkey, tvalue, tkey_comparer>::node *&left_subtree,
//        typename binary_search_tree<tkey, tvalue, tkey_comparer>::node *&right_subtree)
//{
//    if (left_subtree == nullptr && right_subtree == nullptr)
//    {
//        return nullptr;
//    }
//
//    if (left_subtree == nullptr && right_subtree != nullptr)
//    {
//        return right_subtree;
//    }
//    else if (left_subtree != nullptr && right_subtree == nullptr)
//    {
//        return left_subtree;
//    }
//    else
//    {
//        std::stack<typename binary_search_tree<tkey, tvalue, tkey_comparer>::node **> path_to_subtree_root_exclusive;
//
//        typename binary_search_tree<tkey, tvalue, tkey_comparer>::node * successor = left_subtree;
//        typename binary_search_tree<tkey, tvalue, tkey_comparer>::node ** previous_node_address = &left_subtree;
//
//        while (successor->right_subtree_address != nullptr)
//        {
//            path_to_subtree_root_exclusive.push(previous_node_address);
//            previous_node_address = &(successor->right_subtree_address);
//            successor = successor->right_subtree_address;
//        }
//
//        splay(successor, path_to_subtree_root_exclusive);
//
//        left_subtree->right_subtree_address = right_subtree;
//
//        return left_subtree;
//    }
//}

template<
        typename tkey,
        typename tvalue,
        typename tkey_comparer>
void avl_tree<tkey, tvalue, tkey_comparer>::avl_tree_insertion_template_method::after_insert_inner(
        const tkey &key,
        typename binary_search_tree<tkey, tvalue, tkey_comparer>::node *&subtree_root_address,
        std::stack<typename binary_search_tree<tkey, tvalue, tkey_comparer>::node **> &path_to_subtree_root_exclusive)
{
    _tree->splay(subtree_root_address, path_to_subtree_root_exclusive);

    this->trace_with_guard("[AVL TREE] Node inserted.");
}

template<
        typename tkey,
        typename tvalue,
        typename tkey_comparer>
void avl_tree<tkey, tvalue, tkey_comparer>::avl_tree_insertion_template_method::initialize_memory_with_node(
        typename binary_search_tree<tkey, tvalue, tkey_comparer>::node * const node_address)
{
    new (node_address) typename binary_search_tree<tkey, tvalue, tkey_comparer>::node;
}

template<
        typename tkey,
        typename tvalue,
        typename tkey_comparer>
size_t avl_tree<tkey, tvalue, tkey_comparer>::avl_tree_insertion_template_method::get_node_size() const
{
    return sizeof(typename binary_search_tree<tkey, tvalue, tkey_comparer>::node);
}

template<
        typename tkey,
        typename tvalue,
        typename tkey_comparer>
void avl_tree<tkey, tvalue, tkey_comparer>::avl_tree_reading_template_method::after_read_inner(
        typename binary_search_tree<tkey, tvalue, tkey_comparer>::node *&subtree_root_address,
        std::stack<typename binary_search_tree<tkey, tvalue, tkey_comparer>::node **> &path_to_subtree_root_exclusive)
{
    _tree->splay(subtree_root_address, path_to_subtree_root_exclusive);
}

template<
        typename tkey,
        typename tvalue,
        typename tkey_comparer>
tvalue avl_tree<tkey, tvalue, tkey_comparer>::avl_tree_removing_template_method::remove_inner(
        const tkey &key,
        typename binary_search_tree<tkey, tvalue, tkey_comparer>::node *&subtree_root_address,
        std::list<typename binary_search_tree<tkey, tvalue, tkey_comparer>::node **> &path_to_subtree_root_exclusive)
{
    if (subtree_root_address == nullptr)
    {
        std::string message = "Tree is empty";
        this->warning_with_guard(message);

        throw std::invalid_argument("[AVL TREE] " + message + ".");
    }

    std::stack<typename binary_search_tree<tkey, tvalue, tkey_comparer>::node **> path;
    typename binary_search_tree<tkey, tvalue, tkey_comparer>::node ** current_node = &subtree_root_address;

    tkey_comparer comparator;

    while (*current_node != nullptr)
    {
        int compare_result = comparator(key, (*current_node)->key_and_value._key);

        if (compare_result == 0)
        {
            break;
        }
        else
        {
            path.push(current_node);
            current_node = &(compare_result > 0 ? (*current_node)->right_subtree_address : (*current_node)->left_subtree_address);
        }
    }

    if ((*current_node) == nullptr)
    {
        std::string message = "Key not found";
        this->warning_with_guard(message);

        throw std::invalid_argument("[AVL TREE] " + message + ".");
    }

    tvalue removed_value = (*current_node)->key_and_value._value;

    _tree->splay(*current_node, path);

    typename binary_search_tree<tkey, tvalue, tkey_comparer>::node * left_subtree = subtree_root_address->left_subtree_address;
    typename binary_search_tree<tkey, tvalue, tkey_comparer>::node * right_subtree = subtree_root_address->right_subtree_address;

    subtree_root_address->~node();

    _tree->deallocate_with_guard(subtree_root_address);

    this->trace_with_guard("[AVL TREE] Node has been deleted.");

    _tree->_root = _tree->merge(left_subtree, right_subtree);

    return removed_value;
}

template<
        typename tkey,
        typename tvalue,
        typename tkey_comparer>
std::tuple<tkey, tvalue> avl_tree<tkey, tvalue, tkey_comparer>::avl_tree_removing_template_method::remove_node_inner(
        const tkey &key,
        typename binary_search_tree<tkey, tvalue, tkey_comparer>::node *&subtree_root_address,
        std::list<typename binary_search_tree<tkey, tvalue, tkey_comparer>::node **> &path_to_subtree_root_exclusive)
{

    if (subtree_root_address == nullptr)
    {
        std::string message = "Tree is empty";

        this->warning_with_guard(message);

        throw std::invalid_argument("[AVL TREE] " + message + ".");
    }

    std::stack<typename binary_search_tree<tkey, tvalue, tkey_comparer>::node **> path;
    typename binary_search_tree<tkey, tvalue, tkey_comparer>::node ** current_node = &subtree_root_address;


    tkey_comparer comparator;

    while (*current_node != nullptr)
    {
        int compare_result = comparator(key, (*current_node)->key_and_value._key);

        if (compare_result == 0)
        {
            break;
        }
        else
        {
            path.push(current_node);
            current_node = &(compare_result > 0 ? (*current_node)->right_subtree_address : (*current_node)->left_subtree_address);
        }
    }

    if ((*current_node) == nullptr)
    {
        std::string message = "Key not found";

        this->warning_with_guard(message);

        throw std::invalid_argument("[AVL TREE] " + message + ".");
    }

    std::tuple<tkey, tvalue> removed_value((*current_node)->key_and_value._key, (*current_node)->key_and_value._value);

    _tree->splay(*current_node, path);

    typename binary_search_tree<tkey, tvalue, tkey_comparer>::node * left_subtree = subtree_root_address->left_subtree_address;
    typename binary_search_tree<tkey, tvalue, tkey_comparer>::node * right_subtree = subtree_root_address->right_subtree_address;

    subtree_root_address->~node();

    _tree->deallocate_with_guard(subtree_root_address);

    this->trace_with_guard("[AVL TREE] Node has been deleted.");

    _tree->_root = _tree->merge(left_subtree, right_subtree);

    return removed_value;
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