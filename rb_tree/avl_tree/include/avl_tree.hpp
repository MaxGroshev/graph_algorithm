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
        tree_t(){
            tnil_ = new node_t(key_type{}); //tmp probably inher
            root_ = tnil_;
        };
        tree_t(const tree_t<key_type>& tree) {
            root_ = new node_t<key_type> (*(tree.root_));
            tnil_ = new node_t<key_type> (*(tree.tnil_));
            if (root_->left_ != nullptr)
                root_->left_->parent_ = root_;
            if (root_->right_ != nullptr)
                root_->right_->parent_ = root_;
        };
        tree_t(tree_t<key_type>&& tree) noexcept {
            root_ = tree.root_;
            tnil_ = tree.tnil_;
            tree.root_ = nullptr;
            tree.tnil_ = nullptr;
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
        std::vector<typename node_t<key_type>::node_col> store_cl_inorder_walk() const;
        void graphviz_dump() const;
        
    private:
        void fix_insert(avl::node_t<key_type>* cur);
        void erase_impl(avl::node_t<key_type>* cur);
        void fix_erase(avl::node_t<key_type>* cur);
        void rotate_to_left(node_t<key_type>* cur_node);
        void rotate_to_right(node_t<key_type>* cur_node);


        node_t<key_type>* tree_minimum(node_t<key_type>* cur) {
            while (cur->left_ != tnil_) {
                cur = cur->left_;
            }
            return cur;
        }

        node_t<key_type>* tree_maximum(node_t<key_type>* cur) {
            while (cur->right_ != tnil_) {
                cur = cur->right_;
            }
            return cur;
        }
        node_t<key_type>* tree_successor(node_t<key_type>* cur) {
            if (cur->right_ != tnil_) {
                return tree_minimum(cur->right_);
            }
            auto cur_parent = cur->parent_;
            while (cur_parent != tnil_ && cur == cur_parent->right_) {
                cur = cur_parent->right_;
                cur_parent = cur_parent->parent_;
            }
            return cur_parent;
        }
        void transplant(node_t<key_type>* u, node_t<key_type>* v) {
            if (u->parent_ == nullptr) {
                root_ = v;
            }
            else if (u == u->parent_->left_) {
                u->parent_->left_ = v;
            }
            else {
                u->parent_->right_ = v;
            }
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
            if (front->left_ != tnil_) {
                nodes.push(front->left_);
            }
            if (front->right_ != tnil_) {
                nodes.push(front->right_);
            }
        }
        delete front;
    }
    delete tnil_;
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

    auto new_node =  new node_t<key_type>(key);
    new_node->left_ = tnil_;
    new_node->right_= tnil_;
    new_node->color_ = node_t<key_type>::node_col::RED_;

    node_t<key_type>* parent = nullptr;
    node_t<key_type>* cur = root_;
    while (cur != tnil_) {
        parent = cur;
        if (new_node->key_ < cur->key_) {
            cur = cur->left_;
        } 
        else {
            cur = cur->right_;
        }
    }
  
    new_node->parent_ = parent;
    if (parent == nullptr) {
        root_ = new_node;
    } 
    else if (new_node->key_ < parent->key_) {
        parent->left_ = new_node;
    } 
    else {
        parent->right_ = new_node;
    }

    if (new_node->parent_ == nullptr) {
        new_node->color_ = node_t<key_type>::node_col::BLACK_;
        return;
    }
    if (new_node->parent_->parent_ == nullptr) {
        return;
    }

    fix_insert(new_node);
    return;
}

template<typename key_type>
void
tree_t<key_type>::fix_insert(avl::node_t<key_type>* cur) {
    avl::node_t<key_type>*  node_uncle;
    while (cur != root_ && cur->parent_->color_ == node_t<key_type>::node_col::RED_) {
        if (cur->parent_ == cur->parent_->parent_->right_) {
            node_uncle = cur->parent_->parent_->left_;
            if (node_uncle->color_ == node_t<key_type>::node_col::RED_) {
                node_uncle->color_ = node_t<key_type>::node_col::BLACK_;
                cur->parent_->color_ = node_t<key_type>::node_col::BLACK_;
                cur->parent_->parent_->color_ = node_t<key_type>::node_col::RED_;
                cur = cur->parent_->parent_;
            } 
            else {
                if (cur == cur->parent_->left_) {
                    cur = cur->parent_;
                    rotate_to_right(cur);
                }
                cur->parent_->color_ = node_t<key_type>::node_col::BLACK_;
                cur->parent_->parent_->color_ = node_t<key_type>::node_col::RED_;
                rotate_to_left(cur->parent_->parent_);
            }
        }
        else {
            node_uncle = cur->parent_->parent_->right_;

            if (node_uncle->color_ == node_t<key_type>::node_col::RED_) {
                node_uncle->color_ = node_t<key_type>::node_col::BLACK_;
                cur->parent_->color_ = node_t<key_type>::node_col::BLACK_;
                cur->parent_->parent_->color_ = node_t<key_type>::node_col::RED_;
                cur = cur->parent_->parent_;
            } 
            else {
                if (cur == cur->parent_->right_) {
                    cur = cur->parent_;
                    rotate_to_left(cur);
                }
                cur->parent_->color_ = node_t<key_type>::node_col::BLACK_;
                cur->parent_->parent_->color_ = node_t<key_type>::node_col::RED_;
                rotate_to_right(cur->parent_->parent_);
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
    if (y->left_ != tnil_) {
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
    if (y->right_ != tnil_) {
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
tree_t<key_type>::erase_impl(node_t<key_type>* cur) {
    if (cur == tnil_) {
        return;
    }

    node_t<key_type>* y = cur;
    node_t<key_type>* x = nullptr;
    auto y_original_color = y->color_;

    if (cur->left_ == tnil_) {
        x = cur->right_;
        transplant(cur, cur->right_);
    } 
    else if (cur->right_ == tnil_) {
        x = cur->left_;
        transplant(cur, cur->left_);
    } 
    else {
        y = tree_minimum(cur->right_);
        y_original_color = y->color_;
        x = y->right_;

        if (y->parent_ == cur) {
            x->parent_ = y; 
        } 
        else {
            transplant(y, y->right_);
            y->right_ = cur->right_; 
            y->right_->parent_ = y; 
        }
        transplant(cur, y);
        y->left_ = cur->left_;
        y->left_->parent_ = y;
        y->color_ = cur->color_;
    }

    delete cur;
    if (y_original_color == node_t<key_type>::node_col::BLACK_) {
        fix_erase(x);
    }

    return;
}


template<typename key_type>
void
tree_t<key_type>::fix_erase(node_t<key_type>* x) {
    // node_t<key_type>* tmp = nullptr;
    while (x != root_ && x->color_ == node_t<key_type>::node_col::BLACK_) {
        if (x == x->parent_->left_) {
            auto w = x->parent_->right_;
            if (w->color_ == node_t<key_type>::node_col::RED_) {
                w->color_ = node_t<key_type>::node_col::BLACK_;
                x->parent_->color_ = node_t<key_type>::node_col::RED_;
                rotate_to_left(x->parent_);
                w = x->parent_->right_;
            }
            if ( w->left_->color_ == node_t<key_type>::node_col::BLACK_ &&
                w->right_->color_ == node_t<key_type>::node_col::BLACK_) {
                w->color_ = node_t<key_type>::node_col::RED_;
                x = x->parent_;
            } else {
                if (w->right_->color_ == node_t<key_type>::node_col::BLACK_) {
                    w->left_->color_= node_t<key_type>::node_col::BLACK_;
                    w->color_= node_t<key_type>::node_col::RED_;
                    rotate_to_right(w);
                    w = x->parent_->right_;
                }
                w->color_= x->parent_->color_;
                x->parent_->color_= node_t<key_type>::node_col::BLACK_;
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
            if (w->right_->color_ == node_t<key_type>::node_col::BLACK_ &&
                w->left_->color_ == node_t<key_type>::node_col::BLACK_) {
                w->color_ = node_t<key_type>::node_col::RED_;
                x = x->parent_;
            } else {
                if (w->left_->color_ == node_t<key_type>::node_col::BLACK_) {
                    w->right_->color_ = node_t<key_type>::node_col::BLACK_;
                    w->color_ = node_t<key_type>::node_col::RED_;
                    rotate_to_left(w);
                    w = x->parent_->left_;
                }
                w->color_= x->parent_->color_;
                x->parent_->color_ = node_t<key_type>::node_col::BLACK_;
                w->left_->color_ = node_t<key_type>::node_col::BLACK_;
                rotate_to_right(x->parent_);
                x = root_;
            }
        }
    }
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
std::vector<typename node_t<key_type>::node_col> tree_t<key_type>::store_cl_inorder_walk() const {
    if (root_ == nullptr) {
        return std::vector<typename node_t<key_type>::node_col> {};
    }
    std::vector<typename node_t<key_type>::node_col> storage;
    std::stack<const node_t<key_type>*> node_stk;
    const node_t<key_type>* cur_node = root_;

    while (cur_node != tnil_ || !node_stk.empty()) {
        if (!node_stk.empty()) {
            cur_node = node_stk.top();
            storage.push_back(cur_node->color_);
            if (cur_node->right_ != tnil_)
                cur_node = cur_node->right_;
            else
                cur_node = tnil_;

            node_stk.pop();
        }
        while (cur_node != tnil_) {
            node_stk.push(cur_node);
            cur_node = cur_node->left_;
        }
    }
    return storage;
}

template<typename key_type>
std::vector<key_type> tree_t<key_type>::store_inorder_walk() const {
    if (root_ == nullptr) {
        return std::vector<key_type> {};
    }
    std::vector<key_type> storage;
    std::stack<const node_t<key_type>*> node_stk;
    const node_t<key_type>* cur_node = root_;

    while (cur_node != tnil_ || !node_stk.empty()) {
        if (!node_stk.empty()) {
            cur_node = node_stk.top();
            storage.push_back(cur_node->key_);
            if (cur_node->right_ != tnil_)
                cur_node = cur_node->right_;
            else
                cur_node = tnil_;

            node_stk.pop();
        }
        while (cur_node != tnil_) {
            node_stk.push(cur_node);
            cur_node = cur_node->left_;
        }
    }
    return storage;
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