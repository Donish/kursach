#ifndef RED_BLACK_TREE_H
#define RED_BLACK_TREE_H

#include "binary_search_tree.h"
#include <memory>

template<
        typename tkey,
        typename tvalue,
        typename tkey_comparer>
class red_black_tree final : public binary_search_tree<tkey, tvalue, tkey_comparer>
{
protected:

    enum class color_node
    {
        RED,
        BLACK
    };

    struct red_black_node : public binary_search_tree<tkey, tvalue, tkey_comparer>::node
    {
        color_node _color;
    };

protected:

    class red_black_tree_insertion_template_method final : public binary_search_tree<tkey, tvalue, tkey_comparer>::insertion_template_method
    {

    private:

        red_black_tree<tkey, tvalue, tkey_comparer> * _tree;

    public:

        explicit red_black_tree_insertion_template_method(red_black_tree<tkey, tvalue, tkey_comparer> * tree);

    protected:

        void after_insert_inner(
                tkey const &key,
                typename binary_search_tree<tkey, tvalue, tkey_comparer>::node *&subtree_root_address,
                std::stack<typename binary_search_tree<tkey, tvalue, tkey_comparer>::node **> &path_to_subtree_root_exclusive) override;

        void initialize_memory_with_node(typename binary_search_tree<tkey, tvalue, tkey_comparer>::node * const node_address) override;

        size_t get_node_size() const override;

    };

    class red_black_tree_reading_template_method final : public binary_search_tree<tkey, tvalue, tkey_comparer>::reading_template_method
    {

    private:

        red_black_tree<tkey, tvalue, tkey_comparer> * _tree;

    public:

        explicit red_black_tree_reading_template_method(red_black_tree<tkey, tvalue, tkey_comparer> * tree);

    };

    class red_black_tree_removing_template_method final : public binary_search_tree<tkey, tvalue, tkey_comparer>::removing_template_method
    {

    private:

        red_black_tree<tkey, tvalue, tkey_comparer> * _tree;
        std::unique_ptr<red_black_node> _info_deleted_node;

    public:

        explicit red_black_tree_removing_template_method(red_black_tree<tkey, tvalue, tkey_comparer> * tree);

    private:

        void get_info_deleted_node(
                typename binary_search_tree<tkey, tvalue, tkey_comparer>::node *deleted_node,
                std::list<typename binary_search_tree<tkey, tvalue, tkey_comparer>::node**> &path_to_subtree_root_exclusive);

    protected:

        void after_remove_inner(
                tkey const &key,
                typename binary_search_tree<tkey, tvalue, tkey_comparer>::node *&subtree_root_address,
                std::list<typename binary_search_tree<tkey, tvalue, tkey_comparer>::node**> &path_to_subtree_root_exclusive) override;

    public:

        ~red_black_tree_removing_template_method();

    };

private:

    void clear();

    typename binary_search_tree<tkey, tvalue, tkey_comparer>::node * copy() const;

    typename binary_search_tree<tkey, tvalue, tkey_comparer>::node * copy_inner(typename binary_search_tree<tkey, tvalue, tkey_comparer>::node * to_copy) const;

    void move(red_black_tree<tkey, tvalue, tkey_comparer> && other);

    color_node get_color_node(red_black_node *current_node);

public:

    explicit red_black_tree(memory *allocator = nullptr, logger *logger_tree = nullptr);

    red_black_tree(red_black_tree const &other);

    red_black_tree(red_black_tree &&other) noexcept;

    red_black_tree &operator=(red_black_tree const &other);

    red_black_tree &operator=(red_black_tree &&other) noexcept;

    ~red_black_tree() final = default;

private:

    memory *_allocator;
    logger *_logger;

};

template<
        typename tkey,
        typename tvalue,
        typename tkey_comparer>
size_t red_black_tree<tkey, tvalue, tkey_comparer>::red_black_tree_insertion_template_method::get_node_size() const
{
    return sizeof(red_black_node);
}

template<
        typename tkey,
        typename tvalue,
        typename tkey_comparer>
typename red_black_tree<tkey, tvalue, tkey_comparer>::color_node red_black_tree<tkey, tvalue, tkey_comparer>::get_color_node(
        red_black_node *current_node)
{
    return current_node == nullptr ? color_node::BLACK : current_node->_color;
}

template<
        typename tkey,
        typename tvalue,
        typename tkey_comparer>
red_black_tree<tkey, tvalue, tkey_comparer> &red_black_tree<tkey, tvalue, tkey_comparer>::operator=(red_black_tree &&other) noexcept
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
red_black_tree<tkey, tvalue, tkey_comparer> &red_black_tree<tkey, tvalue, tkey_comparer>::operator=(const red_black_tree<tkey, tvalue, tkey_comparer> &other)
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
red_black_tree<tkey, tvalue, tkey_comparer>::red_black_tree(red_black_tree<tkey, tvalue, tkey_comparer> &&other) noexcept
{
    move(std::move(other));

    this->trace_with_guard("The tree has been moved.");
}

template<typename tkey, typename tvalue, typename tkey_comparer>
red_black_tree<tkey, tvalue, tkey_comparer>::red_black_tree(const red_black_tree<tkey, tvalue, tkey_comparer> &other)
        : red_black_tree<tkey, tvalue, tkey_comparer>(other.get_allocator(), other.get_logger())
{
    this->_root = other.copy();
}

template<typename tkey, typename tvalue, typename tkey_comparer>
void red_black_tree<tkey, tvalue, tkey_comparer>::move(red_black_tree<tkey, tvalue, tkey_comparer> &&other)
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
red_black_tree<tkey, tvalue, tkey_comparer>::copy_inner(typename binary_search_tree<tkey, tvalue, tkey_comparer>::node *to_copy) const
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
typename binary_search_tree<tkey, tvalue, tkey_comparer>::node *red_black_tree<tkey, tvalue, tkey_comparer>::copy() const
{
    return copy_inner(this->_root);
}

template<typename tkey, typename tvalue, typename tkey_comparer>
void red_black_tree<tkey, tvalue, tkey_comparer>::clear()
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
void red_black_tree<tkey, tvalue, tkey_comparer>::red_black_tree_insertion_template_method::after_insert_inner(
        tkey const &key,
        typename binary_search_tree<tkey, tvalue, tkey_comparer>::node *&subtree_root_address,
        std::stack<typename binary_search_tree<tkey, tvalue, tkey_comparer>::node **> &path_to_subtree_root_exclusive)
{
    if(path_to_subtree_root_exclusive.empty())
    {
        reinterpret_cast<red_black_node*>(subtree_root_address)->_color = red_black_tree<tkey, tvalue, tkey_comparer>::color_node::BLACK;

        return;
    }
    else
    {
        if(subtree_root_address->left_subtree_address == nullptr &&
            subtree_root_address->right_subtree_address == nullptr)
        {
            reinterpret_cast<red_black_node*>(subtree_root_address)->_color = red_black_tree<tkey, tvalue, tkey_comparer>::color_node::RED;
        }
    }

    red_black_node **parent = nullptr;
    red_black_node **uncle = nullptr;
    red_black_node **grand_parent = nullptr;
    red_black_node **great_grand_parent = nullptr;

    if(!path_to_subtree_root_exclusive.empty())
    {
        parent = reinterpret_cast<red_black_node**>(path_to_subtree_root_exclusive.top());
        path_to_subtree_root_exclusive.pop();

        if(!path_to_subtree_root_exclusive.empty())
        {
            grand_parent = reinterpret_cast<red_black_node**>(path_to_subtree_root_exclusive.top());
            path_to_subtree_root_exclusive.pop();

            if((*grand_parent)->left_subtree_address == *parent)
            {
                uncle = reinterpret_cast<red_black_node**>(&((*grand_parent)->right_subtree_address));
            }
            else
            {
                uncle = reinterpret_cast<red_black_node**>(&((*grand_parent)->left_subtree_address));
            }

            if(!path_to_subtree_root_exclusive.empty())
            {
                great_grand_parent = reinterpret_cast<red_black_node**>(path_to_subtree_root_exclusive.top());
                path_to_subtree_root_exclusive.pop();
            }
        }
    }

    if(parent != nullptr &&
        _tree->get_color_node(*parent) == red_black_tree<tkey, tvalue, tkey_comparer>::color_node::BLACK)
    {
        if(great_grand_parent != nullptr)
        {
            path_to_subtree_root_exclusive.push(reinterpret_cast<typename binary_search_tree<tkey, tvalue, tkey_comparer>::node**>(great_grand_parent));
        }

        if(grand_parent != nullptr)
        {
            path_to_subtree_root_exclusive.push(reinterpret_cast<typename binary_search_tree<tkey, tvalue, tkey_comparer>::node**>(grand_parent));
        }

        path_to_subtree_root_exclusive.push(reinterpret_cast<typename binary_search_tree<tkey, tvalue, tkey_comparer>::node**>(parent));

        return;
    }
    else
    {
        if(parent != nullptr && grand_parent != nullptr &&
            _tree->get_color_node(*parent) == red_black_tree<tkey, tvalue, tkey_comparer>::color_node::RED)
        {
            if((*parent)->right_subtree_address == subtree_root_address && *parent == (*grand_parent)->left_subtree_address)
            {
                _tree->left_rotation(*(reinterpret_cast<typename binary_search_tree<tkey, tvalue, tkey_comparer>::node**>(parent)));
                subtree_root_address = (*parent)->left_subtree_address;
            }
            else
            {
                if(subtree_root_address == (*parent)->left_subtree_address && *parent == (*grand_parent)->right_subtree_address)
                {
                    _tree->right_rotation(*(reinterpret_cast<typename binary_search_tree<tkey, tvalue, tkey_comparer>::node**>(parent)));
                    subtree_root_address = (*parent)->right_subtree_address;
                }
            }

            if(_tree->get_color_node(*uncle) == red_black_tree<tkey, tvalue, tkey_comparer>::color_node::BLACK)
            {
                (*parent)->_color = red_black_tree<tkey, tvalue, tkey_comparer>::color_node::BLACK;
                (*grand_parent)->_color = red_black_tree<tkey, tvalue, tkey_comparer>::color_node::RED;

                if(subtree_root_address == (*parent)->left_subtree_address &&
                    *parent == (*grand_parent)->left_subtree_address)
                {
                    _tree->right_rotation(*(reinterpret_cast<typename binary_search_tree<tkey, tvalue, tkey_comparer>::node**>(grand_parent)));
                    *parent = reinterpret_cast<red_black_node*>((*grand_parent)->right_subtree_address);
                }
                else
                {
                    _tree->left_rotation(*(reinterpret_cast<typename binary_search_tree<tkey, tvalue, tkey_comparer>::node**>(grand_parent)));
                    *parent = reinterpret_cast<red_black_node*>((*grand_parent)->left_subtree_address);
                }

                if(great_grand_parent != nullptr)
                {
                    path_to_subtree_root_exclusive.push(reinterpret_cast<typename binary_search_tree<tkey, tvalue, tkey_comparer>::node**>(great_grand_parent));
                }

                path_to_subtree_root_exclusive.push(reinterpret_cast<typename binary_search_tree<tkey, tvalue, tkey_comparer>::node**>(grand_parent));
            }
            else
            {
                if(great_grand_parent != nullptr)
                {
                    path_to_subtree_root_exclusive.push(reinterpret_cast<typename binary_search_tree<tkey, tvalue, tkey_comparer>::node**>(great_grand_parent));
                }

                (*uncle)->_color = red_black_tree<tkey, tvalue, tkey_comparer>::color_node::BLACK;
                (*parent)->_color = red_black_tree<tkey, tvalue, tkey_comparer>::color_node::BLACK;
                (*grand_parent)->_color = red_black_tree<tkey, tvalue, tkey_comparer>::color_node::RED;
                auto **bin_grand_parent = reinterpret_cast<typename binary_search_tree<tkey, tvalue, tkey_comparer>::node**>(grand_parent);

                after_insert_inner(key, *bin_grand_parent, path_to_subtree_root_exclusive);

                path_to_subtree_root_exclusive.push(bin_grand_parent);
                path_to_subtree_root_exclusive.push(reinterpret_cast<typename binary_search_tree<tkey, tvalue, tkey_comparer>::node**>(parent));
            }
        }
    }

    this->trace_with_guard("[red_black TREE] Node inserted.");
}

template<
        typename tkey,
        typename tvalue,
        typename tkey_comparer>
void red_black_tree<tkey, tvalue, tkey_comparer>::red_black_tree_insertion_template_method::initialize_memory_with_node(
        typename binary_search_tree<tkey, tvalue, tkey_comparer>::node * const node_address)
{
    new (node_address) red_black_node;
}

template<
        typename tkey,
        typename tvalue,
        typename tkey_comparer>
void red_black_tree<tkey, tvalue, tkey_comparer>::red_black_tree_removing_template_method::after_remove_inner(
        const tkey &key, typename binary_search_tree<tkey, tvalue, tkey_comparer>::node *&subtree_root_address,
        std::list<typename binary_search_tree<tkey, tvalue, tkey_comparer>::node **> &path_to_subtree_root_exclusive)
{
    if(subtree_root_address == nullptr)
    {
        return;
    }

    if((_info_deleted_node->_color == red_black_tree<tkey, tvalue, tkey_comparer>::color_node::RED ||
        _info_deleted_node->_color == red_black_tree<tkey, tvalue, tkey_comparer>::color_node::BLACK) &&
        (_info_deleted_node->left_subtree_address != nullptr &&
        _info_deleted_node->right_subtree_address != nullptr))
    {
        return;
    }

    if(_info_deleted_node->_color == red_black_tree<tkey, tvalue, tkey_comparer>::color_node::BLACK &&
        ((_info_deleted_node->left_subtree_address == nullptr && _info_deleted_node->right_subtree_address != nullptr) ||
        (_info_deleted_node->left_subtree_address != nullptr && _info_deleted_node->right_subtree_address == nullptr)))
    {
        if(_info_deleted_node->is_right_child == true)
        {
            reinterpret_cast<red_black_node*>(subtree_root_address->right_subtree_address)->_color = red_black_tree<tkey, tvalue, tkey_comparer>::color_node::BLACK;
        }

        if(_info_deleted_node->is_left_child == true)
        {
            reinterpret_cast<red_black_node*>(subtree_root_address->left_subtree_address)->_color = red_black_tree<tkey, tvalue, tkey_comparer>::color_node::BLACK;
        }

        if(_info_deleted_node->is_left_child == false && _info_deleted_node->is_right_child == false)
        {
            reinterpret_cast<red_black_node*>(subtree_root_address)->_color = red_black_tree<tkey, tvalue, tkey_comparer>::color_node::BLACK;
        }

        return;
    }

    if(_info_deleted_node->_color == red_black_tree<tkey, tvalue, tkey_comparer>::color_node::BLACK &&
        _info_deleted_node->left_subtree_address == nullptr &&
        _info_deleted_node->right_subtree_address == nullptr)
    {
        typename binary_search_tree<tkey, tvalue, tkey_comparer>::node **parent = nullptr;

        if(!path_to_subtree_root_exclusive.empty())
        {
            parent = path_to_subtree_root_exclusive.back();
            path_to_subtree_root_exclusive.pop_back();
        }

        if(_info_deleted_node->is_left_child == true)
        {
            if(subtree_root_address->right_subtree_address != nullptr)
            {
                color_node color_subtree = _tree->get_color_node(reinterpret_cast<red_black_node*>(subtree_root_address));
                color_node color_sibling = _tree->get_color_node(reinterpret_cast<red_black_node*>(subtree_root_address->right_subtree_address));
                color_node color_child_right = _tree->get_color_node(reinterpret_cast<red_black_node*>(subtree_root_address->right_subtree_address->right_subtree_address));
                color_node color_child_left = _tree->get_color_node(reinterpret_cast<red_black_node*>(subtree_root_address->right_subtree_address->left_subtree_address));

                if(color_sibling == red_black_tree<tkey, tvalue, tkey_comparer>::color_node::BLACK)
                {
                    if(color_child_right == red_black_tree<tkey, tvalue, tkey_comparer>::color_node::RED ||
                        color_child_left == red_black_tree<tkey, tvalue, tkey_comparer>::color_node::RED)
                    {
                        if(color_child_right == red_black_tree<tkey, tvalue, tkey_comparer>::color_node::RED)
                        {
                            reinterpret_cast<red_black_node*>(subtree_root_address->right_subtree_address)->_color = color_subtree;
                            reinterpret_cast<red_black_node*>(subtree_root_address)->_color = red_black_tree<tkey, tvalue, tkey_comparer>::color_node::BLACK;
                            reinterpret_cast<red_black_node*>(subtree_root_address->right_subtree_address->right_subtree_address)->_color = red_black_tree<tkey, tvalue, tkey_comparer>::color_node::BLACK;
                            _tree->left_rotation(subtree_root_address);
                        }

                        if(color_child_left == red_black_tree<tkey, tvalue, tkey_comparer>::color_node::RED &&
                            color_child_right == red_black_tree<tkey, tvalue, tkey_comparer>::color_node::BLACK)
                        {
                            reinterpret_cast<red_black_node*>(subtree_root_address->right_subtree_address)->_color = red_black_tree<tkey, tvalue, tkey_comparer>::color_node::RED;
                            reinterpret_cast<red_black_node*>(subtree_root_address->right_subtree_address->left_subtree_address)->_color = color_sibling;
                            _tree->right_rotation(subtree_root_address->right_subtree_address);

                            reinterpret_cast<red_black_node*>(subtree_root_address->right_subtree_address)->_color = color_subtree;
                            reinterpret_cast<red_black_node*>(subtree_root_address)->_color = red_black_tree<tkey, tvalue, tkey_comparer>::color_node::BLACK;
                            reinterpret_cast<red_black_node*>(subtree_root_address->right_subtree_address->right_subtree_address)->_color = red_black_tree<tkey, tvalue, tkey_comparer>::color_node::BLACK;
                            _tree->left_rotation(subtree_root_address);
                        }
                    }

                    if(color_child_left == red_black_tree<tkey, tvalue, tkey_comparer>::color_node::BLACK &&
                        color_child_right == red_black_tree<tkey, tvalue, tkey_comparer>::color_node::BLACK)
                    {
                        reinterpret_cast<red_black_node*>(subtree_root_address->right_subtree_address)->_color = red_black_tree<tkey, tvalue, tkey_comparer>::color_node::RED;
                        reinterpret_cast<red_black_node*>(subtree_root_address)->_color = red_black_tree<tkey, tvalue, tkey_comparer>::color_node::BLACK;

                        if(color_subtree == red_black_tree<tkey, tvalue, tkey_comparer>::color_node::BLACK)
                        {
                            _info_deleted_node->_color = reinterpret_cast<red_black_node*>(subtree_root_address)->_color;
                            _info_deleted_node->key_and_value._key = subtree_root_address->key_and_value._key;
                            _info_deleted_node->key_and_value._value = subtree_root_address->key_and_value._value;
                            _info_deleted_node->left_subtree_address = subtree_root_address->left_subtree_address;
                            _info_deleted_node->right_subtree_address = subtree_root_address->right_subtree_address;

                            if(parent != nullptr)
                            {
                                if((*parent)->left_subtree_address == subtree_root_address)
                                {
                                    _info_deleted_node->is_left_child = true;
                                    _info_deleted_node->is_right_child = false;
                                }
                                if((*parent)->right_subtree_address == subtree_root_address)
                                {
                                    _info_deleted_node->is_left_child = false;
                                    _info_deleted_node->is_right_child = true;
                                }

                                after_remove_inner(key, *parent, path_to_subtree_root_exclusive);
                            }
                            else
                            {
                                path_to_subtree_root_exclusive.push_back(parent);
                                after_remove_inner(key, subtree_root_address, path_to_subtree_root_exclusive);
                                path_to_subtree_root_exclusive.pop_back();
                            }
                        }
                    }
                }
                else
                {
                    //STOP: line 462
                    reinterpret_cast<red_black_node*>(subtree_root_address->right_subtree_address)->_color = red_black_tree<tkey, tvalue, tkey_comparer>::color_node::BLACK;
                    reinterpret_cast<red_black_node*>(subtree_root_address)->_color = red_black_tree<tkey, tvalue, tkey_comparer>::color_node::RED;
                    _tree->left_rotation(subtree_root_address);

                    path_to_subtree_root_exclusive.push_back(parent);
                    path_to_subtree_root_exclusive.push_back(&subtree_root_address);
                    after_remove_inner(key, subtree_root_address->left_subtree_address, path_to_subtree_root_exclusive);
                    path_to_subtree_root_exclusive.pop_back();
                    path_to_subtree_root_exclusive.pop_back();
                }
            }
        }
        else
        {
            if(subtree_root_address->left_subtree_address != nullptr)
            {
                color_node color_subtree = _tree->get_color_node(reinterpret_cast<red_black_node*>(subtree_root_address));
                color_node color_sibling = _tree->get_color_node(reinterpret_cast<red_black_node*>(subtree_root_address->left_subtree_address));
                color_node color_child_right = _tree->get_color_node(reinterpret_cast<red_black_node*>(subtree_root_address->left_subtree_address->right_subtree_address));
                color_node color_child_left = _tree->get_color_node(reinterpret_cast<red_black_node*>(subtree_root_address->left_subtree_address->left_subtree_address));

                if(color_sibling == red_black_tree<tkey, tvalue, tkey_comparer>::color_node::BLACK)
                {
                    if(color_child_right == red_black_tree<tkey, tvalue, tkey_comparer>::color_node::RED ||
                        color_child_left == red_black_tree<tkey, tvalue, tkey_comparer>::color_node::RED)
                    {
                        if(color_child_left == red_black_tree<tkey, tvalue, tkey_comparer>::color_node::RED)
                        {
                            //490
                            reinterpret_cast<red_black_node*>(subtree_root_address->left_subtree_address)->_color = color_subtree;
                            reinterpret_cast<red_black_node*>(subtree_root_address)->_color = red_black_tree<tkey, tvalue, tkey_comparer>::color_node::BLACK;
                            reinterpret_cast<red_black_node*>(subtree_root_address->left_subtree_address->left_subtree_address)->_color = red_black_tree<tkey, tvalue, tkey_comparer>::color_node::BLACK;
                            _tree->right_rotation(subtree_root_address);
                        }

                        if(color_child_right == red_black_tree<tkey, tvalue, tkey_comparer>::color_node::RED &&
                            color_child_left == red_black_tree<tkey, tvalue, tkey_comparer>::color_node::BLACK)
                        {
                            reinterpret_cast<red_black_node*>(subtree_root_address->left_subtree_address)->_color = red_black_tree<tkey, tvalue, tkey_comparer>::color_node::RED;
                            reinterpret_cast<red_black_node*>(subtree_root_address->left_subtree_address->right_subtree_address)->_color = color_sibling;
                            _tree->left_rotation(subtree_root_address->left_subtree_address);

                            reinterpret_cast<red_black_node*>(subtree_root_address->left_subtree_address)->_color = color_subtree;
                            reinterpret_cast<red_black_node*>(subtree_root_address)->_color = red_black_tree<tkey, tvalue, tkey_comparer>::color_node::BLACK;
                            reinterpret_cast<red_black_node*>(subtree_root_address->left_subtree_address->left_subtree_address)->_color = red_black_tree<tkey, tvalue, tkey_comparer>::color_node::BLACK;
                            _tree->right_rotation(subtree_root_address);
                        }
                    }
                    if(color_child_left == red_black_tree<tkey, tvalue, tkey_comparer>::color_node::BLACK &&
                        color_child_right == red_black_tree<tkey, tvalue, tkey_comparer>::color_node::BLACK)
                    {
                        reinterpret_cast<red_black_node*>(subtree_root_address->left_subtree_address)->_color = red_black_tree<tkey, tvalue, tkey_comparer>::color_node::RED;
                        reinterpret_cast<red_black_node*>(subtree_root_address)->_color = red_black_tree<tkey, tvalue, tkey_comparer>::color_node::BLACK;

                        if(color_subtree == red_black_tree<tkey, tvalue, tkey_comparer>::color_node::BLACK)
                        {
                            _info_deleted_node->_color = reinterpret_cast<red_black_node*>(subtree_root_address)->_color;
                            _info_deleted_node->key_and_value._key = subtree_root_address->key_and_value._key;
                            _info_deleted_node->key_and_value._value = subtree_root_address->key_and_value._value;
                            _info_deleted_node->left_subtree_address = subtree_root_address->left_subtree_address;
                            _info_deleted_node->right_subtree_address = subtree_root_address->right_subtree_address;

                            if(parent != nullptr)
                            {
                                if((*parent)->left_subtree_address == subtree_root_address)
                                {
                                    _info_deleted_node->is_left_child = true;
                                    _info_deleted_node->is_right_child = false;
                                }
                                if((*parent)->right_subtree_address == subtree_root_address)
                                {
                                    _info_deleted_node->is_left_child = false;
                                    _info_deleted_node->is_right_child = true;
                                }

                                after_remove_inner(key, *parent, path_to_subtree_root_exclusive);
                            }
                            else
                            {
                                path_to_subtree_root_exclusive.push_back(parent);
                                after_remove_inner(key, subtree_root_address, path_to_subtree_root_exclusive);
                                path_to_subtree_root_exclusive.pop_back();
                            }
                        }
                    }
                }
                else
                {
                    //550
                    reinterpret_cast<red_black_node*>(subtree_root_address->left_subtree_address)->_color = red_black_tree<tkey, tvalue, tkey_comparer>::color_node::BLACK;
                    reinterpret_cast<red_black_node*>(subtree_root_address)->_color = red_black_tree<tkey, tvalue, tkey_comparer>::color_node::RED;
                    _tree->right_rotation(subtree_root_address);

                    path_to_subtree_root_exclusive.push_back(parent);
                    path_to_subtree_root_exclusive.push_back(&subtree_root_address);
                    after_remove_inner(key, subtree_root_address->right_subtree_address, path_to_subtree_root_exclusive);
                    path_to_subtree_root_exclusive.pop_back();
                    path_to_subtree_root_exclusive.pop_back();
                }
            }
        }
        if(parent != nullptr)
        {
            path_to_subtree_root_exclusive.push_back(parent);
        }
    }
}

template<
        typename tkey,
        typename tvalue,
        typename tkey_comparer>
void red_black_tree<tkey, tvalue, tkey_comparer>::red_black_tree_removing_template_method::get_info_deleted_node(
        typename binary_search_tree<tkey, tvalue, tkey_comparer>::node *deleted_node,
        std::list<typename binary_search_tree<tkey, tvalue, tkey_comparer>::node **> &path_to_subtree_root_exclusive)
{
    _info_deleted_node = std::make_unique<red_black_node>();

    if(path_to_subtree_root_exclusive.empty())
    {
        _info_deleted_node->is_left_child = false;
        _info_deleted_node->is_right_child = false;
    }
    else
    {
        if((*path_to_subtree_root_exclusive.back())->right_subtree_address == deleted_node)
        {
            _info_deleted_node->is_left_child = false;
            _info_deleted_node->is_right_child = true;
        }

        if((*path_to_subtree_root_exclusive.back())->left_subtree_address == deleted_node)
        {
            _info_deleted_node->is_left_child = true;
            _info_deleted_node->is_right_child = false;
        }
    }

    _info_deleted_node->key_and_value._key = deleted_node->key_and_value._key;
    _info_deleted_node->key_and_value._value = deleted_node->key_and_value._value;
    _info_deleted_node->left_subtree_address = deleted_node->left_subtree_address;
    _info_deleted_node->right_subtree_address = deleted_node->right_subtree_address;
    _info_deleted_node->_color = reinterpret_cast<red_black_node*>(deleted_node)->_color;
}

template<
        typename tkey,
        typename tvalue,
        typename tkey_comparer>
red_black_tree<tkey, tvalue, tkey_comparer>::red_black_tree_insertion_template_method::red_black_tree_insertion_template_method(
        red_black_tree<tkey, tvalue, tkey_comparer> *tree):
        _tree(tree),
        binary_search_tree<tkey, tvalue, tkey_comparer>::insertion_template_method(tree)
{

}

template<
        typename tkey,
        typename tvalue,
        typename tkey_comparer>
red_black_tree<tkey, tvalue, tkey_comparer>::red_black_tree_reading_template_method::red_black_tree_reading_template_method(
        red_black_tree<tkey, tvalue, tkey_comparer> *tree):
        _tree(tree),
        binary_search_tree<tkey, tvalue, tkey_comparer>::reading_template_method(tree)
{

}

template<
        typename tkey,
        typename tvalue,
        typename tkey_comparer>
red_black_tree<tkey, tvalue, tkey_comparer>::red_black_tree_removing_template_method::red_black_tree_removing_template_method(
        red_black_tree<tkey, tvalue, tkey_comparer> *tree):
        _tree(tree),
        binary_search_tree<tkey, tvalue, tkey_comparer>::removing_template_method(tree)
{

}

template<
        typename tkey,
        typename tvalue,
        typename tkey_comparer>
red_black_tree<tkey, tvalue, tkey_comparer>::red_black_tree(memory *allocator, logger *logger):
        binary_search_tree<tkey, tvalue, tkey_comparer>(
                new red_black_tree_insertion_template_method(this),
                new red_black_tree_reading_template_method(this),
                new red_black_tree_removing_template_method(this),
                allocator,
                logger)
{
    this->trace_with_guard("[red_black TREE] The tree has been created.");
}

#endif //RED_BLACK_TREE_H