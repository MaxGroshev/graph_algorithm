#include "./ui.hpp"

//-----------------------------------------------------------------------------------------

namespace graph_ui {

using namespace graph;

void run(std::istream &in_strm) {
    graph_helper<int> gr_hand{};

    gr_hand.graph_init_from(in_strm);
    gr_hand.gr.graphviz_dump();    
    // auto res = std_like::djicstra(gr_hand.gr.begin(), gr_hand.gr.end());
    // for (const auto& d : res) {
    //     std::cout << d << std::endl;
    // }
}

} // namespace graph_ui