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
    unsigned int start_num_ver = 0;
    unsigned int step = 50;
    vector<pair<unsigned int, double> > results;
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
        if(res)
        {
            results.push_back(make_pair(num_ver, std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count()));
            cout << "number edges = " << num_edges << "number ver = " << results.back().first << " , time =  " << results.back().second << endl;
        }
        else
            num_ver -= step;
    }

    std::ofstream myfile;
    myfile.open ("C:\\Users\\psmolnik\\Desktop\\johnson.xls");
    for(unsigned int i = 0; i < results.size(); i++)
        myfile << results[i].first << " " << results[i].second << "\n";
}

#endif //BRIDGES_BENCHMARK_SHORTEST_PATHS_H
