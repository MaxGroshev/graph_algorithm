namespace graphviz {

template<typename T, typename U>
void node_t::print_node (const T* data_node, const U& data, std::ofstream& graphviz_strm) {
    assert(data_node != nullptr);
    assert(graphviz_strm.good());
    
    graphviz_strm << "node" << data_node <<       " [shape      = \"" << shape
               << "\", width = 0.9, height = 0.9,  fillcolor   = \"" << fillcolor
               << "\", style = \""    << style << "\", fontcolor= \" " << fontcolor
               << "\", fontname = \"" << fontname << "\", color = \"" << color
               << "\", label = \"{num: "<< data << "}\"]\n";
}

//-----------------------------------------------------------------------------------------

template<typename T, typename U>
void edge_t::print_edge (const T* node_from, const T* node_to, const U& price, std::ofstream& graphviz_strm) {
    assert(node_from != nullptr && node_to != nullptr);
    assert(graphviz_strm.good());
    graphviz_strm << "node" << node_from << " -> node" << node_to << " [color = \"" << color
               << "\", style = \""    << style << "\", constraint = " << constraint
               << ", fillcolor = \""  << fillcolor << "\",   fontcolor = \"" << fontcolor
               << "\", fontname = \"" << fontname  <<  "\", fontsize = \"" << "13" << "\", headlabel = \"{"<< price << "}\"]\n";
}

void dump_graph_t::run_graphviz(const char* dot_dir, const char* pic_dir) {
    assert (dot_dir != nullptr && pic_dir != nullptr);
    graphviz_strm << "}\n";

    static int num_of_print = 0;
    num_of_print++;
    std::string dest_file =  pic_dir + std::string{"/tree_d"} + std::to_string(num_of_print) + ".png";
    std::string command = "circo " + std::string(dot_dir) + " -q -T png -o " + dest_file;
    std::clog << dest_file << std::endl;
    graphviz_strm.close();
    system (command.c_str());
}

//-----------------------------------------------------------------------------------------
}
