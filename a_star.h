//
// Created by psmolnik on 4/14/2020.
//

#ifndef BRIDGES_A_STAR_H
#define BRIDGES_A_STAR_H

#include "graph.h"
#include <queue>

class A_star
{
private:
    vector<vector<pair<int, int> > > edges;
    vector<coord> ver_coord;
    unsigned int nodes_number = 0;
    unsigned int edges_number = 0;
    int inf = 0;
    int calculate_H(int cur_ver, int end_ver);
public:
    A_star(vector<vector<pair<int, int> > > edges_, unsigned int nodes_number_, unsigned int edges_number_ );
    void do_a_star(int start_ver, int end_ver, const vector<vector<pair<int, int> > >& graph_edges, vector<int>& path);
    void set_ver_coord(vector<coord>& ver_coord_)
    {
        ver_coord = ver_coord_;
    }
};


A_star::A_star(vector<vector<pair<int, int> > > edges_, unsigned int nodes_number_, unsigned int edges_number_)
{
    edges = edges_;
    nodes_number = nodes_number_;
    edges_number = edges_number_;
    inf = std::numeric_limits<int>::max();
}

void A_star::do_a_star(int start_ver, int end_ver, const vector<vector<pair<int, int> > >& graph_edges, vector<int>& path)
{
    vector<int> dist;
    dist.resize(nodes_number);
    dist.assign(nodes_number, inf);
    dist[start_ver] = 0;

    vector<bool> visit (nodes_number, false);
    vector<int> p(nodes_number, -1);
    auto comparator = [&](int a_ver, int b_ver){return (dist[a_ver] + calculate_H(a_ver, end_ver)) > (dist[b_ver] + calculate_H(b_ver, end_ver));};
    std::priority_queue<int, vector<int>, decltype(comparator)> ver_queue(comparator);
    ver_queue.push(start_ver);
    while(!ver_queue.empty())
    {
        int ver_for_visit = ver_queue.top();
        ver_queue.pop();
        if(visit[ver_for_visit])
            continue;
        if(ver_for_visit == end_ver)
        {
            break;
        }

        for(unsigned int j = 0; j < graph_edges[ver_for_visit].size(); ++j)
        {
            int next_ver = graph_edges[ver_for_visit][j].first;
            int weight = graph_edges[ver_for_visit][j].second;
            if ((dist[ver_for_visit] + weight)  < dist[next_ver])
            {
                dist[next_ver] = dist[ver_for_visit] + weight;
                p[next_ver] = ver_for_visit;
                ver_queue.push((next_ver));
            }
        }
        visit[ver_for_visit] = true;
    }
    int i = end_ver;
    int j = i;
    while(1)
    {
        if ((j == start_ver) || (j == -1))
            break;
        path.push_back(p[j]);
        j = p[j];

    }
    reverse(path.begin(), path.end());
    path.push_back(i);

    cout << dist[end_ver] << endl;
    for(unsigned int i = 0; i < path.size(); ++i)
        cout << path[i] << " ";
    cout << endl;
}

int A_star::calculate_H(int cur_ver, int end_ver)
{
    if(ver_coord.empty())
        return 1;
    else
        return sqrt((ver_coord[cur_ver].x - ver_coord[end_ver].x) * (ver_coord[cur_ver].x - ver_coord[end_ver].x)
                    + (ver_coord[cur_ver].y - ver_coord[end_ver].y) * (ver_coord[cur_ver].y - ver_coord[end_ver].y));
}

#endif //BRIDGES_A_STAR_H
