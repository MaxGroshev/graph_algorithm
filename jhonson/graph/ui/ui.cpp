#include "ui.hpp"

//-----------------------------------------------------------------------------------------

namespace graph_ui {

using namespace graph;

void run(std::istream &in_strm) {
    graph_helper<int> gr_hand{};

    gr_hand.graph_init_from(in_strm);

    auto res = std_like::djicstra(gr_hand.gr.begin(), gr_hand.gr.end(), 0);
    for(const auto& r : res) {
        std::cout << r << std::endl;
    }
    gr_hand.gr.graphviz_dump();    
}

} // namespace graph_ui