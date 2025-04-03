#pragma once

#include <vector>
#include <list>
#include <utility>
#include <graphviz.h>
#include <iterator>
#include <queue>

//-----------------------------------------------------------------------------------------

namespace std_like {
static constexpr size_t max_price = std::numeric_limits<long long>::max();

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

} // namespace std_like