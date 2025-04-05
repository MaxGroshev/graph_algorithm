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
    std::vector<long long> dist(fin - start, 228);
    dist[1] = 0;
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
template <typename T>
std::vector<long long> dijkstra_j(int start, T& graph) {
    std::vector<long long> dist(graph.size(), max_price);
    dist[start] = 0;
    std::set<std::pair<long long, int> > st;
    st.emplace(std::make_pair(0, start));
    while (st.size()) {
        int v = st.begin()->second; st.erase(st.begin());
        for (auto& e : graph[v]) {
            if (dist[e.index] > dist[v] + e.price) {
                st.erase({ dist[e.index], e.index });
                dist[e.index] = dist[v] + e.price;
                st.emplace(dist[e.index], e.index);
            }
        }
    }
    return dist;
}

template <typename T>
std::vector<std::vector<int>> jhonson(T& graph) {
    std::vector<std::vector<int>> res(graph.size(), std::vector<int>(graph.size(), 0));

    auto gr = graph;
    gr.append_front_connect_with_others_out(0);
    gr.graphviz_dump();
    if (!bellman_ford(gr).has_value()) {
        std::cerr << "Has negative cycle";
        // return {{}};
    }
    auto sht_path = bellman_ford(gr).value();
    // sht_path.erase(sht_path.begin());

    for (int i = 0; i < gr.size(); i++) {
        auto head  = gr[i].begin();
        for (auto it = std::next(head); it != gr[i].end(); ++it) {
            it->price = it->price + sht_path[i].second - sht_path[it->index].second;
        }
    }
    gr.graphviz_dump();
    gr.dump();
    for (int i = 1; i < gr.size(); i++) {
        auto dj_path = dijkstra_j(0, gr);
        // auto dj_path = djicstra(gr.begin(), gr.end(), 1);
        for (const auto& n : dj_path) {
            std::cout << n << "   ";
        }
        std::cout << std::endl;
        // for (int j = 1; j < gr.size(); j++)
        //     res[i - 1][j - 1] = dj_path[j] + sht_path[i].second - sht_path[j].second;
    }
    // for (const auto& i : res) {
    //     for (const auto& j : i) {
    //         std::cout << j << " ";
    //     }
    //     std::cout << std::endl;
    // }
    return res;

}


} // namespace std_like