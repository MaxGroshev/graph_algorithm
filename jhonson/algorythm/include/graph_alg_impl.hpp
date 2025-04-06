#pragma once

#include <vector>
#include <list>
#include <utility>
#include <graphviz.h>
#include <iterator>
#include <queue>
#include <optional>
#include <set>

//-----------------------------------------------------------------------------------------

//try to make module here
namespace std_like {
    static constexpr size_t max_price = std::numeric_limits<long long>::max();
    
    //------------------------------------
    
template <typename iter>
std::vector<long long> djicstra(iter start, iter fin, int from) {
    std::vector<long long> dist(fin - start, max_price);
    dist[from] = 0;
    std::priority_queue<std::pair<long long, int>> q;
    q.push(std::make_pair(0, from));
    
    while (!q.empty()) {
        auto len = -q.top().first;
        auto i = q.top().second;
        q.pop();
        if (len > dist[i]) {
            continue;
        }
        for (const auto& node : *(start + i)) { // each in adj list
            if (dist[node.index] > dist[i] + node.price) {
                dist[node.index] = dist[i] + node.price;
                q.push(std::make_pair(-dist[node.index], node.index));
            }
        }
    }
    return dist;
}

//------------------------------------
//make class hadler for ballman ford
template <typename iter, typename contT>
bool relax(iter u, iter v, contT& dist) {
    if (dist[v->index].second > dist[u->index].second + v->price) {
        dist[v->index].second = dist[u->index].second + v->price;
        dist[v->index].first  = u->index;
        return true;
    }
    return false;
}
//make iteraror verson
template <typename T>
std::optional<std::vector<std::pair<int, long long>>> bellman_ford(T& graph) { // return node and price
    std::vector<std::pair<int, long long>> dist (graph.size(), std::make_pair(-1, max_price));
    dist[0].first  = 0;
    dist[0].second = 0;

    for (int i = 1; i < graph.size() - 1; i++) {
        for (int j = 0; j < graph.size(); j++) {
            auto head  = graph[j].begin();
            for (auto it = std::next(head); it != graph[j].end(); ++it) {
                relax(head, it, dist);
            }
        }
    }

    for (int i = 1; i < graph.size(); i++) {
        auto head  = graph[i].begin();
        for (auto it = std::next(head); it != graph[i].end(); ++it) {
            if (relax(head, it, dist)) {
                return {};
            }   
        }
    }
    return dist;
} 

//------------------------------------

template <typename T>
std::optional<std::vector<std::vector<int>>> jhonson(T& graph) {
    std::vector<std::vector<int>> res(graph.size(), std::vector<int>(graph.size(), 0));

    auto gr = graph;
    gr.append_front_connect_with_others_out(0);
    gr.graphviz_dump();
    if (!bellman_ford(gr).has_value()) {
        std::cerr << "Has negative cycle";
        return std::nullopt;
    }
    auto sht_path = bellman_ford(gr).value();
    for (int i = 0; i < gr.size(); i++) {
        auto head  = gr[i].begin();
        for (auto it = std::next(head); it != gr[i].end(); ++it) {
            it->price = it->price + sht_path[i].second - sht_path[it->index].second;
        }
    }

    auto gr_enh = graph;
    for (int i = 1; i < gr.size(); i++) {
        auto head  = gr[i].begin();
        for (int it = 0; it != gr[i].size(); ++it) {
            auto iter1 = gr_enh[i - 1].begin();
            auto iter2 = gr[i].begin();
            std::advance(iter1, it);
            std::advance(iter2, it);
            iter1->price = iter2->price;
            iter1->index = iter2->index - 1;
        }
    }
    sht_path.erase(sht_path.begin());
    for (int i = 0; i < gr_enh.size(); i++) {
        auto dj_path = djicstra(gr_enh.begin(), gr_enh.end(), i);
        for (int j = 0; j < gr_enh.size(); j++) {
            res[i][j] = dj_path[j] - sht_path[i].second + sht_path[j].second;
        }
    }
    for (const auto& i : res) {
        for (const auto& j : i) {
            std::cout << j << " ";
        }
        std::cout << std::endl;
    }
    return res;

}
} // namespace std_like