#pragma once
#include "avl_node.hpp"

//-----------------------------------------------------------------------------------------

namespace avl {

template<typename key_type = int>
class tree_t final {
    private:
        node_t<key_type>* root_ = nullptr;
    public:
        tree_t(){};
        tree_t(key_type key) {
            root_ = new node_t(key);
            assert(root_ != nullptr);
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
    }
    root_ = root_->insert(root_, key);
    root_->parent_ = nullptr;
}

template< typename key_type>
void tree_t<key_type>::erase(const key_type& key) {
    root_ = root_->erase(root_, key);

    root_->set_parent(nullptr);
    root_->parent_ = nullptr;
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