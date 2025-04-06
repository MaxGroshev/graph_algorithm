#pragma once

using namespace graph;

//-----------------------------------------------------------------------------------------

class bellman_ford : public ::testing::Test {
    protected:
        graph_ui::graph_helper<int> gr_helper;
        std::vector<std::pair<int, long long>> wiki_no_cycle_res 
                            = {{0, 0}, {0, 6}, {0, 7}, {2, 4}, {1, 2}};
        std::vector<std::pair<int, long long>> wiki_cycle_res 
                            = {{0, 0}, {3, 2}, {0, 7}, {2, 4}, {1, -2}};

        void SetUp() {}
};

//----------------c-------------------------------------------------------------------------

TEST_F(bellman_ford, wiki_no_cycle) {
    std::stringstream ss;
    ss << R"(0 |
           1 i 0 6 |
           2 i 0 7 i 1 8 |
           3 i 1 5 i 2 -3 |
           4 i 2 9 i 1 -4 o 3 7 $)";
    gr_helper.graph_init_from(ss);
    gr_helper.gr.graphviz_dump();
    auto res = std_like::bellman_ford(gr_helper.gr.begin(), gr_helper.gr.end());
    ASSERT_TRUE(res.has_value() && res == wiki_no_cycle_res);
}

TEST_F(bellman_ford, wiki_cycle) {
    std::stringstream ss;
    ss << R"(0 |
        1 i 0 6 |
        2 i 0 7 i 1 8 |
        3 i 1 5 i 2 -3 o 1 -2|
        4 i 2 9 i 1 -4 o 3 7 o 0 2$)";
    gr_helper.graph_init_from(ss);
    gr_helper.gr.graphviz_dump();
    auto res = std_like::bellman_ford(gr_helper.gr.begin(), gr_helper.gr.end());
    ASSERT_TRUE(res.has_value() && res == wiki_cycle_res);
}

TEST_F(bellman_ford, wiki_negative_cycle) {
    std::stringstream ss;
    ss << R"(0 |
            1 i 0 -1 |
            2 i 1 -1 o 0 -1 $)";
    gr_helper.graph_init_from(ss);
    gr_helper.gr.graphviz_dump();
    auto res = std_like::bellman_ford(gr_helper.gr.begin(), gr_helper.gr.end());
    ASSERT_TRUE(!res.has_value());
}

