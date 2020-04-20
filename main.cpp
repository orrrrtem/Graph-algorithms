#include <iostream>
#include "graph.h"
#include "algorithms.h"
#include "benchmark_shortest_paths.h"
#include "reader.h"
#include <sstream>

using namespace std;


int main() {
    shortest_paths_combo();
    //a_star_shortest_test(500);
    //shortest_paths_combo();
    //floid_all_pairs_shortest_test(10);
/*
    vector<vector<pair<int, int> > >  edges(5);
    edges[0].push_back(make_pair(1, 6));
    edges[0].push_back(make_pair(3, 7));
    edges[1].push_back(make_pair(2, 5));
    edges[1].push_back(make_pair(3, -8));
    edges[1].push_back(make_pair( 4, -4));
    edges[2].push_back(make_pair(1, -2));
    edges[3].push_back(make_pair(2, -3));
    edges[3].push_back(make_pair( 4, 4));
    edges[4].push_back(make_pair( 0, 2));
    edges[4].push_back(make_pair( 2, 7));
    int num_nodes = 5;
    int num_edges = 10;
*/
/*
    Adj_list adj_list;
    unsigned int num_nodes = 10;
    vector<vector<pair<int, int> > >  edges = adj_list.create_graph(num_nodes, 0.5);
    adj_list.print_adj_list();
    unsigned int num_edges = adj_list.get_num_edges();

    vector<int> dist1, dist2;
    Johnson first(edges, num_nodes, num_edges);
    A_star second(edges, num_nodes, num_edges);
    vector<int> path;
    second.do_a_star(2, 0, path);
    first.do_johnson();
    first.print_results();


*/


    //vector<unsigned int> s_v = {0,1,2};
    //vector<unsigned int> d_v = {1, 2, 0};
    //vector<float> weights = {1,2,1};

//    vector<unsigned int> s_v;
//    vector<unsigned int> d_v;
//    vector<float> weights;

    //ifstream myfile("/Users/artemaroslankin/Downloads/inf-USAir97.mtx");

    //auto adj = adjacency_list_from_COO(s_v, d_v, weights,true);


    //apsp_floid sd(adj);

    //sd.print_result();
    //real_graph_becnhmark_shortest_paths<float>("/Users/artemaroslankin/Downloads/inf-USAir97.mtx", true);  //18 seconds
    real_graph_becnhmark_shortest_paths<float>("/Users/artemaroslankin/Downloads/inf-USAir97.mtx", floyd_worshall_method);
    real_graph_becnhmark_shortest_paths<float>("/Users/artemaroslankin/Downloads/inf-USAir97.mtx", johnson_method);


    return 0;
}
