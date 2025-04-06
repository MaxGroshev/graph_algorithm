#pragma once

using namespace graph;

//-----------------------------------------------------------------------------------------

class djicstra : public ::testing::Test {
    protected:
        graph_ui::graph_helper<int> gr_helper;
        std::vector<long long> wiki_res = {0, 7, 9, 11, 20, 20};

        void SetUp() {}
};

//----------------c-------------------------------------------------------------------------

TEST_F(djicstra, wiki) {
    std::stringstream ss;
    ss << R"(0 |
        1 i 0 7 |
        2 i 0 9 i  1 10 |
        3 i 0 14 i 2 2 |
        4 i 2 11 i 1 15  |
        5 i 3 9  i 4 6 $)";
    gr_helper.graph_init_from(ss);
    gr_helper.gr.graphviz_dump();
    auto res = std_like::djicstra(gr_helper.gr.begin(), gr_helper.gr.end(), 0);
    ASSERT_TRUE(res == wiki_res);
}

