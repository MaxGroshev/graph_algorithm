#pragma once

using namespace graph;

//-----------------------------------------------------------------------------------------

class jhonson : public ::testing::Test {
    protected:
        graph_ui::graph_helper<int> gr_helper;
        std::vector<std::vector<int>> wiki 
                            = {{1}};
        void SetUp() {}
};

//----------------c-------------------------------------------------------------------------

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
    // ASSERT_TRUE(res.has_value() && res == wiki_no_cycle_res);
}

