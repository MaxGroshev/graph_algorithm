#pragma once

#include "utils.hpp"
#include <stack>

//-----------------------------------------------------------------------------------------

namespace avl {

template <typename key_type = int>
class node_t {
    public:
        enum node_col {
            BLACK_ = 0,
            RED_   = 1,
        };
        key_type key_;
        node_t<key_type>* left_  = nullptr;
        node_t<key_type>* right_ = nullptr;
        node_t<key_type>* parent_= nullptr;
        size_t size_   = 1;
        node_col color_  = node_col::BLACK_;

    private:
        node_t(key_type key, size_t size, node_col color) :
            key_(key),
            color_(color),
            size_(size)
            {};

    public:
        node_t(key_type key) : key_(key) {};
        node_t(key_type key, node_col color) : key_(key), color_(color) {};
        node_t(const node_t<key_type>& node) : key_(node.key_), color_(node.color_) {

            node_t<key_type>* ret_node = safe_copy(&node);
            left_  = ret_node->left_;
            right_ = ret_node->right_;

            delete ret_node;
        }
        node_t(node_t<key_type>&& node): key_(node.key_), color_(node.color_),
                                           parent_(node.parent),
                                           left_(node.left_),    right_(node.right_) {
            node.right_ = nullptr;
            node.left_  = nullptr;
            node.parent_ = nullptr;
        }
        node_t<key_type>* safe_copy (const node_t<key_type>* node);
        node_t<key_type>& operator= (const node_t<key_type>& node);
        node_t<key_type>& operator= (node_t<key_type>&& node);
        ~node_t() = default;

        node_t<key_type>* balance_subtree(avl::node_t<key_type>* cur, avl::node_t<key_type>* root);
        node_t<key_type>* insert(avl::node_t<key_type>* cur_node, key_type key);
        size_t get_size(node_t<key_type>* node) const {
            if (node) return node->size_; else return 0;
        }
        node_col get_color() const {
            return color_;
        }
        key_type const & get_key() const {
            return key_;
        }
        
        std::vector<key_type> store_inorder_walk() const;
        void graphviz_dump(graphviz::dump_graph_t& tree_dump) const ;
        node_t<key_type>* upper_bound(key_type key);
        node_t<key_type>* lower_bound(key_type key);

        size_t define_node_rank(node_t<key_type>* root) const;
        
        private:
    };
}

//-----------------------------------------------------------------------------------------

namespace avl {

template<typename key_type>
node_t<key_type>& node_t<key_type>::operator= (const node_t<key_type>& node) {
    if (this == &node)
        return *this;

    node_t<key_type>* tmp_left_  = new node_t<key_type> (*(node.left_));
    node_t<key_type>* tmp_right_ = new node_t<key_type> (*(node.right_));
    assert(tmp_left_ != nullptr && tmp_right_ != nullptr);

    key_ = node.key_;
    color_ = node.color_;

    delete left_;
    delete right_;
    delete parent_;

    left_ = tmp_left_;
    right_ = tmp_right_;

    return *this;
}

template<typename key_type>
node_t<key_type>& node_t<key_type>::operator= (node_t<key_type>&& node) {
    if (this == &node)
        return *this;

    delete left_;
    delete right_;
    delete parent_;

    left_ = node.left_;
    right_ = node.right_;
    parent_ = node.parent_;
    key_ = node.key_;
    color_ = node.color_;

    node.right_ = nullptr;
    node.left_  = nullptr;
    node.parent_ = nullptr;
    return *this;
}

template<typename key_type>
node_t<key_type>* node_t<key_type>::safe_copy(const node_t<key_type>* origine_node) {

    node_t<key_type>* new_node = new node_t<key_type>(origine_node->key_,
            origine_node->color_);

    node_t<key_type>* root = new_node;

    try {
        while (origine_node != nullptr) {
            if (new_node->left_ == nullptr && origine_node->left_ != nullptr) {
                new_node->left_ = new node_t<key_type>(
                                origine_node->left_->key_, origine_node->left_->color_);
                new_node->left_->parent_ = new_node;

                new_node     = new_node->left_;
                origine_node = origine_node->left_;
            }
            else if (new_node->right_ == nullptr && origine_node->right_ != nullptr) {
                new_node->right_ = new node_t<key_type>(
                                origine_node->right_->key_, origine_node->right_->color_);
                new_node->right_->parent_ = new_node;

                new_node = new_node->right_;
                origine_node = origine_node->right_;
            }
            else {
                new_node = new_node->parent_;
                origine_node = origine_node->parent_;
            }
        }
    } catch(...) {
        delete new_node;
        throw;
    }
    return root;
}

//--------------------RANGES---------------------------------------------------------------

template<typename key_type>
node_t<key_type>* node_t<key_type>::upper_bound(key_type key) {

    node_t<key_type>* node = nullptr;
    if (key_ < key) {
        if (right_ != nullptr)
            node = right_->upper_bound(key);
        else
            return this;
    }
    else if (key_ > key) {
        if (left_ != nullptr)
            node = left_->upper_bound(key);
        else
            return this;
    }
    else if (key_ == key) {
        return this;
    }

    if (node->key_ > key && key_ < key) {
        return this;
    }
    return node;
}

template<typename key_type>
node_t<key_type>* node_t<key_type>::lower_bound(key_type key) {

    node_t<key_type>* node = nullptr;
    if (key_ < key) {
        if (right_ != nullptr)
            node = right_->lower_bound(key);
        else
            return this;
    }
    else if (key_ > key) {
        if (left_ != nullptr)
            node = left_->lower_bound(key);
        else
            return this;
    }
    else if (key_ == key) {
        return this;
    }

    if (node->key_ < key && key_ > key) {
        return this;
    }
    return node;
}

//-----------------------------------------------------------------------------------------

template<typename key_type>
size_t node_t<key_type>::define_node_rank(node_t<key_type>* root) const {

    size_t rank = 1;
    if (left_ != nullptr) {
        rank += left_->size_;
    }
    const node_t<key_type>* cur_node = this;
    while (cur_node != root) {
        if (cur_node == cur_node->parent_->right_) {
            rank += get_size (cur_node->parent_->left_) + 1;
        }
        cur_node = cur_node->parent_;
        // std::cout << "rank: " << rank << "\n";
    }
    return rank;
}

//--------------------WALKING--------------------------------------------------------------

template<typename key_type>
std::vector<key_type> node_t<key_type>::store_inorder_walk() const {
    std::vector<key_type> storage;
    std::stack<const node_t<key_type>*> node_stk;
    const node_t<key_type>* cur_node = this;

    while (cur_node || !node_stk.empty()) {
        if (!node_stk.empty()) {
            cur_node = node_stk.top();
            storage.push_back(cur_node->key_);
            if (cur_node->right_)
                cur_node = cur_node->right_;
            else
                cur_node = nullptr;

            node_stk.pop();
        }
        while (cur_node) {
            node_stk.push(cur_node);
            cur_node = cur_node->left_;
        }
    }

    return storage;
}

template<typename key_type>
void node_t<key_type>::graphviz_dump(graphviz::dump_graph_t& tree_dump) const {
    tree_dump.graph_node.print_node(this, tree_dump.graphviz_strm);

    if (left_ != nullptr)
    {
        tree_dump.graph_edge.fillcolor = "#7FC7FF";
        tree_dump.graph_edge.color     = "#7FC7FF";
        tree_dump.graph_edge.print_edge(this, left_, tree_dump.graphviz_strm);
        left_->graphviz_dump(tree_dump);
    }
    if (right_ != nullptr)
    {
        tree_dump.graph_edge.fillcolor = "#DC143C";
        tree_dump.graph_edge.color     = "#DC143C";
        tree_dump.graph_edge.print_edge(this, right_, tree_dump.graphviz_strm);
        right_->graphviz_dump(tree_dump);
    }
}

}