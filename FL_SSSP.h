//
// Created by Артем Аросланкин on 11.03.2020.
//

#ifndef BRIDGES_FL_SSSP_H
#define BRIDGES_FL_SSSP_H


#include <limits.h>
#include "graph.h"

template <class weight_type = unsigned int, class node_type = unsigned int>
struct pair_distance_path
{
    weight_type distance;
    unsigned int* path;

    pair_distance_path();

    pair_distance_path(weight_type distance, unsigned int* & path) {
        this->distance = distance;
        this->path = path;
    }
    void set_new_path(vector<node_type> new_path) {
        if (path != nullptr) {
            delete path;
        }
        path = new unsigned int[new_path.size()];
        for(size_t i = 0; i < new_path.size(); i++) {
            path[i] = new_path[i];
        }
    }
};


unsigned int max_weight = 100000;


template<class weight_type = unsigned int, class node_type = unsigned int>
class sssp_floid
{
private:
    vector<vector< weight_type > >  sssp_map;
    vector<vector< vector<node_type> > >  shortest_paths;
    vector<vector<node_type>> * adj_map;
    node_type nodes_number = 0;

public:
    sssp_floid() = default;

    sssp_floid(vector<vector<node_type>>& adj_map) {
        nodes_number = adj_map.size();
        sssp_map.resize(nodes_number);
        shortest_paths.resize(nodes_number);
        for (node_type i = 0; i < nodes_number; i++) {
            sssp_map[i].resize(nodes_number);
            shortest_paths[i].resize(nodes_number);
            for (node_type j = 0; j < nodes_number; j++) {
                if (adj_map[i][j] != 0) {
                    sssp_map[i][j] = adj_map[i][j];
                    shortest_paths[i][j].push_back(j);
                } else {
                    sssp_map[i][j] = max_weight;
                }
            }
        }

        for (node_type k = 0; k < nodes_number; ++k) {
            for (node_type i = 0; i < nodes_number; ++i) {
                for (node_type j = 0; j < nodes_number; ++j) {
                    weight_type possible_new_weigth = sssp_map[i][k] + sssp_map[k][j];
                    if(possible_new_weigth < sssp_map[i][j]) {
                        sssp_map[i][j] = possible_new_weigth;
                        shortest_paths[i][j] = shortest_paths[i][k];
                        for(auto m = 0; m < shortest_paths[k][j].size(); m++) {
                            shortest_paths[i][j].push_back(shortest_paths[k][j][m]);
                        }
                    }
                }
            }
        }
    }

    void print_result() {
        for (auto i = 0 ; i < nodes_number; i ++) {
            for (auto j = 0; j < nodes_number; j++) {
                if( sssp_map[i][j] != max_weight) {
                    cout << i << " " << j << "with distance = " << sssp_map[i][j] << "\t path: ";
                    for (auto k:shortest_paths[i][j]) {
                        cout << k << " ";
                    }
                    cout << endl;
                }
            }
        }
    }
};


#endif //BRIDGES_FL_SSSP_H
