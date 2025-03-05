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
        node_t<key_type>* tree_minimum(node_t<key_type>* cur) {
            while (cur->left_ != nullptr) {
                cur = cur->left_;
            }
            return cur;
        }

        node_t<key_type>* tree_maximum(node_t<key_type>* cur) {
            while (cur->right_ != nullptr) {
                cur = cur->right_;
            }
            return cur;
        }
        node_t<key_type>* tree_successor(node_t<key_type>* cur) {
            if (cur->right_ != nullptr) {
                return tree_minimum(cur->right_);
            }
            auto cur_parent = cur->parent_;
            while (cur_parent != nullptr && cur == cur_parent->right_) {
                cur = cur_parent->right_;
                cur_parent = cur_parent->parent_;
            }
            return cur_parent;
        }
        node_t<key_type>* transplant(node_t<key_type>* u, node_t<key_type>* v, node_t<key_type>* root) {
            if (u->parent_ == nullptr)
                root = v;
            else if (u == u->parent_->left_)
                u->parent_->left_ = v;
            else
                u->parent_->right_ = v;
            if (v != nullptr)
                v->parent_ = u->parent_;
            return root;
        }

        node_t<key_type>* rotate_to_left(node_t<key_type>* cur_node, node_t<key_type>* root);
        node_t<key_type>* rotate_to_right(node_t<key_type>* cur_node, node_t<key_type>* root);
        node_t<key_type>* erase(node_t<key_type>* root, const key_type& key);
        node_t<key_type>* fix_erase(node_t<key_type>* x, node_t<key_type>* root);
        
        std::vector<key_type> store_inorder_walk() const;
        void graphviz_dump(graphviz::dump_graph_t& tree_dump) const ;
        node_t<key_type>* upper_bound(key_type key);
        node_t<key_type>* lower_bound(key_type key);

        size_t define_node_rank(node_t<key_type>* root) const;
        
        private:
        node_t<key_type>* erase_node_impl(node_t<key_type>* cur, node_t<key_type>* root);
        // void get_sibling();
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
            origine_node->data_, origine_node->size_,origine_node->height_);

    node_t<key_type>* root = new_node;

    try {
        while (origine_node != nullptr) {
            if (new_node->left_ == nullptr && origine_node->left_ != nullptr) {
                new_node->left_ = new node_t<key_type>(
                                origine_node->left_->key_, origine_node->left_->data_,
                                origine_node->left_->size_, origine_node->left_->height_);
                new_node->left_->parent_ = new_node;

                new_node     = new_node->left_;
                origine_node = origine_node->left_;
            }
            else if (new_node->right_ == nullptr && origine_node->right_ != nullptr) {
                new_node->right_ = new node_t<key_type>(
                                origine_node->right_->key_, origine_node->right_ ->data_,
                                origine_node->right_->size_, origine_node->right_->height_);
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

//-----------------------------------------------------------------------------------------

template<typename key_type>
node_t<key_type>*
node_t<key_type>::insert(avl::node_t<key_type>* root, key_type key) {
    if(!root)
        throw("Invalid ptr");

    node_t<key_type>* parent = root->parent_;
    node_t<key_type>* cur = root;
    while (cur != nullptr) {
        parent = cur;
        if (key > cur->get_key()) {
            cur = cur->right_;
            continue;
        }
        cur = cur->left_;
    }

    auto new_node =  new node_t<key_type>(key);
    if (key < parent->get_key()) {
        parent->left_ = new_node;
        parent->left_->parent_ = parent;
        parent->left_->color_ = node_col::RED_;
        if (parent->left_->parent_->parent_ == nullptr) {
            return root;
        }
        return balance_subtree(parent->left_, root);
    } 
    else {
        parent->right_ = new_node;
        parent->right_->parent_ = parent;
        parent->right_->color_ = node_col::RED_;
        if (parent->right_->parent_->parent_ == nullptr) {
            return root;
        }
        return balance_subtree(parent->right_, root);
    }
}

template<typename key_type>
node_t<key_type>*
node_t<key_type>::erase(node_t<key_type>* root, const key_type& key) {
    if(!root)
        throw("Invalid ptr");
    //make search func
    node_t<key_type>* parent = root->parent_;
    node_t<key_type>* cur = root;
    while (cur->get_key() != key) {
        parent = cur;
        if (key > cur->get_key()) {
            cur = cur->right_;
        }
        else if (key < cur->get_key()) {
            cur = cur->left_;
        }
    }
    return erase_node_impl(cur, root);
}

template<typename key_type>
node_t<key_type>*
node_t<key_type>::erase_node_impl(node_t<key_type>* z, node_t<key_type>* root) {
    if (z == nullptr)
        return root;

    node_t<key_type>* y = z;
    node_t<key_type>* x = nullptr;
    node_col y_original_color = y->color_;

    if (z->left_ == nullptr) {
        x = z->right_;
        root = transplant(z, z->right_, root);
    } else if (z->right_ == nullptr) {
        x = z->left_;
        root = transplant(z, z->left_, root);
    } else {
        y = tree_minimum(z->right_);
        y_original_color = y->color_;
        x = y->right_;

        if (y->parent_ == z) {
            if (x != nullptr) {
                x->parent_ = y; // Check if x is not nullptr before assigning parent
            }
        } else {
            if (x != nullptr)
                x->parent_ = y->parent_; // Check if x and y->parent are not nullptr before assigning parent
            root = transplant(y, y->right_, root);
            if (y->right_ != nullptr)
                y->right_->parent_ = y; // Check if y->right_ is not nullptr before assigning parent
            y->right_ = z->right_;
            if (y->right_ != nullptr)
                y->right_->parent_ = y; // Check if y->right_ is not nullptr before assigning parent
        }
        root = transplant(z, y, root);
        y->left_ = z->left_;
        if (y->left_ != nullptr)
            y->left_->parent_ = y; // Check if y->left is not nullptr before assigning parent
        y->color_ = z->color_;
    }

    if (y_original_color == node_col::BLACK_ && x != nullptr) 
        root = fix_erase(x, root);

    delete z;
    return root;
}

template<typename key_type>
node_t<key_type>*
node_t<key_type>::fix_erase(node_t<key_type>* x, node_t<key_type>* root) {
    while (x != root && x != nullptr && x->color_== node_col::BLACK_) {
        if (x == x->parent_->left_) {
            auto w = x->parent_->right_;
            if (w->color_==node_col::RED_) {
                w->color_=node_col::BLACK_;
                x->parent_->color_=node_col::RED_;
                root = rotate_to_left(x->parent_, root);
                w = x->parent_->right_;
            }
            if ((w->left_ == nullptr || w->left_->color_==node_col::BLACK_) &&
                (w->right_== nullptr || w->right_->color_==node_col::BLACK_)) {
                w->color_= node_col::RED_;
                x = x->parent_;
            } else {
                if (w->right_== nullptr || w->right_->color_==node_col::BLACK_) {
                    if (w->left_ != nullptr)
                        w->left_->color_=node_col::BLACK_;
                    w->color_= node_col::RED_;
                    root = rotate_to_right(w, root);
                    w = x->parent_->right_;
                }
                w->color_= x->parent_->color_;
                x->parent_->color_= node_col::BLACK_;
                if (w->right_!= nullptr)
                    w->right_->color_= node_col::BLACK_;
                root = rotate_to_left(x->parent_, root);
                x = root;
            }
        } else {
            auto w = x->parent_->left_;
            if (w->color_== node_col::RED_) {
                w->color_= node_col::BLACK_;
                x->parent_->color_= node_col::RED_;
                root = rotate_to_right(x->parent_, root);
                w = x->parent_->left_;
            }
            if ((w->right_== nullptr || w->right_->color_ == node_col::BLACK_) &&
                (w->left_ == nullptr || w->left_->color_ == node_col::BLACK_)) {
                w->color_ = node_col::RED_;
                x = x->parent_;
            } else {
                if (w->left_ == nullptr || w->left_->color_ == node_col::BLACK_) {
                    if (w->right_!= nullptr)
                        w->right_->color_ = node_col::BLACK_;
                    w->color_ = node_col::RED_;
                    root = rotate_to_left(w, root);
                    w = x->parent_->left_;
                }
                w->color_= x->parent_->color_;
                x->parent_->color_ = node_col::BLACK_;
                if (w->left_ != nullptr)
                    w->left_->color_ = node_col::BLACK_;
                root = rotate_to_right(x->parent_, root);
                x = root;
            }
        }
    }
    if (x != nullptr)
        x->color_=node_col::BLACK_;
    return root;

}

//----------------------------ROTATES------------------------------------------------------

template<typename key_type>
avl::node_t<key_type>*
node_t<key_type>::balance_subtree(avl::node_t<key_type>* cur, avl::node_t<key_type>* root) {
    auto cur_parent = cur->parent_;
    while (cur_parent && cur_parent->color_ == node_col::RED_) {
        auto cur_gparent = cur_parent->parent_;
        if (cur_parent == cur_gparent->left_) {
            auto uncle = cur_gparent->right_;
            if (uncle && uncle->color_ == node_col::RED_) {
                cur_parent->color_ = node_col::BLACK_;
                uncle->color_ = node_col::BLACK_;
                cur_gparent->color_ = node_col::RED_;
                cur_parent = cur_gparent->parent_;
            } else {
                if (cur == cur_parent->right_) {
                    root = rotate_to_left(cur_gparent->left_, root);
                    cur_parent = cur_gparent->left_;
                }
                cur_parent->color_ = node_col::BLACK_;
                cur_gparent->color_ = node_col::RED_;
                root = rotate_to_right(cur_gparent, root);
            }
        } else {
            auto uncle = cur_gparent->left_;
            if (uncle && uncle->color_ == node_col::RED_) {
                cur_parent->color_ = node_col::BLACK_;
                uncle->color_ = node_col::BLACK_;
                cur_gparent->color_ = node_col::RED_;
                cur_parent = cur_gparent->parent_;
            } else {
                if (cur == cur_parent->left_) {
                    root = rotate_to_right(cur_gparent->right_, root);
                    cur_parent = cur_gparent->right_;
                }
                cur_parent->color_ = node_col::BLACK_;
                cur_gparent->color_ = node_col::RED_;
                root = rotate_to_left(cur_gparent, root);
            }
        }
    }
    root->color_ = node_col::BLACK_;
    return root;
}


template<typename key_type>
node_t<key_type>*
node_t<key_type>::rotate_to_left(node_t<key_type>* cur, node_t<key_type>* root) {

    node_t<key_type>* y = cur->right_;
    cur->right_ = y->left_;
    if (y->left_ != nullptr) {
        y->left_->parent_ = cur;
    }
    y->parent_ = cur->parent_;
    if (cur->parent_ == nullptr) {
        root = y;
    }
    else if (cur == cur->parent_->left_) {
        cur->parent_->left_ = y;
    }
    else {
        cur->parent_->right_ = y;
    }
    y->left_ = cur;
    cur->parent_ = y;
    
    return root;
}

template<typename key_type>
node_t<key_type>*
node_t<key_type>::rotate_to_right(node_t<key_type>* cur, node_t<key_type>* root) {

    node_t<key_type>* y = cur->left_;
    cur->left_ = y->right_;
    if (y->right_ != nullptr) {
        y->right_->parent_ = cur;
    }
    y->parent_ = cur->parent_;
    if (cur->parent_ == nullptr) {
        root = y;
    }
    else if (cur == cur->parent_->right_) {
        cur->parent_->right_ = y;
    }
    else {
        cur->parent_->left_ = y;
    }
    y->right_ = cur;
    cur->parent_ = y;

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