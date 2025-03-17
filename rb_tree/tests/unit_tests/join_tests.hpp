#pragma once

using namespace rb;

//-----------------------------------------------------------------------------------------

class join : public ::testing::Test {
    protected:
    rb::tree_t<int> tree1;
    rb::tree_t<int> tree2;
    std::vector<int> init1 = {2, 3, 5, 6, 7, 8, 10, 11, 12, 13, 14};
    std::vector<int> init2 = {22, 24, 25, 26, 29, 30};
    void SetUp() {
        for (const auto& key : init1) {
            tree1.insert(key);
        }
        for (const auto& key : init2) {
            tree2.insert(key);
        }
    }
};

//-----------------------------------------------------------------------------------------

TEST_F(join, simp1) {
    rb::tree_t<int> tree11;
    rb::tree_t<int> tree22;
    std::vector<int> init11 = {1, 2, 3};
    std::vector<int> init22 = {5};
    for (const auto& key : init11) {
        tree11.insert(key);
    }
    for (const auto& key : init22) {
        tree22.insert(key);
    }
    tree11.graphviz_dump();
    tree22.graphviz_dump();
    tree11.union_tree(tree11, tree22);
    tree11.graphviz_dump();
}

// TEST_F(join, simp2) {
//     rb::tree_t<int> tree11;
//     rb::tree_t<int> tree22;
//     std::vector<int> init11 = {10, 20, 30, 40, 50, 60};
//     std::vector<int> init22 = {70, 80, 90};
//     for (const auto& key : init11) {
//         tree11.insert(key);
//     }
//     for (const auto& key : init22) {
//         tree22.insert(key);
//     }
//     tree11.graphviz_dump();
//     tree22.graphviz_dump();
//     tree11.join(tree11, tree22, 65);
//     tree11.graphviz_dump();
// }

// TEST_F(join, simp3) {
//     rb::tree_t<int> tree11;
//     rb::tree_t<int> tree22;
//     std::vector<int> init11 = {10, 20, 30, 40};
//     std::vector<int> init22 = {70, 80, 90, 100, 110, 120, 130};
//     for (const auto& key : init11) {
//         tree11.insert(key);
//     }
//     for (const auto& key : init22) {
//         tree22.insert(key);
//     }
//     tree11.graphviz_dump();
//     tree22.graphviz_dump();
//     tree11.join(tree11, tree22, 65);
//     tree11.graphviz_dump();
// }


