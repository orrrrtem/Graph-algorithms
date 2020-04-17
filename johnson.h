#include "graph.h"
#include <queue>

template<class weight_type = int, class node_type = unsigned int>
class Johnson {
private:
    vector<vector<vector<node_type> > > shortest_paths;
    vector<vector<pair<node_type, weight_type> > > adj_list;
    vector<vector<weight_type>> modified_distance_map;
    vector<vector<weight_type>> real_distance_map;
    unsigned int nodes_number = 0;
    weight_type inf = 0;
    bool negative_cycle = false;
public:
    vector<vector<weight_type>> get_modified_distance_map() const
    {
        return modified_distance_map;
    }

    vector<vector<vector<node_type> > > get_shortest_path() const
    {
        return shortest_paths;
    }

    Johnson(vector<vector<pair<node_type, weight_type> > > adj_list_,
            unsigned int nodes_number_)
    {
        adj_list = adj_list_;
        nodes_number = nodes_number_;
        shortest_paths.resize(nodes_number);
        inf = std::numeric_limits<weight_type>::max();
    }

    bool do_johnson()
    {
        vector<vector<pair<node_type, weight_type> > > new_edges = adj_list;
        new_edges.resize(nodes_number + 1);
        for (unsigned int i = 0; i < nodes_number; ++i)
            new_edges[nodes_number].push_back(make_pair(i, 0));
        vector<weight_type> h;
        if (!bellman_ford(nodes_number, h, new_edges, (nodes_number + 1))) {
            cout << "there is negative cycle" << endl;
            negative_cycle = true;
            return false;
        }
        vector<vector<pair<weight_type, weight_type> > > modified_adj_list = adj_list;
        for (unsigned int i = 0; i < modified_adj_list.size(); ++i)
            for (unsigned int j = 0; j < modified_adj_list[i].size(); ++j) {
                modified_adj_list[i][j].second =
                        modified_adj_list[i][j].second + h[i] - h[modified_adj_list[i][j].first];
            }
        modified_distance_map.resize(nodes_number);
        for (unsigned int i = 0; i < nodes_number; ++i) {
            dijkstra(i, modified_distance_map[i], modified_adj_list, shortest_paths[i]);
        }
        return true;


    }

    void dijkstra(node_type ver, vector<weight_type> &dist,
                  const vector<vector<pair<node_type, weight_type> > > &graph_adj_list,
                  vector<vector<node_type> > &path)
    {
        dist.resize(nodes_number);
        dist.assign(nodes_number, inf);
        dist[ver] = 0;
        vector<bool> visit(nodes_number, false);
        vector<node_type> p(nodes_number, -1);
        path.resize(nodes_number);
        auto comparator = [&](node_type a_ver, node_type b_ver) { return (dist[a_ver]) > (dist[b_ver]); };
        vector<node_type> ver_queue;
        ver_queue.push_back(ver);
        std::make_heap(ver_queue.begin(), ver_queue.end(), comparator);
        while (!ver_queue.empty()) {
            int ver_for_visit = ver_queue.front();
            std::pop_heap(ver_queue.begin(), ver_queue.end(), comparator);
            ver_queue.pop_back();

            for (unsigned int j = 0; j < graph_adj_list[ver_for_visit].size(); ++j)
            {
                node_type next_ver = graph_adj_list[ver_for_visit][j].first;
                weight_type weight = graph_adj_list[ver_for_visit][j].second;
                if ((dist[ver_for_visit] + weight) < dist[next_ver])
                {
                    dist[next_ver] = dist[ver_for_visit] + weight;
                    p[next_ver] = ver_for_visit;
                    ver_queue.push_back(next_ver);
                    std::push_heap(ver_queue.begin(), ver_queue.end(), comparator);
                }
            }
            visit[ver_for_visit] = true;

        }

        for (unsigned int i = 0; i < p.size(); ++i) {
            node_type j = i;
            while (1) {
                if ((j == ver) || (j == -1))
                    break;
                path[i].push_back(p[j]);
                j = p[j];

            }
            reverse(path[i].begin(), path[i].end());
            path[i].push_back(i);
        }

    }

    bool bellman_ford(int ver, vector<weight_type> &dist,
                      const vector<vector<pair<node_type, weight_type> > > &graph_adj_list,
                      unsigned int nodes_num)
    {
        dist.resize(nodes_num);
        dist.assign(nodes_num, inf);
        dist[ver] = 0;
        int x;
        //extra iteration for negative cycle check
        for (unsigned int k = 0; k < nodes_num; ++k) {
            x = -1;
            for (unsigned int i = 0; i < nodes_num; ++i) {
                for (unsigned j = 0; j < graph_adj_list[i].size(); ++j) {
                    if ((dist[graph_adj_list[i][j].first] < inf) || (i == ver)) {
                        if (dist[graph_adj_list[i][j].first] > (dist[i] + graph_adj_list[i][j].second)) {
                            dist[graph_adj_list[i][j].first] = std::max(-inf, dist[i] + graph_adj_list[i][j].second);
                            x = graph_adj_list[i][j].first;
                        }
                    }
                }


            }

        }

        if (x == -1)
            return true;
        else
            return false;
    }

    vector<vector<weight_type> > get_real_distance_map()
    {
        real_distance_map.resize(nodes_number);
        for (unsigned int i = 0; i < nodes_number; ++i) {
            real_distance_map[i].resize(nodes_number);
            real_distance_map[i].assign(nodes_number, 0);
            if (negative_cycle)
                continue;
            for (unsigned int j = 0; j < nodes_number; ++j) {
                for (int k = 0; k < shortest_paths[i][j].size() - 1; ++k) {
                    if (modified_distance_map[i][j] == inf) {
                        real_distance_map[i][j] = inf;
                        break;
                    } else {
                        for (unsigned int m = 0; m < adj_list[shortest_paths[i][j][k]].size(); ++m) {
                            if (adj_list[shortest_paths[i][j][k]][m].first == shortest_paths[i][j][k + 1])
                                real_distance_map[i][j] += adj_list[shortest_paths[i][j][k]][m].second;
                        }
                    }
                }
            }

        }
        return real_distance_map;
    }

    void print_results()
    {
        get_real_distance_map();
        for (unsigned int i = 0; i < nodes_number; ++i) {
            for (unsigned int j = 0; j < nodes_number; ++j) {
                if (modified_distance_map[i][j] != inf) {
                    cout << i << " " << j << " with modified distance = " << modified_distance_map[i][j]
                         << "\t with real distance = " << real_distance_map[i][j] << "\t path: ";
                    for (auto k:shortest_paths[i][j]) {
                        cout << k << " ";
                    }
                    cout << endl;
                }
            }
        }
    }
};