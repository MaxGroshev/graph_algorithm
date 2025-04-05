#pragma once

#include "utils.hpp"
#include <vector>
#include <list>
#include <utility>
#include <graphviz.h>
#include <iterator>

//-----------------------------------------------------------------------------------------
//probably add iterators to iterate into graph
namespace graph {

template <typename T = int>
class graph_t final {
    class node_t final {
        public:
            node_t(const T& data_) : data(data_) {};
            T data;
    };
    
    class node_wrap_t final {
        public:
            node_wrap_t(long long index_, long long price_) : index(index_), price(price_) {}; 
            long long index;
            long long price = 0;
    };
    
    private:
        long long node_cnt = 0;
        std::vector<node_t> nodes; //may be list
        std::vector<std::list<node_wrap_t>> node_adj; //un_map in advance
        void relax() {

        };
        void graphviz_dump_impl(graphviz::dump_graph_t& tree_dump) const {
            for (auto node_l : node_adj) {
                auto head = node_l.front();
                tree_dump.graph_node.print_node(&nodes[head.index], nodes[head.index].data, tree_dump.graphviz_strm);
                for (auto it = std::next(node_l.begin()); it != node_l.end(); ++it) {
                    tree_dump.graph_edge.print_edge(&nodes[head.index], &nodes[it->index], it->price, tree_dump.graphviz_strm);
                }
            }
        }
    public:
        //utils; weak implementation of iterators just for fun
        typedef typename std::vector<std::list<node_wrap_t>>::iterator iterator;
        typedef typename std::vector<std::list<node_wrap_t>>::reverse_iterator reverse_iterator;
        typedef typename std::vector<std::list<node_wrap_t>>::const_iterator const_iterator;
        typedef typename std::vector<std::list<node_wrap_t>>::const_reverse_iterator const_reverse_iterator;

        iterator begin()              { return node_adj.begin(); }
        iterator end()                { return node_adj.end(); }
        const_iterator cbegin() const { return node_adj.cbegin(); }
        const_iterator cend()   const { return node_adj.cend(); }
        const_iterator begin()  const { return node_adj.begin(); }
        const_iterator end()    const { return node_adj.end(); }

        std::list<node_wrap_t>& operator[](const int index) { return node_adj[index]; }
        size_t size() const {return nodes.size(); } 
        
        template <typename iter> 
            void append(const T& data, iter in_b, iter in_e, iter out_b, iter out_e) {
                nodes.push_back(node_t{data});
                std::list<node_wrap_t> node_list = {node_wrap_t(node_cnt, 0)};
                node_adj.push_back(std::move(node_list));
                for (auto it = in_b; it != in_e; it++) {
                    node_adj[it->first].push_back(node_wrap_t(node_cnt, it->second));
                }
                for (auto it = out_b; it != out_e; it++) {
                    node_adj[node_cnt].push_back(node_wrap_t(it->first, it->second));
                }
                node_cnt++;
            };
            void graphviz_dump() const {
                graphviz::dump_graph_t dump("../graphviz_lib/graph_dump.dot"); 
                graphviz_dump_impl(dump);
                dump.run_graphviz("../graphviz_lib/graph_dump.dot", "../graphviz_lib/dump");
                dump.close_input();
            }
            void remove();
            bool bellman_ford();
};

}