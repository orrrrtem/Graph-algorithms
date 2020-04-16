//
// Created by Polina Smolnikova on 4/12/2020.
//

#ifndef BRIDGES_BENCHMARK_SHORTEST_PATHS_H
#define BRIDGES_BENCHMARK_SHORTEST_PATHS_H

#include "graph.h"
#include <fstream>
#include <tuple>


using namespace std;

void shortest_paths_combo()
{
    unsigned int start_num_ver = 1;
    unsigned int step = 50;
    vector<tuple<unsigned int, double, double> > results1;
    vector<tuple<unsigned int, double, double, double> > results2;
    // do Johnson and Floyd
    for(unsigned int num_ver = start_num_ver; num_ver < 200; num_ver+= step)
    {
        Adj_list adj_list;
        unsigned int num_nodes = num_ver;
        vector<vector<pair<int, int> > >  edges = adj_list.create_graph(num_nodes, 0.005);
        unsigned int num_edges = adj_list.get_num_edges();
        Johnson johnson(edges, num_nodes, num_edges);
        bool res;
        auto t1 = std::chrono::high_resolution_clock::now();
        res = johnson.do_johnson();
        auto t2 = std::chrono::high_resolution_clock::now();
        double time_johnson = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();

        t1 = std::chrono::high_resolution_clock::now();
        apsp_floid<int, int> floid(edges);
        t2 = std::chrono::high_resolution_clock::now();
        double time_floid = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();

        if(res)
        {
            results1.push_back(make_tuple(num_ver, time_johnson, time_floid));
            cout << "number edges = " << num_edges << "number ver = " << num_nodes << " , Johnson time =  " << time_johnson
                 << ", Floyd time " << time_floid <<endl;
        }
        else
            num_ver -= step;
    }
    // do dijkstra and a_star
    for(unsigned int num_ver = start_num_ver; num_ver < 400; num_ver+= step)
    {
        Adj_list adj_list;
        unsigned int num_nodes = num_ver;
        vector<coord> graph_map;
        vector<vector<pair<int, int> > >  edges = adj_list.create_map_graph(num_nodes, 0.5, graph_map);
        unsigned int num_edges = adj_list.get_num_edges();
        Johnson johnson(edges, num_nodes, num_edges);

        vector<int> dist;
        vector< vector<int> > path;
        auto t1 = std::chrono::high_resolution_clock::now();
        johnson.dijkstra(0, dist, edges, path);
        auto t2 = std::chrono::high_resolution_clock::now();
        double time_dijkstra = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();

        A_star a_star(edges, num_nodes, num_edges);
        int max_dist_index = 0;
        for(int k = 0; k < num_ver; ++k)
        {
            if((path[k].size() > path[max_dist_index].size()) && (dist[k] != std::numeric_limits<int>::max()))
                max_dist_index = k;
        }

        vector<int> path_a_star;
        t1 = std::chrono::high_resolution_clock::now();
        a_star.do_a_star(0, max_dist_index, path_a_star);
        t2 = std::chrono::high_resolution_clock::now();
        double time_a_star = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();

        a_star.set_ver_coord(graph_map);
        vector<int> path_a_star_h;
        t1 = std::chrono::high_resolution_clock::now();
        a_star.do_a_star(0, max_dist_index, path_a_star_h);
        t2 = std::chrono::high_resolution_clock::now();
        double time_a_star_h = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();
        results2.push_back(make_tuple(num_ver, time_dijkstra, time_a_star, time_a_star_h));

        cout << "number edges = " << num_edges << ", number ver = " << num_ver << " , time =  " << time_dijkstra << " " << time_a_star << " " << time_a_star_h << endl;

    }

    std::ofstream myfile1;
    myfile1.open ("C:\\Users\\psmolnik\\Desktop\\johnson.xls");
    for(unsigned int i = 0; i < results1.size(); i++)
        myfile1 << get<0>(results1[i]) << " " << get<1>(results1[i]) << " " << get<2>(results1[i]) << "\n";

    std::ofstream myfile2;
    myfile2.open ("C:\\Users\\psmolnik\\Desktop\\a_star.xls");
    for(unsigned int i = 0; i < results2.size(); i++)
        myfile2 << get<0>(results2[i]) << " " << get<1>(results2[i]) << " " << get<2>(results2[i])  << " " << get<3>(results2[i]) << "\n";
}

#endif //BRIDGES_BENCHMARK_SHORTEST_PATHS_H
