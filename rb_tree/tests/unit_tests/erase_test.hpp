#pragma once

using namespace avl;

//-----------------------------------------------------------------------------------------

class erase : public ::testing::Test {
    protected:
    avl::tree_t<int> tree;
    std::vector<int> data = {-14, 0, 3, 5, 11, 20, 21, 28, 42, 60};
    std::vector<int> correct_tree_min = { 0, 3, 5, 11, 20, 21, 28, 42, 60};
    std::vector<int> correct_tree_max = {-14, 0, 3, 5, 11, 20, 21, 28, 42};
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

}

TEST_F(erase, max) {
    tree.erase(60);
    std::vector<int> storage = tree.store_inorder_walk();
    ASSERT_TRUE(storage == correct_tree_max);
}

