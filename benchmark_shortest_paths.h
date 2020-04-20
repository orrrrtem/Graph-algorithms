//
// Created by Polina Smolnikova on 4/12/2020.
//

#ifndef BRIDGES_BENCHMARK_SHORTEST_PATHS_H
#define BRIDGES_BENCHMARK_SHORTEST_PATHS_H

#pragma once
#include "graph.h"
#include <fstream>
#include <tuple>
#include "johnson.h"
#include "FW_APSP.h"
#include "a_star.h"
#include "reader.h"

using namespace std;

void shortest_paths_combo()
{
    unsigned int start_num_ver = 0;
    unsigned int step = 100;
    vector<tuple<unsigned int, double, double> > results1;
    vector<tuple<unsigned int, double, double, double> > results2;
    // do Johnson and Floyd
    for(unsigned int num_ver = start_num_ver; num_ver < 200; num_ver+= step)
    {
        Adj_list adj_list;
        unsigned int num_nodes = num_ver;
        vector<vector<pair<int, int> > >  graph_adj_list = adj_list.create_graph(num_nodes, 0.003);
        unsigned int num_edges = adj_list.get_num_edges();
        Johnson<int, int> johnson(graph_adj_list, num_nodes);
        bool res;
        auto t1 = std::chrono::high_resolution_clock::now();
        res = johnson.do_johnson();
        auto t2 = std::chrono::high_resolution_clock::now();
        double time_johnson = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();

        t1 = std::chrono::high_resolution_clock::now();
        apsp_floid<int, int> floid(graph_adj_list);
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
/*
    std::ofstream myfile1;
    myfile1.open ("C:\\Users\\psmolnik\\Desktop\\johnson.xls");
    for(unsigned int i = 0; i < results1.size(); i++)
        myfile1 << get<0>(results1[i]) << " " << get<1>(results1[i]) << " " << get<2>(results1[i]) << "\n";
*/
    // do dijkstra and a_star
    for(unsigned int num_ver = start_num_ver; num_ver < 1000; num_ver+= step)
    {
        if (num_ver == 0)
            continue;
        Adj_list adj_list;
        unsigned int num_nodes = num_ver;
        vector<coord> graph_map;
        vector<vector<pair<int, int> > >  edges = adj_list.create_map_graph(num_nodes, 0.3, graph_map);
        unsigned int num_edges = adj_list.get_num_edges();
        Johnson<int, int> johnson(edges, num_nodes);

        vector<int> dist;
        vector< vector<int> > path;
        auto t1 = std::chrono::high_resolution_clock::now();
        johnson.dijkstra(0, dist, edges, path);
        auto t2 = std::chrono::high_resolution_clock::now();
        double time_dijkstra = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();

        A_star<int, int> a_star(edges, num_nodes, num_edges);
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

/*

    std::ofstream myfile2;
    myfile2.open ("C:\\Users\\psmolnik\\Desktop\\a_star.xls");
    for(unsigned int i = 0; i < results2.size(); i++)
        myfile2 << get<0>(results2[i]) << " " << get<1>(results2[i]) << " " << get<2>(results2[i])  << " " << get<3>(results2[i]) << "\n";
*/
}



enum  APSP_method {floyd_worshall_method, johnson_method };

template<class weight_type>
void real_graph_becnhmark_shortest_paths(const string& file_path, APSP_method method = floyd_worshall_method, bool to_display = false) {
    cout << "Real graph measure started" << endl;
    Timer time;
    time.Start();
    reader graph(file_path,true);
    time.Stop();
    std::cout << "read in\t "
              << time.Seconds()
              << " seconds from " <<file_path <<"\n";

    time.Start();
    vector<vector<pair<unsigned int, weight_type> > > g = adjacency_list_from_COO(graph.source, graph.dest, graph.weights);
    time.Stop();
    std::cout << "Adj matrix build took\t "
              << time.Seconds()
              << " seconds\n";

    cout << "|V|=" << g.size() << endl;

    time.Start();
    if (method == floyd_worshall_method) {
        apsp_floid<weight_type, unsigned int> sd(g);
        if (to_display == true) {
            sd.print_result();
        }
        time.Stop();
        cout << "Floyd worshall ";
    }
    if (method == johnson_method) {

        unsigned int num_nodes = g.size();
        Johnson<weight_type, unsigned int > johnson(g, num_nodes);
        time.Stop();
        cout << "Johnson ";
    }

    std::cout << "APSP:\t "
              << time.Seconds()
              << " seconds\n";


}


#endif //BRIDGES_BENCHMARK_SHORTEST_PATHS_H
