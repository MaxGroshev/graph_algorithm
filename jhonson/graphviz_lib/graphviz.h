#pragma once
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <fstream>
#include <vector>
#include <string>
#include <cassert>

//-----------------------------------------------------------------------------------------

namespace graphviz {

struct node_t
{
    std::string shape     = "Mrecord";
    std::string fillcolor = "#0083C3";
    std::string style     = "filled";
    std::string fontcolor = "black";
    std::string fontname  = "Calibri";
    std::string color     = "black";
    std::string margin    = "";
    std::string label     = "";

    template<typename T, typename U>
    inline void print_node (const T* data_node, const U& data,std::ofstream& graphviz_strm);
};

struct edge_t
{
    std::string constraint = "true";
    std::string fillcolor  = "#8DB6CD";
    std::string style      = "";
    std::string fontcolor  = "";
    std::string fontname   = "Calibri";
    std::string color      = "#8DB6CD";
    std::string label      = "";

    template<typename T, typename U>
    inline void print_edge(const T* node_from, const T* node_to, const U& price, std::ofstream& graphviz_strm);
};

struct dump_graph_t
{
    std::ofstream graphviz_strm;
    const char* orientation = "LR";
    const char* splines = "true";

    node_t graph_node;
    edge_t graph_edge;

    dump_graph_t(const char* dot_dir) {
        graphviz_strm.open (dot_dir);
        assert (graphviz_strm.good());

        graphviz_strm << "digraph   \n{\n";
        graphviz_strm << "rankdir = \"" << orientation << "\"\n";
        graphviz_strm << "splines = \"" << splines << "\"\n\n";
    }

    inline void close_input() {
        graphviz_strm << "}\n";
        graphviz_strm.close();
    };
    inline void run_graphviz (const char* dot_dir, const char* pic_dir);
};

//-----------------------------------------------------------------------------------------

}

#include "graphviz.tpp"
