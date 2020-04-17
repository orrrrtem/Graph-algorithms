//
// Created by Polina Smolnikova on 4/13/2020.
//

#ifndef BRIDGES_TESTS_SHORTEST_PATHS_H
#define BRIDGES_TESTS_SHORTEST_PATHS_H

#include <boost/property_map/property_map.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/johnson_all_pairs_shortest.hpp>
#include <boost/graph/floyd_warshall_shortest.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/graph_traits.hpp>
#include "graph.h"
using namespace boost;

typedef adjacency_list<vecS, vecS, directedS, no_property, property < edge_weight_t, int > > Graph;
typedef pair<int, int> Edge;
typedef graph_traits <Graph>::vertex_descriptor vertex_descriptor;

double floid_all_pairs_shortest_test(unsigned int num_check)
{
    int accuracy = 0;
    for(unsigned int step = 0; step < num_check; step++)
    {
        Adj_list adj_list;
        unsigned int num_nodes = 20;
        vector<vector<pair<int, int> > >  graph_adj_list = adj_list.create_graph(num_nodes, 0.5);
        unsigned int num_edges = adj_list.get_num_edges();
        unsigned  V = num_nodes;
        unsigned E = num_edges;

        Edge edge_array[E];
        int weights[E];
        unsigned int edge_counter = 0;
        for(unsigned int i = 0; i < V; i++)
            for(unsigned int j = 0; j < graph_adj_list[i].size(); j++)
            {
                edge_array[edge_counter] = make_pair(i, graph_adj_list[i][j].first);
                weights[edge_counter] = graph_adj_list[i][j].second;
                edge_counter++;
            }
        Graph g(edge_array, edge_array + E,weights, V);
        property_map < Graph, edge_weight_t >::type w = get(edge_weight, g);

        vector<int> d(V, (numeric_limits<int>::max)());
        vector< vector<int> > dist_map_boost(V, vector<int>(V));
        floyd_warshall_all_pairs_shortest_paths(g, dist_map_boost, distance_map(&d[0]));
        apsp_floid<int, int> floid(graph_adj_list);

        vector<vector<int> > dist_map = floid.get_distance_map();
        unsigned int accuracy_count = 0;
        for(unsigned int i = 0; i < V; i++)
            for(unsigned int j = 0; j < V; j++)
            {
                if(dist_map_boost[i][j] == dist_map[i][j])
                    accuracy_count++;
            }
        if(accuracy_count ==( V * V))
            accuracy++;

    }
    cout << "Floyd algorithm accuracy = " << (double(accuracy) / double(num_check)) * 100 << "%" << endl;
    return double(accuracy) / double(num_check);
}
double johnson_all_pairs_shortest_test(unsigned int num_check)
{
    int accuracy = 0;
    for(unsigned int step = 0; step < num_check; step++)
    {
        Adj_list adj_list;
        unsigned int num_nodes = 10;
        vector<vector<pair<int, int> > >  graph_adj_list = adj_list.create_graph(num_nodes, 0.5);
        unsigned int num_edges = adj_list.get_num_edges();
        unsigned  V = num_nodes;
        unsigned E = num_edges;

        Edge edge_array[E];
        int weights[E];
        unsigned int edge_counter = 0;
        for(unsigned int i = 0; i < V; i++)
            for(unsigned int j = 0; j < graph_adj_list[i].size(); j++)
            {
                edge_array[edge_counter] = make_pair(i, graph_adj_list[i][j].first);
                weights[edge_counter] = graph_adj_list[i][j].second;
                edge_counter++;
            }
        Graph g(edge_array, edge_array + E,weights, V);
        property_map < Graph, edge_weight_t >::type w = get(edge_weight, g);

        vector<int> d(V, (numeric_limits<int>::max)());
        vector< vector<int> > dist_map_boost(V, vector<int>(V));
        johnson_all_pairs_shortest_paths(g, dist_map_boost, distance_map(&d[0]));
        Johnson<int, int> johnson(graph_adj_list, V, E);
        bool res = johnson.do_johnson();
        vector<vector<int> > dist_map = johnson.get_real_distance_map();
        unsigned int accuracy_count = 0;
        for(unsigned int i = 0; i < V; i++)
            for(unsigned int j = 0; j < V; j++)
            {
                if(dist_map_boost[i][j] == dist_map[i][j])
                    accuracy_count++;
            }
        if(accuracy_count ==( V * V))
            accuracy++;

    }
    cout << "Johnson algorithm accuracy = " << (double(accuracy) / double(num_check)) * 100 << "%" << endl;
    return double(accuracy) / double(num_check);
}

double a_star_shortest_test(unsigned int num_check)
{
    unsigned int accuracy = 0;
    for(unsigned int step = 0; step < num_check; step++)
    {
        Adj_list adj_list;
        unsigned int num_nodes = 20;
        vector<coord> graph_map;
        vector<vector<pair<int, int> > >  graph_adj_list = adj_list.create_map_graph(num_nodes, 0.5, graph_map);
        unsigned int num_edges = adj_list.get_num_edges();
        unsigned  V = num_nodes;
        unsigned E = num_edges;

        Edge edge_array[E];
        int weights[E];
        unsigned int edge_counter = 0;
        for(unsigned int i = 0; i < V; i++)
            for(unsigned int j = 0; j < graph_adj_list[i].size(); j++)
            {
                edge_array[edge_counter] = make_pair(i, graph_adj_list[i][j].first);
                weights[edge_counter] = graph_adj_list[i][j].second;
                edge_counter++;
            }
        Graph g(edge_array, edge_array + E, weights, V);
        property_map < Graph, edge_weight_t >::type w = get(edge_weight, g);
        vector<vertex_descriptor> p(num_vertices(g));
        vector<int> d(num_vertices(g));
        vertex_descriptor s = vertex(0, g);
        dijkstra_shortest_paths(g, s,
                                predecessor_map(boost::make_iterator_property_map(p.begin(), get(boost::vertex_index, g))).
                                distance_map(boost::make_iterator_property_map(d.begin(), get(boost::vertex_index, g))));

        Johnson<int, int> johnson(graph_adj_list, num_nodes, num_edges);
        vector<int> dist1;
        vector< vector<int> > path;
        johnson.dijkstra(0, dist1, graph_adj_list, path);
        A_star<int, int> a_star(graph_adj_list, num_nodes, num_edges);
        a_star.set_ver_coord(graph_map);
        vector<int> path_a_star;

        int dist = a_star.do_a_star(0, V - 1, path_a_star);
        if(d[V - 1] == dist)
            accuracy++;;
    }
    cout << "a_star accuracy = " << (double(accuracy) / double(num_check)) * 100 << "%" << endl;
    return double(accuracy) / double(num_check);
}

#endif //BRIDGES_TESTS_SHORTEST_PATHS_H
