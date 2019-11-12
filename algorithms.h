//
// Created by Артем Аросланкин on 13.11.2019.
//

#ifndef BRIDGES_ALGORITHMS_H
#define BRIDGES_ALGORITHMS_H

#include "graph.h"

void finding_bridges_dfs_method_(CSRgraph & g, int u, int & iter, vector<int> & enter, vector<int> & ret, vector<bool> &used);

void finding_bridges_dfs_method(CSRgraph &g)
{
    int iter = 0;
    vector<int> enter(g.num_vert, 0);
    vector<int> ret(g.num_vert, 0);
    vector<bool> used(g.num_vert, false);
    int init_vert = 3;
    finding_bridges_dfs_method_(g, init_vert, iter, enter, ret, used);

}
void finding_bridges_dfs_method_(CSRgraph & g, int u, int & iter, vector<int> & enter, vector<int> & ret, vector<bool> &used)
{
    iter += 1;
    enter[u] = iter;
    ret[u] = iter;

    pair<int, int> Nu = g.get_neighbors(u);
    for(int i_ = Nu.first; i_ < Nu.second; i_++ )
    {
        int i = g.cols[i_];

        int to = i;
        if(to == -1)
        {
            continue;
        }
        if (used[to] == true)
        {
            ret[u] = min( ret[u], enter[to]);
        }
        else
        {
            finding_bridges_dfs_method_(g,to, iter, enter, ret, used);
            ret[u] = min( ret[u], enter[to]);
            if (ret[to] > enter[u])
            {
                cout << "bridge (" << u <<"," << to <<")" << endl;
            }
        }
    }
}
#endif //BRIDGES_ALGORITHMS_H
