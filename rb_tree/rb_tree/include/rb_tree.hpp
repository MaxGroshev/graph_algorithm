#pragma once
#include "rb_node.hpp"

//-----------------------------------------------------------------------------------------

namespace rb {

template<typename key_type = int>
class tree_t final {
    private:
    node_t<key_type>* root_ = nullptr;
    public:
        tree_t(){};
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
        std::vector<typename node_t<key_type>::node_col> store_cl_inorder_walk() const;
        void graphviz_dump() const;
        
        private:
            void fix_insert(rb::node_t<key_type>* cur);
            void erase_impl(rb::node_t<key_type>* cur);
            void fix_erase(rb::node_t<key_type>* cur);
            node_t<key_type>* rotate_to_left(node_t<key_type>* cur_node);
            node_t<key_type>* rotate_to_right(node_t<key_type>* cur_node);
            
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
                node_t<key_type>* tmp = cur;

                while (tmp->left_ != nullptr) {
                    tmp = tmp->left_;
                }
            
                return tmp;
            }

            node_t<key_type>* get_replace(node_t<key_type>* x) {
                if (x->left_ != nullptr && x->right_ != nullptr) {
                    return tree_successor(x->right_);
                }
                if (x->left_ == nullptr && x->right_ == nullptr) {
                    return nullptr;
                }
                if (x->left_ != nullptr) {
                    return x->left_;
                }
                else {
                    return x->right_;
                }
            }
        
            node_t<key_type>* join_right(node_t<key_type>* t_l_root, node_t<key_type>* t_r_root, const key_type& key) {
                if ((t_l_root == nullptr || t_l_root->color_ == node_t<key_type>::node_col::BLACK_) &&
                    (root_->get_black_height(t_l_root) == root_->get_black_height(t_r_root))) {
                        auto tmp_root = new node_t<key_type>(key, node_t<key_type>::node_col::RED_);
                        tmp_root->left_ = t_l_root;
                        tmp_root->right_ = t_r_root;
                        tmp_root->left_->parent_ = tmp_root;
                        tmp_root->right_->parent_ = tmp_root;
                        return tmp_root;
                }
                auto node_right = join_right(t_l_root->right_, t_r_root, key);
                auto node = new node_t<key_type>(t_l_root->key_, t_l_root->color_);
                node->left_ = t_l_root->left_;
                node->right_ = node_right;
                node->left_->parent_  = node;
                node->right_->parent_ = node;

                if ((t_l_root == nullptr || t_l_root->color_ == node_t<key_type>::node_col::BLACK_) &&
                    node->right_->color_ == node_t<key_type>::node_col::RED_ &&
                    node->right_->right_->color_ == node_t<key_type>::node_col::RED_) {
                        node->right_->right_->color_ = node_t<key_type>::node_col::BLACK_;
                        return rotate_to_left(node);
                }
                return node;
            }

            node_t<key_type>* join_left(node_t<key_type>* t_l_root, node_t<key_type>* t_r_root, const key_type& key) {
                if ((t_r_root == nullptr || t_r_root->color_ == node_t<key_type>::node_col::BLACK_) &&
                root_->get_black_height(t_l_root) == root_->get_black_height(t_r_root)) {
                        auto tmp_root = new node_t<key_type>(key, node_t<key_type>::node_col::RED_);
                        tmp_root->left_ = t_l_root;
                        tmp_root->right_ = t_r_root;
                        tmp_root->left_->parent_ = tmp_root;
                        tmp_root->right_->parent_ = tmp_root;
                        return tmp_root;
                    }
                    auto node_left = join_left(t_l_root, t_r_root->left_, key);
                    auto node = new node_t<key_type>(t_r_root->key_, t_r_root->color_);
                    node->left_ = node_left;
                    node->right_ = t_r_root->right_;
                    node->left_->parent_  = node;
                    node->right_->parent_ = node;

                if ((t_r_root == nullptr || t_r_root->color_ == node_t<key_type>::node_col::BLACK_) &&
                    node->left_->color_ == node_t<key_type>::node_col::RED_ &&
                    node->left_->left_->color_ == node_t<key_type>::node_col::RED_) {
                        node->left_->left_->color_ = node_t<key_type>::node_col::BLACK_;
                        return rotate_to_right(node);
                }
                return node;
            }

            node_t<key_type>* join(node_t<key_type>* t_l_root, node_t<key_type>* t_r_root, const key_type& key) {
                    if (root_->get_black_height(t_l_root) > root_->get_black_height(t_r_root)) {
                        auto node = join_right(t_l_root, t_r_root, key);
                        if (node->color_ ==  node_t<key_type>::node_col::RED_ && 
                            node->right_->color_ ==  node_t<key_type>::node_col::BLACK_) {
                                node->color_ =  node_t<key_type>::node_col::BLACK_; 
                        }
                        return node;
                    }
                    else if (root_->get_black_height(t_l_root) < root_->get_black_height(t_r_root)) {
                        auto node = join_left(t_l_root, t_r_root, key);
                        if (node->color_ ==  node_t<key_type>::node_col::RED_ && 
                            node->left_->color_ ==  node_t<key_type>::node_col::BLACK_) {
                                node->color_ =  node_t<key_type>::node_col::BLACK_; 
                        }
                        return node;
                    }
                    else if ((t_l_root == nullptr || t_l_root->color_ == node_t<key_type>::node_col::BLACK_) &&
                             (t_l_root != nullptr && (t_l_root->right_ == nullptr || t_l_root->right_->color_ == node_t<key_type>::node_col::BLACK_))) {
                        auto tmp_root = new node_t<key_type>(key, node_t<key_type>::node_col::RED_);
                        tmp_root->left_ = t_l_root;
                        tmp_root->right_ = t_r_root;
                        if (tmp_root->left_ != nullptr) {
                            tmp_root->left_->parent_ = tmp_root;
                        }
                        if (tmp_root->right_ != nullptr) {
                            tmp_root->right_->parent_ = tmp_root;
                        }
                        return tmp_root;
                    }
                    auto tmp_root = new node_t<key_type>(key, node_t<key_type>::node_col::BLACK_);
                    tmp_root->left_ = t_l_root;
                    tmp_root->right_ = t_r_root;
                    if (tmp_root->left_ != nullptr) {
                        tmp_root->left_->parent_ = tmp_root;
                    }
                    if (tmp_root->right_ != nullptr) {
                        tmp_root->right_->parent_ = tmp_root;
                    }
                    return tmp_root;
                }
        
                std::tuple<node_t<key_type>*, bool, node_t<key_type>*> split(node_t<key_type>* cur, key_type key) {
                    if (cur == nullptr) {
                        return {nullptr, false, nullptr};
                    }
                    if (key == cur->get_key()) {
                        return {cur->left_, true, cur->right_};
                    }
                    if (key < cur->get_key()) {
                        auto [left, res, right] = split(cur->left_, key);
                        return {left, res, join(right, cur->right_, cur->get_key())};
                    }
                    auto [left, res, right] = split(cur->right_, key);
                    return {join(cur->left_, left, cur->get_key()), res, cur->right_};
                }
        
                node_t<key_type>*union_impl(node_t<key_type>* t_l_root, node_t<key_type>* t_r_root) {
                    if (t_l_root == nullptr) {
                        return t_r_root;
                    }
                    else if (t_r_root == nullptr) {
                        return t_l_root;
                    }
        
                    auto [left, res, right] = split(t_l_root, t_r_root->get_key());
                    auto left_p = union_impl(left, t_r_root->left_);
                    auto right_p = union_impl(right, t_r_root->right_);
                    return join(left_p, right_p, t_r_root->get_key());
                }
    public:

        void union_tree(tree_t& t_l, tree_t& t_r) {
            auto new_root = union_impl(t_l.root_, t_r.root_) ;
            t_l.root_ = new_root;
            t_r.root_ = nullptr;
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
tree_t<key_type>::fix_insert(rb::node_t<key_type>* cur) {
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
node_t<key_type>*
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
    
    return y;
}

template<typename key_type>
node_t<key_type>*
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

    return y;
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
    node_t<key_type>* repl = get_replace(cur);
    node_t<key_type>* parent = cur->parent_;

    if (repl == nullptr) {
        if (cur == root_) {
            root_ = nullptr;
        } 
        else {
            if ((repl == nullptr ||  repl->color_ == node_t<key_type>::node_col::BLACK_) 
                && (cur->color_ == node_t<key_type>::node_col::BLACK_)) {
                fix_erase(cur);
            } 
            else {
                if (cur->get_brother() != nullptr) {
                    cur->get_brother()->color_ = node_t<key_type>::node_col::RED_;
                }
            } 
            if (cur->on_left_br()) {
                parent->left_ = nullptr;
            }
            else {
                parent->right_ = nullptr;
            }
        }
        delete cur;
        return;
    }
 
    if (cur->left_ == nullptr ||  cur->right_ == nullptr) {
        if (cur == root_) {
            cur->key_ = repl->key_;
            cur->left_ = cur->right_ = nullptr;
            delete repl;
        } 
        else {
            if (cur->on_left_br()) {
                parent->left_ = repl;
            } 
            else {
                parent->right_ = repl;
            }
            delete cur;
            repl->parent_ = parent;
            if ((repl == nullptr ||  repl->color_ == node_t<key_type>::node_col::BLACK_) 
                && (cur->color_ == node_t<key_type>::node_col::BLACK_)) {
                fix_erase(repl);
            } 
            else {
                repl->color_ = node_t<key_type>::node_col::BLACK_;
            }
        }
        return;
    }
 
    std::swap(repl->key_, cur->key_);
    erase_impl(repl);
}


template<typename key_type>
void
tree_t<key_type>::fix_erase(node_t<key_type>* x) {
    if (x == root_)
	    return;

	node_t<key_type>* brother = x->get_brother();
    node_t<key_type>* parent = x->parent_;
	if (brother == nullptr) {
	    fix_erase(parent);
	} 
    else {
        if (brother->color_ == node_t<key_type>::node_col::RED_) {
            parent->color_ = node_t<key_type>::node_col::RED_;
            brother->color_ = node_t<key_type>::node_col::BLACK_;
            if (brother->on_left_br()) {
                rotate_to_right(parent);
            } 
            else {
                rotate_to_left(parent);
            }
            fix_erase(x);
        } 
        else {
            if (brother->left_ != nullptr && brother->left_->color_ == node_t<key_type>::node_col::RED_) {
                if (brother->on_left_br()) {
                    brother->left_->color_ = brother->color_;
                    brother->color_ = parent->color_;
                    rotate_to_right(parent);
                } 
                else {
                    brother->left_->color_ = parent->color_;
                    rotate_to_right(brother);
                    rotate_to_left(parent);
                }
                parent->color_ = node_t<key_type>::node_col::BLACK_;
            } 
            else if (brother->right_ != nullptr && brother->right_->color_ == node_t<key_type>::node_col::RED_) {
                if (brother->on_left_br()) {
                    brother->right_->color_ = parent->color_;
                    rotate_to_left(brother);
                    rotate_to_right(parent);
                } else {
                    brother->right_->color_ = brother->color_;
                    brother->color_ = parent->color_;
                    rotate_to_left(parent);
                }
                parent->color_ = node_t<key_type>::node_col::BLACK_;
            }
            else {
                brother->color_ = node_t<key_type>::node_col::RED_;
                if (parent->color_ == node_t<key_type>::node_col::BLACK_) {
                    fix_erase(parent);
                }
                else {
                    parent->color_ = node_t<key_type>::node_col::BLACK_;
                }
            }
        }
	}
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

    while (cur_node != nullptr || !node_stk.empty()) {
        if (!node_stk.empty()) {
            cur_node = node_stk.top();
            storage.push_back(cur_node->color_);
            if (cur_node->right_ != nullptr)
                cur_node = cur_node->right_;
            else
                cur_node = nullptr;

            node_stk.pop();
        }
        while (cur_node != nullptr) {
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

    while (cur_node != nullptr || !node_stk.empty()) {
        if (!node_stk.empty()) {
            cur_node = node_stk.top();
            storage.push_back(cur_node->key_);
            if (cur_node->right_ != nullptr)
                cur_node = cur_node->right_;
            else
                cur_node = nullptr;

            node_stk.pop();
        }
        while (cur_node != nullptr) {
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