//
// Created by Артем Аросланкин on 13.11.2019.
//

#ifndef BRIDGES_ALGORITHMS_H
#define BRIDGES_ALGORITHMS_H

#include "graph.h"



void print_bridge(int v, int to) {
    cout << "Bridge: (" << v << "," << to << ")" << endl;
}



class dfs_bridges {

    vector<bool> visited;
    vector<int> tin, low;
    int timer;





    void dfs(const CSRgraph &g, int v, int p = -1) {
        visited[v] = true;
        tin[v] = low[v] = timer++;

        pair<int, int> Nv = g.get_neighbors(v);
        for (int to_ = Nv.first; to_ < Nv.second; to_++)
            //for (int to : adj[v]) {
        {
            int to = g.cols[to_];
            if (to == p) continue;
            if (visited[to]) {
                low[v] = min(low[v], tin[to]);
            } else {
                dfs(g, to, v);
                low[v] = min(low[v], low[to]);
                if (low[to] > tin[v])
                    print_bridge(v, to);

            }
        }
    }

    void find_bridges(const CSRgraph &g) {
        timer = 0;
        visited.assign(g.num_vert, false);
        tin.assign(g.num_vert, -1);
        low.assign(g.num_vert, -1);
        for (int i = 0; i < g.num_vert; ++i) {
            if (!visited[i])
                dfs(g, i);
        }
    }
public:
    dfs_bridges(const CSRgraph & g)
    {
        cout << "Searching bridges with dfs method started" << endl;
        find_bridges(g);
    }
};




#endif //BRIDGES_ALGORITHMS_H
