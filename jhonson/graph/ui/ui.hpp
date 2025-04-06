#pragma once

//-----------------------------------------------------------------------------------------

#include "debug_utils.hpp"
#include "graph_impl.hpp"
#include "graphviz.h"
#include "time_control.hpp"
import <utility>;
import <vector>;
import <iostream>;
import graph_alg;

//-----------------------------------------------------------------------------------------

namespace graph_ui {

using namespace time_control;

void run(std::istream &in_strm = std::cin);

template <typename T>
struct graph_helper {
    public:
        graph::graph_t<T> gr;
        
        void graph_init_from(std::istream &in_strm = std::cin) {
            char type_of_edge = '\0';
            int data = 0;
            long long edge = 0;
            long long price = 0;

            bool end_in = false;
            while (!end_in) { //^D to exit from cin input
                in_strm >> data;
                std::vector<std::pair<size_t, size_t>> in;
                std::vector<std::pair<size_t, size_t>> out;

                // std::cout << data << std::endl;
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
                    // std::cout << edge << "  " << price << std::endl;
                    if (type_of_edge == 'o') {
                        out.push_back(std::pair<size_t, size_t>(edge, price));
                    } else if (type_of_edge == 'i') {
                        in.push_back(std::pair<size_t, size_t>(edge, price));
                    } else if (type_of_edge == 'b') {
                        out.push_back(std::pair<size_t, size_t>(edge, price));
                        in.push_back(std::pair<size_t, size_t>(edge, price));
                    }
                }
                gr.append(data, in.begin(), in.end(), out.begin(), out.end());
            }
        }
};

} // namespace graph_ui
