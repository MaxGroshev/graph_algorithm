namespace graphviz {

template<typename T>
void node_t::print_node (const T* data_node, std::ofstream& graphviz_strm) {
    assert(data_node != nullptr);
    assert(graphviz_strm.good());
    graphviz_strm << "node" << data_node <<       " [shape      = \"" << shape
               << "\", width = 1.1, height = 1.21,  fillcolor   = \"" << fillcolor
               << "\", style = \""    << style << "\", fontcolor= \" " << fontcolor
               << "\", fontname = \"" << fontname << "\", color = \"" << color
               << "\", label = \"{key: "<< data_node->key_  << "| size: " << data_node->size_
               << "| height: "          << data_node->height_  <<"}\"]\n";
}

//-----------------------------------------------------------------------------------------

template<typename T>
void edge_t::print_edge (const T* node_from, const T* node_to, std::ofstream& graphviz_strm) {
    assert(node_from != nullptr && node_to != nullptr);
    assert(graphviz_strm.good());
    graphviz_strm << "node" << node_from << " -> node" << node_to << " [color = \"" << color
               << "\", style = \""    << style << "\", constraint = " << constraint
               << ", fillcolor = \""  << fillcolor << "\",   fontcolor = \"" << fontcolor
               << "\", fontname = \"" << fontname  << "\", label = \"" << label << "\"];\n";
}

//-----------------------------------------------------------------------------------------
}
