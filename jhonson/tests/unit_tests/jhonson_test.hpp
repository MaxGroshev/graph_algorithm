#pragma once

using namespace graph;

//-----------------------------------------------------------------------------------------

class jhonson : public ::testing::Test {
    protected:
        graph_ui::graph_helper<int> gr_helper;
        std::vector<std::vector<int>> wiki_res 
                            = {{0, 1, -3, 2, -4},
                               {3, 0, -4, 1, -1},
                               {7, 4, 0, 5, 3},
                               {2, -1, -5, 0, -2},
                               {8, 5, 1, 6, 0}};
        void SetUp() {}
};

//-----------------------------------------------------------------------------------------

TEST_F(jhonson, wiki) {
    std::stringstream ss;
    ss << R"(0 |
        1 i 0 3|
        2 i 0 8 o 1 4 |
        3 i 1 1 o 0 2 o 2 -5 |
        4 i 0 -4 i 1 7 o 3 6$)";
    gr_helper.graph_init_from(ss);
    gr_helper.gr.graphviz_dump();
    auto res = std_like::jhonson(gr_helper.gr);
    ASSERT_TRUE(res.has_value() && res == wiki_res);
}

