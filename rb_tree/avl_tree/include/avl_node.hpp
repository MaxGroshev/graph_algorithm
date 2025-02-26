#pragma once

#include "utils.hpp"
#include <memory>
#include <stack>
#include <cassert>
#include <stdexcept>
#include <utility>

//-----------------------------------------------------------------------------------------

namespace avl {

template <typename key_type> class node_t;

template<typename key_type = int>
class wrap_node_t final {

    node_t<key_type>* dat_node_;

    public:
        wrap_node_t(node_t<key_type>* node) : dat_node_(node) {};
        key_type const & get_key() const {
            return dat_node_->get_key();
        }
        size_t get_height() const {
            if (dat_node_)
                return dat_node_->get_height();
            return 0;
        }
        size_t get_size() const {
            if (dat_node_)
                return dat_node_->get_size();
            return 0;
        }
        size_t define_node_rank(node_t<key_type>* root) const {
            if (dat_node_)
                return dat_node_->define_node_rank(root, dat_node_);
            return 0;
        }
        bool is_valid() {
            return (dat_node_ != nullptr);
        }
};

template <typename key_type = int>
class node_t {
    public:
        enum node_col {
            BLACK_ = 0,
            RED_   = 1,
        };

    private: 
        using unique_ptr_node_t = typename std::unique_ptr<node_t<key_type>>;

        unique_ptr_node_t left_      = nullptr;
        unique_ptr_node_t right_     = nullptr;
        node_t<key_type>* parent_    = nullptr;
        size_t size_   = 1;
        size_t height_ = 1;
        node_col color_  = node_col::BLACK_;  
        key_type key_;

    public:
        node_t(const key_type& key) : key_(key){};
        node_t(key_type&& key) :  key_(std::forward<key_type>(key)) {};
        node_t(const node_t<key_type>& node) : key_(node.key_),
                                               height_(node.height_),
                                               color_(node.color_) {

            unique_ptr_node_t ret_node = safe_copy(node);
            left_  = std::move(ret_node->left_);
            right_ = std::move(ret_node->right_);
        }
        node_t(key_type key, size_t size, size_t height, node_col color) :
            key_(key),
            size_(size), height_(height), color_(color)
            {};

        unique_ptr_node_t safe_copy (const node_t<key_type>& node);
        node_t<key_type>& operator= (const node_t<key_type>& node);
        node_t(node_t<key_type>&& node) = default;
        node_t<key_type>& operator= (node_t<key_type>&& node){
            left_ = std::move(node->left_);
            right_ = std::move(node->right_);
            parent_ = parent_;
            key_ = node->key_;
            color_ = node->color_;
        };


        int find_balance_fact(const unique_ptr_node_t& node) const {
            if (node)
                return (get_height(node->right_) - get_height(node->left_));
            return 0;
        }
        unique_ptr_node_t& get_left()  {return left_;};
        unique_ptr_node_t& get_right() {return right_;};
        node_t<key_type>* get_parent() {return parent_;};
        void set_parent(node_t<key_type>* node) {parent_ = node;};
        void set_left(unique_ptr_node_t& node) {left_ = node;};
        void set_right(unique_ptr_node_t& node) {right_ = node;};

        size_t get_height(const unique_ptr_node_t& node) const {
            if (node) return node->height_; return 0;
        }
        size_t get_size(const unique_ptr_node_t& node) const {
            if (node) return node->size_; return 0;
        }
        node_col get_color() const {
            return color_;
        }
        void set_color(node_col color)  { //TODO: fix it
            color_ = color;
        }
        key_type const & get_key() const {
            return key_;
        }
        // unique_ptr_node_t& tree_minimum(unique_ptr_node_t& cur) {
        //     while (cur->left_ != nullptr) {
        //         cur = cur->left_;
        //     }
        //     return cur;
        // }

        // unique_ptr_node_t& tree_maximum(const unique_ptr_node_t& cur) const {
        //     while (cur->right_ != nullptr) {
        //         cur = cur->right_;
        //     }
        //     return cur;
        // }
//BAD Design?
        // node_t<key_type>* tree_successor(unique_ptr_node_t& cur) {
        //     if (cur->right_ != nullptr) {
        //         return tree_minimum(cur->right_).get();
        //     }
        //     auto cur_parent = cur->parent_;
        //     auto cur_raw    = cur.get();
        //     while (cur_parent != nullptr && cur_raw == cur_parent->right_) {
        //         cur_raw = cur_parent->right_.get();
        //         cur_parent = cur_parent->parent_;
        //     }
        //     return cur_parent;
        // }

        unique_ptr_node_t balance_subtree(unique_ptr_node_t& cur_node, unique_ptr_node_t& root);
        void rotate_to_left(node_t<key_type>* cur_node, unique_ptr_node_t& root);
        void rotate_to_right(unique_ptr_node_t&& cur_node, unique_ptr_node_t& root);
        unique_ptr_node_t erase(unique_ptr_node_t& root, const key_type& key);
        unique_ptr_node_t insert(unique_ptr_node_t& cur_node,  const key_type& key);
        
        std::vector<key_type> store_inorder_walk() const;
        void graphviz_dump(graphviz::dump_graph_t& tree_dump) const ;
        node_t<key_type>* upper_bound(avl::node_t<key_type>* node, const key_type& key) const;
        node_t<key_type>* lower_bound(avl::node_t<key_type>* node, const key_type& key) const;
        
        size_t define_node_rank(const node_t<key_type>* root, const node_t<key_type>* cur_node) const;
    private:
        unique_ptr_node_t erase_node_impl(unique_ptr_node_t& cur, unique_ptr_node_t& root);
        void run_rotate_to_left(node_t<key_type>* x, unique_ptr_node_t& root);
        void run_rotate_to_right(node_t<key_type>* cur_gparent, unique_ptr_node_t& root);
        void get_sibling();
};
}

//-----------------------------------------------------------------------------------------

namespace avl {

template<typename key_type>
node_t<key_type>& node_t<key_type>::operator= (const node_t<key_type>& node) {
    if (this == &node)
        return *this;

    std::unique_ptr<node_t<key_type>> tmp_left_  =
                                std::make_unique<node_t<key_type>> (*(node.left_));
    std::unique_ptr<node_t<key_type>> tmp_right_ =
                                std::make_unique<node_t<key_type>> (*(node.right_));
    assert(tmp_left_ != nullptr && tmp_right_ != nullptr);

    key_   = node.key_;
    height_= node.height_;
    color_ = node.color_;

    left_  = std::move(tmp_left_);
    right_ = std::move(tmp_right_);

    return *this;
}

template<typename key_type>
typename node_t<key_type>::unique_ptr_node_t
node_t<key_type>::safe_copy(const node_t<key_type>& origine_node) {

    auto origine_node_ptr = &origine_node;
    std::unique_ptr<node_t<key_type>> new_node =
                        std::make_unique<node_t<key_type>>(origine_node_ptr->key_,
                        origine_node_ptr->size_, origine_node_ptr->height_, origine_node_ptr->color_);

    node_t<key_type>* iter_node = new_node.get();
    while (origine_node_ptr != nullptr) {
        if (iter_node->left_ == nullptr && origine_node_ptr->left_ != nullptr) {
            iter_node->left_ = std::make_unique<node_t<key_type>>(
                            origine_node_ptr->left_->key_,
                            origine_node_ptr->left_->size_, origine_node_ptr->left_->height_, 
                            origine_node_ptr->left_->color_);
            iter_node->left_->parent_ = iter_node;

            iter_node    = iter_node->left_.get();
            origine_node_ptr = origine_node_ptr->left_.get();
        }
        else if (iter_node->right_ == nullptr && origine_node_ptr->right_ != nullptr) {
            iter_node->right_ = std::make_unique<node_t<key_type>>(
                            origine_node_ptr->right_->key_,
                            origine_node_ptr->right_->size_, origine_node_ptr->right_->height_,
                            origine_node_ptr->left_->color_);
            iter_node->right_->parent_ = iter_node;

            iter_node    = iter_node->right_.get();
            origine_node_ptr = origine_node_ptr->right_.get();
        }
        else {
            iter_node = iter_node->parent_;
            origine_node_ptr = origine_node_ptr->parent_;
        }
    }
    return new_node;
}

//-----------------------------------------------------------------------------------------

template<typename key_type>
typename node_t<key_type>::unique_ptr_node_t
node_t<key_type>::insert(unique_ptr_node_t& root, const key_type& key) {
    if(!root)
        throw("Invalid ptr");

    node_t<key_type>* parent = root->parent_;
    node_t<key_type>* cur = root.get();
    while (cur != nullptr) {
        parent = cur;
        if (key > cur->get_key()) {
            cur = cur->right_.get();
            continue;
        }
        cur = cur->left_.get();
    }

    unique_ptr_node_t new_root = nullptr;
    if (key < parent->get_key()) {
        parent->left_ = std::make_unique<node_t<key_type>>(key);
        parent->left_->parent_ = parent;
        parent->left_->set_color(node_col::RED_);
        if (parent->left_->parent_->parent_ == nullptr) {
            return std::move(root);//TODO: shit
        }
        return balance_subtree(parent->left_, root);
    } 
    else {
        parent->right_ = std::make_unique<node_t<key_type>>(key);
        parent->right_->parent_ = parent;
        parent->right_->set_color(node_col::RED_);
        if (parent->right_->parent_->parent_ == nullptr) {
            return std::move(root);
        }
        return balance_subtree(parent->right_, root);
    }
}

template<typename key_type>
typename node_t<key_type>::unique_ptr_node_t
node_t<key_type>::erase(unique_ptr_node_t& root, const key_type& key) {
    if(!root)
        throw("Invalid ptr");
    //make search func
    node_t<key_type>* parent = root->parent_;
    node_t<key_type>* cur = root.get();
    while (cur->get_key() != key) {
        parent = cur;
        if (key > cur->get_key()) {
            cur = cur->right_.get();
        }
        else if (key < cur->get_key()) {
            cur = cur->left_.get();
        }
    }
    if (parent == nullptr) {
        return erase_node_impl(root, root);
    }
    else if (parent->left_.get() == cur) {
        return erase_node_impl(parent->left_, root);
    }
    else {
        return erase_node_impl(parent->right_, root);
    }
}

template<typename key_type>
typename node_t<key_type>::unique_ptr_node_t
node_t<key_type>::erase_node_impl(unique_ptr_node_t& z, unique_ptr_node_t& root) {
    // node_t<key_type>* x = nullptr;
    // node_t<key_type>* y = z.get();
    // if (z->left_ == nullptr || z->right_ == nullptr) {
    //     y = tree_successor(z);
    // }
    // if (y->left_ != nullptr) {
    //     x = y->left_.get();
    // }
    // else {
    //     x = y->right_.get();
    // }
    // x->parent_ = y->parent_;
    // if (y->parent_ == nullptr) {
    //     root.reset(x);
    // }
    // else if (y == y->parent_->left_.get()) {
    //     y->parent_->left_.reset(x);
    // }
    // else {
    //     y->parent_->left_.reset(x);
    // }
    // if (y != z.get()) {
    //     z->key_ = y->key_;
    // }
    // if (y->color_ == node_col::BLACK_) {
    //     // return fix_erase(x, root);
    // }
}

// template<typename key_type>
// typename node_t<key_type>::unique_ptr_node_t
// node_t<key_type>::fix_erase(node_t<key_type>* x, unique_ptr_node_t& root) {
//     node_t<key_type>* w = nullptr;
//     while (x != root.get() && x->color_ == node_col::BLACK_) {
//         if (x == x->parent_->left_) {
//             w = x->parent_->right_;
//             if (w->color_ == node_col::RED_) {
//                 rotate_to_left(, root)
//             }
//         }
//     }
// }

//----------------------------ROTATES------------------------------------------------------

template<typename key_type>
typename node_t<key_type>::unique_ptr_node_t
node_t<key_type>::balance_subtree(unique_ptr_node_t& cur, unique_ptr_node_t& root) {
    auto cur_parent = cur->parent_;
    while (cur_parent && cur_parent->color_ == node_col::RED_) {
        auto cur_gparent = cur_parent->parent_;
        if (cur_parent == cur_gparent->left_.get()) {
            auto uncle = cur_gparent->right_.get();
            if (uncle && uncle->color_ == node_col::RED_) {
                cur_parent->color_ = node_col::BLACK_;
                uncle->color_ = node_col::BLACK_;
                cur_gparent->color_ = node_col::RED_;
                cur_parent = cur_gparent->parent_;
            } else {
                if (cur == cur_parent->right_) {
                    rotate_to_left(cur_gparent->left_.get(), root);
                    cur_parent = cur_gparent->left_.get();
                }
                cur_parent->color_ = node_col::BLACK_;
                cur_gparent->color_ = node_col::RED_;
                run_rotate_to_right(cur_gparent, root);
            }
        } else {
            auto uncle = cur_gparent->left_.get();
            if (uncle && uncle->color_ == node_col::RED_) {
                cur_parent->color_ = node_col::BLACK_;
                uncle->color_ = node_col::BLACK_;
                cur_gparent->color_ = node_col::RED_;
                cur_parent = cur_gparent->parent_;
            } else {
                if (cur == cur_parent->left_) {
                    rotate_to_right(std::move(cur_gparent->right_), root);
                    cur_parent = cur_gparent->right_.get();
                }
                cur_parent->color_ = node_col::BLACK_;
                cur_gparent->color_ = node_col::RED_;
                rotate_to_left(cur_gparent, root);
            }
        }
    }
    root->color_ = node_col::BLACK_;
    return std::move(root);
}

template<typename key_type>
void
node_t<key_type>::run_rotate_to_left(node_t<key_type>* cur_gparent, unique_ptr_node_t& root) {
    auto cur_ggparent = cur_gparent->parent_;
    if (!cur_ggparent) {
        rotate_to_left(root.get(), root);
    } 
    else if (cur_gparent == cur_ggparent->left_.get()) {
        rotate_to_left(cur_ggparent->left_.get(), root);
    } 
    else {
        rotate_to_left(cur_ggparent->right_.get(), root);
    }
}

template<typename key_type>
void
node_t<key_type>::rotate_to_left(node_t<key_type>* x, unique_ptr_node_t& root) {
    auto y = std::move(x->right_);
    x->right_ = std::move(y->left_);
    // if (x->right_ != nullptr) {
    //     x->right_->parent_ = x;
    // }
    y->parent_ = x->parent_;
    if (x->parent_ == nullptr) {
        root = std::move(y);
        x->right_.reset(x);
        x->parent_ = root.get();
        return;
    }
    // else if (x == x->parent_->left_.get()) {
    //     x->parent_->left_ = y;
    // }
    // else {
    //     x->parent_->right_ = y;
    // }
}

template<typename key_type>
void
node_t<key_type>::run_rotate_to_right(node_t<key_type>* cur_gparent, unique_ptr_node_t& root) {
    auto cur_ggparent = cur_gparent->parent_;
    if (!cur_ggparent) {
        rotate_to_left(root.get(), root);
    } 
    else if (cur_gparent == cur_ggparent->left_.get()) {
        rotate_to_right(std::move(cur_ggparent->left_), root);
    } 
    else {
        rotate_to_right(std::move(cur_ggparent->right_), root);
    }
}

template<typename key_type>
void
node_t<key_type>::rotate_to_right(unique_ptr_node_t&& x,  unique_ptr_node_t& root) {

    if(!x)
        throw("Invalid ptr");

    auto y = std::move(x->left_);
    x->left_ = std::move(y->right_);
    if (x->left_) {
        x->left_->parent_ = x.get();
    }
    y->parent_ = x->parent_;
    auto xparent = x->parent_;
    if (!xparent) {
        auto px = x.release();
        root = std::move(y);
        root->right_ = std::unique_ptr<node_t<key_type>>(px);
        root->right_->parent_ = root.get();
    } else if (x == xparent->left_) {
        auto px = x.release();
        xparent->left_ = std::move(y);
        xparent->left_->right_ = std::unique_ptr<node_t<key_type>>(px);
        xparent->left_->right_->parent_ = xparent->left_.get();
    } else {
        auto px = x.release();
        xparent->right_ = std::move(y);
        xparent->right_->right_ = std::unique_ptr<node_t<key_type>>(px);
        xparent->right_->right_->parent_ = xparent->right_.get();
    }
}

//--------------------RANGES---------------------------------------------------------------

template<typename key_type>
node_t<key_type>*
node_t<key_type>::upper_bound(node_t<key_type>* cur_node, const key_type& key) const {

    node_t<key_type>* node = nullptr;
    if (cur_node->key_ < key) {
        if (cur_node->right_ != nullptr)
            node = upper_bound(cur_node->right_.get(), key);
        else
            return cur_node;
    }
    else if (cur_node->key_ > key) {
        if (cur_node->left_ != nullptr)
            node = upper_bound(cur_node->left_.get(), key);
        else
            return cur_node;
    }
    else if (cur_node->key_ == key) {
        return cur_node;
    }

    if (node->key_ > key && cur_node->key_ < key) {
        return cur_node;
    }
    return node;
}

template<typename key_type>
node_t<key_type>*
node_t<key_type>::lower_bound(node_t<key_type>* cur_node, const key_type& key) const {

    node_t<key_type>*  node = nullptr;
    if (cur_node->key_ < key) {
        if (cur_node->right_ != nullptr)
            node = lower_bound(cur_node->right_.get(), key);
        else
            return cur_node;
    }
    else if (cur_node->key_ > key) {
        if (cur_node->left_ != nullptr)
            node = lower_bound(cur_node->left_.get(), key);
        else
            return cur_node;
    }
    else if (cur_node->key_ == key) {
        return cur_node;
    }

    if (node->key_ < key && cur_node->key_ > key) {
        return cur_node;
    }
    return node;
}

//-----------------------------------------------------------------------------------------

template<typename key_type>
size_t node_t<key_type>::define_node_rank(const node_t<key_type>* root,
                                          const node_t<key_type>* cur_node) const {

    if(cur_node == nullptr)
        throw("Invalid ptr");

    size_t rank = 1;
    if (cur_node->left_ != nullptr) {
        rank += cur_node->left_->size_;
    }
    const node_t<key_type>* tmp_node = this;
    while (tmp_node != root) {
        if (tmp_node == tmp_node->parent_->right_.get()) {
            rank += get_size (tmp_node->parent_->left_) + 1;
        }
        tmp_node = tmp_node->parent_;
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
                cur_node = cur_node->right_.get();
            else
                cur_node = nullptr;

            node_stk.pop();
        }
        while (cur_node) {
            node_stk.push(cur_node);
            cur_node = cur_node->left_.get();
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
        tree_dump.graph_edge.print_edge(this, left_.get(), tree_dump.graphviz_strm);
        left_->graphviz_dump(tree_dump);
    }
    if (right_ != nullptr)
    {
        tree_dump.graph_edge.fillcolor = "#DC143C";
        tree_dump.graph_edge.color     = "#DC143C";
        tree_dump.graph_edge.print_edge(this, right_.get(), tree_dump.graphviz_strm);
        right_->graphviz_dump(tree_dump);
    }
}
}

