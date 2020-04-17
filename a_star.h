//
// Created by psmolnik on 4/14/2020.
//

#ifndef BRIDGES_A_STAR_H
#define BRIDGES_A_STAR_H

#include "graph.h"
#include <queue>

template<class weight_type = int, class node_type = unsigned int>
class A_star {
private:
    vector<vector<pair<node_type, weight_type> > > adj_list;
    vector<coord> ver_coord;
    unsigned int nodes_number = 0;
    unsigned int edges_number = 0;
    weight_type inf = 0;

public:

    void set_ver_coord(vector<coord> &ver_coord_) {
        ver_coord = ver_coord_;
    }


    A_star(vector<vector<pair<node_type, weight_type> > > adj_list_, unsigned int nodes_number_,
           unsigned int edges_number_) {
        adj_list = adj_list_;
        nodes_number = nodes_number_;
        edges_number = edges_number_;
        inf = std::numeric_limits<node_type>::max();
    }

    int do_a_star(node_type start_ver, node_type end_ver, vector<node_type> &path) {
        vector<weight_type> dist;
        dist.resize(nodes_number);
        dist.assign(nodes_number, inf);
        dist[start_ver] = 0;

        vector<bool> visit(nodes_number, false);
        vector<node_type> p(nodes_number, -1);
        auto comparator = [&](node_type a_ver, node_type b_ver) {
            return (dist[a_ver] + calculate_H(a_ver, end_ver)) > (dist[b_ver] + calculate_H(b_ver, end_ver));
        };
        vector<node_type> ver_queue;
        ver_queue.push_back(start_ver);
        std::make_heap(ver_queue.begin(), ver_queue.end(), comparator);

        while (!ver_queue.empty()) {
            int ver_for_visit = ver_queue.front();
            std::pop_heap(ver_queue.begin(), ver_queue.end(), comparator);
            ver_queue.pop_back();
            if (visit[ver_for_visit])
                continue;
            if (ver_for_visit == end_ver)
            {
                break;
            }

            for (unsigned int j = 0; j < adj_list[ver_for_visit].size(); ++j)
            {
                int next_ver = adj_list[ver_for_visit][j].first;
                int weight = adj_list[ver_for_visit][j].second;
                if ((dist[ver_for_visit] + weight) < dist[next_ver]) {
                    dist[next_ver] = dist[ver_for_visit] + weight;
                    p[next_ver] = ver_for_visit;
                    ver_queue.push_back(next_ver);
                    std::push_heap(ver_queue.begin(), ver_queue.end(), comparator);
                }
            }
            visit[ver_for_visit] = true;
        }
        node_type i = end_ver;
        node_type j = i;
        while (1) {
            if ((j == start_ver) || (j == -1))
                break;
            path.push_back(p[j]);
            j = p[j];

        }
        reverse(path.begin(), path.end());
        path.push_back(i);


        return dist[end_ver];
    }

    double calculate_H(node_type cur_ver, node_type end_ver) {
        if (ver_coord.empty())
            return 1;
        else
            return sqrt((ver_coord[cur_ver].x - ver_coord[end_ver].x) * (ver_coord[cur_ver].x - ver_coord[end_ver].x)
                       +(ver_coord[cur_ver].y - ver_coord[end_ver].y) * (ver_coord[cur_ver].y - ver_coord[end_ver].y));
    }

};
#endif //BRIDGES_A_STAR_H
