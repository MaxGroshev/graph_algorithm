#pragma once
#include "avl_node.hpp"

//-----------------------------------------------------------------------------------------

namespace avl {

template<typename key_type = int>
class tree_t final {
    private:
        node_t<key_type>* root_ = nullptr;
        node_t<key_type>* tnil_ = nullptr;
    public:
        tree_t(){};
        tree_t(key_type key) {
            root_ = new node_t(key);
            assert(root_ != nullptr);
            // tnil_ = new node_t(11111); //tmp probably inher
            // root_->parent_ = tnil_;
        };
        tree_t(const tree_t<key_type>& tree) {

            root_ = new node_t<key_type> (*(tree.root_));
            if (root_->left_ != nullptr)
                root_->left_->parent_ = root_;
            if (root_->right_ != nullptr)
                root_->right_->parent_ = root_;

        };
        tree_t(tree_t<key_type>&& tree) noexcept {
            root_ = tree.root_;
            tree.root_ = nullptr;
            assert(root_ != nullptr);
        };
        tree_t<key_type>& operator= (const tree_t<key_type>& tree);
        tree_t<key_type>& operator= (tree_t<key_type>&& tree);
        ~tree_t();

        void   insert(key_type key);
        void   erase(const key_type& key);
        size_t range_query(int l_bound, int u_bound) const;
        size_t distance(node_t<key_type>* l_node, node_t<key_type>* u_node) const;
        node_t<key_type>* upper_bound(key_type key) const;
        node_t<key_type>* lower_bound(key_type key) const;
        std::vector<key_type> store_inorder_walk() const;
        void graphviz_dump() const;
        
    private:
        void fix_insert(avl::node_t<key_type>* cur);
        void erase_impl(avl::node_t<key_type>* cur);
        void fix_erase(avl::node_t<key_type>* cur);
        void rotate_to_left(node_t<key_type>* cur_node);
        void rotate_to_right(node_t<key_type>* cur_node);


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
        void transplant(node_t<key_type>* u, node_t<key_type>* v) {
            if (u->parent_ == nullptr)
                root_ = v;
            else if (u == u->parent_->left_)
                u->parent_->left_ = v;
            else
                u->parent_->right_ = v;
            if (v != nullptr)
                v->parent_ = u->parent_;
            return;
        }
};

//-----------------------------------------------------------------------------------------

template<typename key_type>
tree_t<key_type>::~tree_t<key_type> () {

    if (root_ == nullptr) return;
    // graphviz_dump();
    std::stack<node_t<key_type>*> nodes;
    nodes.push(root_);
    node_t<key_type>* front = nullptr;

    while(!nodes.empty()) {
        front = nodes.top();
        nodes.pop();
        if (front != nullptr) { //case of deleteing after move constr
            if (front->left_ != nullptr) {
                nodes.push(front->left_);
            }
            if (front->right_ != nullptr) {
                nodes.push(front->right_);
            }
        }
        delete front;
    }
}

template<typename key_type>
tree_t<key_type>& tree_t<key_type>::operator= (const tree_t<key_type>& tree) {
    if (this == &tree)
        return *this;

    node_t<key_type>* tmp_root_ = new node_t<key_type> (*(tree.root_));
    assert(tmp_root_ == nullptr);
    delete root_;
    root_ = tmp_root_;
    return *this;
}

template<typename key_type>
tree_t<key_type>&tree_t<key_type>::operator= (tree_t<key_type>&& tree) {
    if (this == &tree)
        return *this;

    node_t<key_type>* tmp_root_ = tree.root_;
    tree.root_ = nullptr;
    delete root_;
    root_ = tmp_root_;
    return *this;
}

//-----------------------------------------------------------------------------------------

template<typename key_type>
void tree_t<key_type>::insert(key_type key) {
    if (root_ == nullptr) {
        node_t<key_type>* tmp_root_ = new node_t<key_type> (key);
        assert(tmp_root_ != nullptr);
        root_ = tmp_root_;
        return;
    }

    node_t<key_type>* parent = root_->parent_;
    node_t<key_type>* cur = root_;
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
        parent->left_->color_ = node_t<key_type>::node_col::RED_;
        if (parent->left_->parent_->parent_ == nullptr) {
            return;
        }
        return fix_insert(parent->left_);
    } 
    else {
        parent->right_ = new_node;
        parent->right_->parent_ = parent;
        parent->right_->color_ = node_t<key_type>::node_col::RED_;
        if (parent->right_->parent_->parent_ == nullptr) {
            return;
        }
        return fix_insert(parent->right_);
    }
    return;
}

template<typename key_type>
void
tree_t<key_type>::fix_insert(avl::node_t<key_type>* cur) {
    auto cur_parent = cur->parent_;
    while (cur_parent && cur_parent->color_ == node_t<key_type>::node_col::RED_) {
        auto cur_gparent = cur_parent->parent_;
        if (cur_parent == cur_gparent->left_) {
            auto uncle = cur_gparent->right_;
            if (uncle && uncle->color_ == node_t<key_type>::node_col::RED_) {
                cur_parent->color_ = node_t<key_type>::node_col::BLACK_;
                uncle->color_ = node_t<key_type>::node_col::BLACK_;
                cur_gparent->color_ = node_t<key_type>::node_col::RED_;
                cur_parent = cur_gparent->parent_;
            } else {
                if (cur == cur_parent->right_) {
                    rotate_to_left(cur_gparent->left_);
                    cur_parent = cur_gparent->left_;
                }
                cur_parent->color_ = node_t<key_type>::node_col::BLACK_;
                cur_gparent->color_ = node_t<key_type>::node_col::RED_;
                rotate_to_right(cur_gparent);
            }
        } else {
            auto uncle = cur_gparent->left_;
            if (uncle && uncle->color_ == node_t<key_type>::node_col::RED_) {
                cur_parent->color_ = node_t<key_type>::node_col::BLACK_;
                uncle->color_ = node_t<key_type>::node_col::BLACK_;
                cur_gparent->color_ = node_t<key_type>::node_col::RED_;
                cur_parent = cur_gparent->parent_;
            } else {
                if (cur == cur_parent->left_) {
                    rotate_to_right(cur_gparent->right_);
                    cur_parent = cur_gparent->right_;
                }
                cur_parent->color_ = node_t<key_type>::node_col::BLACK_;
                cur_gparent->color_ = node_t<key_type>::node_col::RED_;
                rotate_to_left(cur_gparent);
            }
        }
    }
    root_->color_ = node_t<key_type>::node_col::BLACK_;
    return;
}

//-----------------------------------------------------------------------------------------

template<typename key_type>
void
tree_t<key_type>::rotate_to_left(node_t<key_type>* cur) {

    node_t<key_type>* y = cur->right_;
    cur->right_ = y->left_;
    if (y->left_ != nullptr) {
        y->left_->parent_ = cur;
    }
    y->parent_ = cur->parent_;
    if (cur->parent_ == nullptr) {
        root_ = y;
    }
    else if (cur == cur->parent_->left_) {
        cur->parent_->left_ = y;
    }
    else {
        cur->parent_->right_ = y;
    }
    y->left_ = cur;
    cur->parent_ = y;
    
    return;
}

template<typename key_type>
void
tree_t<key_type>::rotate_to_right(node_t<key_type>* cur) {

    node_t<key_type>* y = cur->left_;
    cur->left_ = y->right_;
    if (y->right_ != nullptr) {
        y->right_->parent_ = cur;
    }
    y->parent_ = cur->parent_;
    if (cur->parent_ == nullptr) {
        root_ = y;
    }
    else if (cur == cur->parent_->right_) {
        cur->parent_->right_ = y;
    }
    else {
        cur->parent_->left_ = y;
    }
    y->right_ = cur;
    cur->parent_ = y;

    return;
}

//-----------------------------------------------------------------------------------------

template< typename key_type>
void tree_t<key_type>::erase(const key_type& key) {
    node_t<key_type>* parent = root_->parent_;
    node_t<key_type>* cur = root_;
    while (cur->get_key() != key) {
        parent = cur;
        if (key > cur->get_key()) {
            cur = cur->right_;
        }
        else if (key < cur->get_key()) {
            cur = cur->left_;
        }
    }
    erase_impl(cur);
}

template<typename key_type>
void
tree_t<key_type>::erase_impl(node_t<key_type>* z) {
    if (z == nullptr)
        return;

    node_t<key_type>* y = z;
    node_t<key_type>* x = nullptr;
    auto y_original_color = y->color_;

    if (z->left_ == nullptr) {
        x = z->right_;
        transplant(z, z->right_);
    } else if (z->right_ == nullptr) {
        x = z->left_;
        transplant(z, z->left_);
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
            transplant(y, y->right_);
            if (y->right_ != nullptr)
                y->right_->parent_ = y; // Check if y->right_ is not nullptr before assigning parent
            y->right_ = z->right_;
            if (y->right_ != nullptr)
                y->right_->parent_ = y; // Check if y->right_ is not nullptr before assigning parent
        }
        transplant(z, y);
        y->left_ = z->left_;
        if (y->left_ != nullptr)
            y->left_->parent_ = y; // Check if y->left is not nullptr before assigning parent
        y->color_ = z->color_;
    }

    if (y_original_color == node_t<key_type>::node_col::BLACK_ && x != nullptr) 
        fix_erase(x);

    delete z;
    return;
}


template<typename key_type>
void
tree_t<key_type>::fix_erase(node_t<key_type>* x) {
    while (x != root_ && x != nullptr && x->color_ == node_t<key_type>::node_col::BLACK_) {
        if (x == x->parent_->left_) {
            auto w = x->parent_->right_;
            if (w->color_ == node_t<key_type>::node_col::RED_) {
                w->color_ = node_t<key_type>::node_col::BLACK_;
                x->parent_->color_ = node_t<key_type>::node_col::RED_;
                rotate_to_left(x->parent_);
                w = x->parent_->right_;
            }
            if ((w->left_ == nullptr || w->left_->color_ == node_t<key_type>::node_col::BLACK_) &&
                (w->right_== nullptr || w->right_->color_ == node_t<key_type>::node_col::BLACK_)) {
                w->color_ = node_t<key_type>::node_col::RED_;
                x = x->parent_;
            } else {
                if (w->right_== nullptr || w->right_->color_ == node_t<key_type>::node_col::BLACK_) {
                    if (w->left_ != nullptr)
                        w->left_->color_= node_t<key_type>::node_col::BLACK_;
                    w->color_= node_t<key_type>::node_col::RED_;
                    rotate_to_right(w);
                    w = x->parent_->right_;
                }
                w->color_= x->parent_->color_;
                x->parent_->color_= node_t<key_type>::node_col::BLACK_;
                if (w->right_!= nullptr)
                    w->right_->color_= node_t<key_type>::node_col::BLACK_;
                rotate_to_left(x->parent_);
                x = root_;
            }
        } else {
            auto w = x->parent_->left_;
            if (w->color_== node_t<key_type>::node_col::RED_) {
                w->color_= node_t<key_type>::node_col::BLACK_;
                x->parent_->color_= node_t<key_type>::node_col::RED_;
                rotate_to_right(x->parent_);
                w = x->parent_->left_;
            }
            if ((w->right_== nullptr || w->right_->color_ == node_t<key_type>::node_col::BLACK_) &&
                (w->left_ == nullptr || w->left_->color_ == node_t<key_type>::node_col::BLACK_)) {
                w->color_ = node_t<key_type>::node_col::RED_;
                x = x->parent_;
            } else {
                if (w->left_ == nullptr || w->left_->color_ == node_t<key_type>::node_col::BLACK_) {
                    if (w->right_!= nullptr)
                        w->right_->color_ = node_t<key_type>::node_col::BLACK_;
                    w->color_ = node_t<key_type>::node_col::RED_;
                    rotate_to_left(w);
                    w = x->parent_->left_;
                }
                w->color_= x->parent_->color_;
                x->parent_->color_ = node_t<key_type>::node_col::BLACK_;
                if (w->left_ != nullptr)
                    w->left_->color_ = node_t<key_type>::node_col::BLACK_;
                rotate_to_right(x->parent_);
                x = root_;
            }
        }
    }
    if (x != nullptr)
        x->color_=node_t<key_type>::node_col::BLACK_;
    return;
}

//-----------------------------------------------------------------------------------------

template<typename key_type>
node_t<key_type>* tree_t<key_type>::upper_bound(key_type key) const {
    node_t<key_type>* node = root_->upper_bound(key);
    assert(node != nullptr);
    return node;
}

template<typename key_type>
node_t<key_type>* tree_t<key_type>::lower_bound(key_type key) const {
    node_t<key_type>* node = root_->lower_bound(key);
    assert(node != nullptr);
    return node;
}

template<typename key_type>
size_t tree_t<key_type>::range_query(int l_bound, int u_bound) const {

    if (l_bound >= u_bound || root_ == nullptr) {
        return 0;
    }
    node_t<key_type>* l_node = upper_bound(u_bound);
    node_t<key_type>* u_node = lower_bound(l_bound);
    assert(l_node != nullptr && u_node != nullptr);

    if (u_node->key_ > u_bound || l_node->key_ < l_bound) { //corner_case
        return 0;
    }
    return distance(l_node, u_node);
}

template<typename key_type>
size_t tree_t<key_type>::distance(node_t<key_type>* l_node,
                                     node_t<key_type>* u_node) const {
    assert(l_node != nullptr && u_node != nullptr);
    size_t u_bound_rank = l_node->define_node_rank(root_);
    size_t l_bound_rank = u_node->define_node_rank(root_);
    return u_bound_rank - l_bound_rank + 1;
}

//-----------------------------------------------------------------------------------------

template<typename key_type>
std::vector<key_type> tree_t<key_type>::store_inorder_walk() const {
    if (root_ == nullptr) {
        return std::vector<key_type> {};
    }
    return root_->store_inorder_walk();
}

template<typename key_type>
void tree_t<key_type>::graphviz_dump() const {
    graphviz::dump_graph_t tree_dump("../graph_lib/tree_dump.dot"); 

    root_->graphviz_dump(tree_dump);
    tree_dump.run_graphviz("../graph_lib/tree_dump.dot", "../graph_lib/dump");
    tree_dump.close_input();
    // tree_dump.close_input();
}
}