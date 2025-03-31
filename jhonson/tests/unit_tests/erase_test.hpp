#pragma once

using namespace rb;
using n_col = node_t<int>::node_col;

//-----------------------------------------------------------------------------------------

class erase : public ::testing::Test {
    protected:
    rb::tree_t<int> tree;
    std::vector<int> data = {-14, 0, 3, 5, 11, 20, 21, 28, 42, 60};
    
    std::vector<int> correct_tree_min = { 0, 3, 5, 11, 20, 21, 28, 42, 60};
    std::vector<int> cl_correct_tree_min = {n_col::BLACK_, n_col::RED_, n_col::BLACK_, n_col::BLACK_, n_col::BLACK_, 
                                            n_col::BLACK_, n_col::BLACK_, n_col::BLACK_, n_col::RED_};
    
    std::vector<int> correct_tree_max = {-14, 0, 3, 5, 11, 20, 21, 28, 42};
    std::vector<int> cl_correct_tree_max = {n_col::BLACK_, n_col::BLACK_, n_col::BLACK_, n_col::BLACK_, n_col::BLACK_, 
                                            n_col::BLACK_, n_col::BLACK_, n_col::RED_, n_col::BLACK_};
    
    std::vector<int> correct_tree_rm3 = {-14, 0, 5, 11, 20, 21, 28, 42, 60};
    std::vector<int> cl_correct_tree_rm3 = {n_col::RED_, n_col::BLACK_, n_col::BLACK_, n_col::BLACK_, n_col::BLACK_, 
                                            n_col::BLACK_, n_col::BLACK_, n_col::BLACK_, n_col::RED_};
    
    std::vector<int> correct_tree_rm20 = {-14, 0, 3, 5, 11, 21, 28, 42, 60};
    std::vector<int> cl_correct_tree_rm20 = {n_col::BLACK_, n_col::BLACK_, n_col::BLACK_, n_col::BLACK_, n_col::BLACK_, 
                                             n_col::BLACK_, n_col::BLACK_, n_col::RED_, n_col::BLACK_};
    void SetUp() {
        for (const auto& key : data) {
            tree.insert(key);
        }
    }
};

//----------------c-------------------------------------------------------------------------

TEST_F(erase, min) {
    tree.graphviz_dump();
    tree.erase(-14);
    std::vector<int> storage = tree.store_inorder_walk();
    tree.graphviz_dump();
    ASSERT_TRUE(storage == correct_tree_min);
    auto cl_storage = tree.store_cl_inorder_walk();
    for(int i = 0; i < cl_storage.size(); i++) {
        ASSERT_TRUE(static_cast<std::vector<int>>(cl_storage[i]) == static_cast<std::vector<int>>(cl_correct_tree_min[i]));
    }
}

TEST_F(erase, max) {
    tree.erase(60);
    std::vector<int> storage = tree.store_inorder_walk();
    tree.graphviz_dump();
    ASSERT_TRUE(storage == correct_tree_max);
    auto cl_storage = tree.store_cl_inorder_walk();
    for(int i = 0; i < cl_storage.size(); i++) {
        ASSERT_TRUE(static_cast<int>(cl_storage[i]) == static_cast<int>(cl_correct_tree_max[i]));
    }
}

TEST_F(erase, rm3) {
    tree.erase(3);
    std::vector<int> storage = tree.store_inorder_walk();
    tree.graphviz_dump();
    ASSERT_TRUE(storage == correct_tree_rm3);
    auto cl_storage = tree.store_cl_inorder_walk();
    for(int i = 0; i < cl_storage.size(); i++) {
        ASSERT_TRUE(static_cast<int>(cl_storage[i]) == static_cast<int>(cl_correct_tree_rm3[i]));
    }
}

TEST_F(erase, rm20) {
    tree.erase(20);
    std::vector<int> storage = tree.store_inorder_walk();
    tree.graphviz_dump();
    ASSERT_TRUE(storage == correct_tree_rm20);
    auto cl_storage = tree.store_cl_inorder_walk();
    for(int i = 0; i < cl_storage.size(); i++) {
        ASSERT_TRUE(static_cast<int>(cl_storage[i]) == static_cast<int>(cl_correct_tree_rm20[i]));
    }
}

