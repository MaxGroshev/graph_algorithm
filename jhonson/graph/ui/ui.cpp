#include "./ui.hpp"

//-----------------------------------------------------------------------------------------

namespace graph_ui {

using namespace graph;

void run(std::istream &in_strm) {
    graph::graph_t<int> gr;
    char type_of_edge = '\0';
    int data = 0;
    size_t edge = 0;
    size_t price = 0;

    
    bool end_in = false;
    while (!end_in) { //^D to exit from cin input
        in_strm >> data;
        std::vector<std::pair<size_t, size_t>> in;
        std::vector<std::pair<size_t, size_t>> out;

        std::cout << data << std::endl;
        while (type_of_edge != '|') {
            in_strm >> type_of_edge;
            if (type_of_edge == '$') {
                end_in = true;
                break;
            }
            if (type_of_edge == '|') {
                type_of_edge = '\0';
                break;
            }
            in_strm >> edge >> price;
            if (type_of_edge == 'o') {
                out.push_back(std::pair<size_t, size_t>(edge, price));
            } else if (type_of_edge == 'i') {
                in.push_back(std::pair<size_t, size_t>(edge, price));
            } 
        }
        gr.append(data, in.begin(), in.end(), out.begin(), out.end());
    }
    gr.graphviz_dump();
    std::cout << std::endl;
}

} // namespace graph_ui