module;
export module graph_alg;

import <vector>;
import <list>;
import <utility>;
import <iterator>;
import <queue>;
import <optional>;
import <iostream>;

//-----------------------------------------------------------------------------------------

namespace std_like {
    
static constexpr size_t max_price = std::numeric_limits<long long>::max();
    
//------------------------------------

class djicstra_helper {
    public:
        std::priority_queue<std::pair<long long, int>> q;
        std::vector<long long> dist;
        std::vector<long long> parents;

        djicstra_helper(size_t size, long long start) : dist(size, max_price), parents(size, -1) {
            dist[start] = 0;
            q.push(std::make_pair(0, start));
        };
};

class djicstra_res {
    public:
        std::vector<long long> dist;
        std::vector<long long> parents;

        djicstra_res(std::vector<long long>&& dist_, std::vector<long long>&& parents_) : 
            dist(std::move(dist_)), parents(std::move(parents_)) {
        };
};

export template <typename iter>
djicstra_res djicstra(iter start, iter fin, int from) {
    djicstra_helper helper{static_cast<size_t>(fin - start), from};    
    
    while (!helper.q.empty()) {
        auto len = -helper.q.top().first;
        auto i = helper.q.top().second;
        helper.q.pop();
        if (len > helper.dist[i]) {
            continue;
        }
        for (const auto& node : *(start + i)) { // each in adj list
            if (helper.dist[node.index] > helper.dist[i] + node.price) {
                helper.dist[node.index] = helper.dist[i] + node.price;
                helper.parents[node.index] = i;
                helper.q.push(std::make_pair(-helper.dist[node.index], node.index));
            }
        }
    }
    return {std::move(helper.dist), std::move(helper.parents)};
}

//------------------------------------
//make class hadler for ballman ford

class bellman_ford_helper {
    public:
        size_t gr_size = 0; 
        std::vector<std::pair<int, long long>> dist;

        bellman_ford_helper(size_t size) : dist(size, std::make_pair(-1, max_price)), gr_size(size) {
            dist[0].first  = 0;
            dist[0].second = 0;
        };
};

template <typename iter, typename contT>
bool relax(iter u, iter v, contT& dist) {
    if (dist[v->index].second > dist[u->index].second + v->price) {
        dist[v->index].second = dist[u->index].second + v->price;
        dist[v->index].first  = u->index;
        return true;
    }
    return false;
}

export template <typename iter>
std::optional<std::vector<std::pair<int, long long>>> bellman_ford(iter start, iter fin) { // return node and price
    bellman_ford_helper helper{static_cast<size_t>(fin - start)};

    for (int i = 1; i < helper.gr_size; i++) {
        for (auto lst = start; lst != fin; lst++) {
            auto head  = lst->begin();
            for (auto it = std::next(head); it != lst->end(); ++it) {
                relax(head, it, helper.dist);
            }
        }
    }

    for (auto lst = start; lst != fin; lst++) {
        auto head  = lst->begin();
        for (auto it = std::next(head); it != lst->end(); ++it) {
            if (relax(head, it, helper.dist)) {
                return std::nullopt;
            }   
        }
    }
    return helper.dist;
}

//------------------------------------

export template <typename T>
std::optional<std::vector<std::vector<int>>> jhonson(T& graph) {
    std::vector<std::vector<int>> res(graph.size(), std::vector<int>(graph.size(), 0));

    auto gr = graph;
    gr.append_front_connect_with_others_out(0);
    gr.graphviz_dump();
    auto bellman_ford_output = bellman_ford(gr.begin(), gr.end());
    if (!bellman_ford_output.has_value()) {
        std::cerr << "Has negative cycle";
        return std::nullopt;
    }
    auto sht_path {std::move(bellman_ford_output.value())};
    for (int i = 0; i < gr.size(); i++) {
        auto head  = gr[i].begin();
        for (auto it = std::next(head); it != gr[i].end(); ++it) {
            it->price = it->price + sht_path[i].second - sht_path[it->index].second;
        }
    }
    gr.graphviz_dump();
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
    gr_enh.graphviz_dump();
    sht_path.erase(sht_path.begin());
    for (int i = 0; i < gr_enh.size(); i++) {
        auto dj_path = djicstra(gr_enh.begin(), gr_enh.end(), i).dist;
        for (int j = 0; j < gr_enh.size(); j++) {
            res[i][j] = dj_path[j] - sht_path[i].second + sht_path[j].second;
        }
    }

    return res;
}

} // namespace std_like