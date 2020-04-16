//
// Created by Артем Аросланкин on 11.03.2020.
//

#ifndef BRIDGES_FL_SSSP_H
#define BRIDGES_FL_SSSP_H


#include <limits.h>
#include "graph.h"



template<class weight_type = float>
vector<vector<weight_type>> adjacency_matrix_from_COO(const vector<unsigned int>& source_vertices, const vector<unsigned int>& destination_vertices,const vector<weight_type> & weights ) {
    if(source_vertices.size() != destination_vertices.size() || source_vertices.size() != weights.size()) {
        vector<vector<weight_type >> empty;
        return empty;
    }
    vector<vector<weight_type >> adj(source_vertices.size());
    for(size_t i = 0; i < source_vertices.size(); ++i) {
        adj[i] = vector<weight_type >(source_vertices.size(),0);
    }
    for( size_t i = 0; i < source_vertices.size(); ++i) {
        adj[source_vertices[i]][destination_vertices[i]] = weights[i];
        adj[destination_vertices[i]][source_vertices[i]] = weights[i];
    }
    return adj;
}

template<class weight_type = float>
void display_adjacency_list(const vector<vector<pair<unsigned int, weight_type> > >& adj) {
    cout << "Adjacency list format: source: (neighbor1, its weight), ... " << endl;
    for (auto i = 0; i < adj.size(); ++i) {
        cout << i << ": ";
        for (auto j = 0; j < adj[i].size(); ++j)
            cout << "(" << adj[i][j].first << ", " << adj[i][j].second << ")  ";
        cout << endl;
    }
}

template<class weight_type = float>
vector<vector<pair<unsigned int, weight_type> > > adjacency_list_from_COO(const vector<unsigned int>& source_vertices,
                                                        const vector<unsigned int>& destination_vertices,
                                                        const vector<weight_type> & weights,
                                                        bool to_display = false) {
    if(source_vertices.size() != destination_vertices.size() || source_vertices.size() != weights.size()) {
        vector<vector<pair<unsigned int, weight_type> > > empty;
        return empty;
    }
    vector<vector<pair<unsigned int, weight_type> > > adjacency_list(source_vertices.size());
    for( size_t i = 0; i < source_vertices.size(); ++i) {
        adjacency_list[source_vertices[i]].push_back(make_pair(destination_vertices[i], weights[i]));
    }
    if (to_display) {
        display_adjacency_list<weight_type>(adjacency_list);
    }
    return adjacency_list;
}



template <class weight_type = float, class node_type = unsigned int>
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


unsigned int max_weight =  std::numeric_limits<int>::max();;


template<class weight_type = float, class node_type = unsigned int>
class apsp_floid
{
private:
    vector<vector< weight_type > >  sssp_map;
    vector<vector< vector<node_type> > >  shortest_paths;
    node_type nodes_number = 0;

public:
    apsp_floid() = default;

    apsp_floid(const vector<vector<pair<node_type, weight_type> > >& adj_list) {
        cout << "APSP FL started" << endl;
        nodes_number = adj_list.size();
        sssp_map.resize(nodes_number);
        shortest_paths.resize(nodes_number);

        for (auto i = 0; i < nodes_number; ++i) {
            sssp_map[i].resize(nodes_number);
            fill(sssp_map[i].begin(), sssp_map[i].begin() + nodes_number, max_weight);
            shortest_paths[i].resize(nodes_number);
            for (auto j_weight: adj_list[i]) {
                    sssp_map[i][j_weight.first] = j_weight.second;
                    shortest_paths[i][j_weight.first].push_back(j_weight.first);
            }
        }

        for (node_type k = 0; k < nodes_number; ++k) {
            for (node_type i = 0; i < nodes_number; ++i) {
                for (node_type j = 0; j < nodes_number; ++j) {
                    if ( sssp_map[i][k] == max_weight || sssp_map[k][j] == max_weight) {
                        continue;
                    }
                    weight_type possible_new_weight = sssp_map[i][k] + sssp_map[k][j];
                    if(possible_new_weight < sssp_map[i][j]) {
                        sssp_map[i][j] = possible_new_weight;
                        shortest_paths[i][j] = shortest_paths[i][k];
                        for(auto m = 0; m < shortest_paths[k][j].size(); m++) {
                            shortest_paths[i][j].push_back(shortest_paths[k][j][m]);
                        }
                    }
                }
            }
        }
    }
    vector<vector< weight_type > > get_distance_map() const
    {
        return sssp_map;
    }
    void print_result() {
        for (auto i = 0 ; i < nodes_number; i ++) {
            for (auto j = 0; j < nodes_number; j++) {
                if( sssp_map[i][j] != max_weight) {
                    cout << i << " " << j << "\twith distance = " << sssp_map[i][j] << "\t path: ";
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
