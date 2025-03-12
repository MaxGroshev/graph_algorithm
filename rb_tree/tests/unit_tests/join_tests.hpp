#pragma once

using namespace avl;

//-----------------------------------------------------------------------------------------

class join : public ::testing::Test {
    protected:
    avl::tree_t<int> tree1;
    avl::tree_t<int> tree2;
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

// TEST_F(join, simp1) {
//     avl::tree_t<int> tree11;
//     avl::tree_t<int> tree22;
//     std::vector<int> init11 = {1, 2, 3};
//     std::vector<int> init22 = {5, 6, 7};
//     for (const auto& key : init11) {
//         tree11.insert(key);
//     }
//     for (const auto& key : init22) {
//         tree22.insert(key);
//     }
//     tree11.graphviz_dump();
//     tree22.graphviz_dump();
//     tree11.join(tree11, tree22, 4);
//     tree11.graphviz_dump();
// }

TEST_F(join, simp2) {
    avl::tree_t<int> tree11;
    avl::tree_t<int> tree22;
    std::vector<int> init11 = {10, 20, 30, 40, 50, 60};
    std::vector<int> init22 = {70, 80, 90};
    for (const auto& key : init11) {
        tree11.insert(key);
    }
    for (const auto& key : init22) {
        tree22.insert(key);
    }
    tree11.graphviz_dump();
    tree22.graphviz_dump();
    tree11.join(tree11, tree22, 65);
    tree11.graphviz_dump();
}

// TEST_F(join, wiki_example) {
//     tree1.graphviz_dump();
//     tree2.graphviz_dump();
//     tree1.join(tree1, tree2, 15);
//     tree1.graphviz_dump();
// }


