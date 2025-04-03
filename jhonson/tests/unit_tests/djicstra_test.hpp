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
    std::cout << "Current path is " << std::filesystem::current_path() << '\n';
    std::ifstream in("../tests/end_to_end_tests/my_test_dat/0.dat", std::ios_base::in);
    gr_helper.graph_init_from(in);
    in.close();

    auto res = std_like::djicstra(gr_helper.gr.begin(), gr_helper.gr.end());
    ASSERT_TRUE(res == wiki_res);
}

