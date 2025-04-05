#pragma once

#include <vector>
#include <list>
#include <utility>
#include <graphviz.h>
#include <iterator>
#include <queue>
#include <optional>

//-----------------------------------------------------------------------------------------

namespace std_like {
static constexpr size_t max_price = std::numeric_limits<long long>::max();

//------------------------------------


template <typename iter>
std::vector<long long> djicstra(iter start, iter fin) {
    std::vector<long long> dist(fin - start, max_price);
    dist[0] = 0;
    std::priority_queue<std::pair<long long, int>> q;
    q.push(std::make_pair(0, 0));
    
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


} // namespace std_like