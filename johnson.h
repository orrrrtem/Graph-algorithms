#include "graph.h"
#include <queue>

class Johnson
{
private:
    vector<vector< vector<int> > >  shortest_paths;
    vector<vector<pair<int, int> > > edges;
    vector<vector<int>> modified_distance_map;
    vector<vector<int>> real_distance_map;
    unsigned int nodes_number = 0;
    unsigned int edges_number = 0;
    int inf = 0;
    bool negative_cycle = false;
public:
    Johnson(vector<vector<pair<int, int> > > edges_, unsigned int nodes_number_, unsigned int edges_number_ );
    bool do_johnson();
    bool bellman_ford(int ver, vector<int>& dist, const vector<vector<pair<int, int> > >& graph_edges,unsigned int nodes_num);
    void dijkstra(int ver, vector<int>& dist, const vector<vector<pair<int, int> > >& graph_edges, vector< vector<int> >& path);
    vector<vector<int>> get_modified_distance_map() const
    {
        return modified_distance_map;
    }
    vector<vector< vector<int> > > get_shortest_path() const
    {
        return shortest_paths;
    }

    vector<vector<int> > get_real_distance_map();
    void print_results();
};


Johnson::Johnson(vector<vector<pair<int, int> > > edges_, unsigned int nodes_number_, unsigned int edges_number_)
{
    edges = edges_;
    nodes_number = nodes_number_;
    edges_number = edges_number_;
    shortest_paths.resize(nodes_number);
    inf = std::numeric_limits<int>::max();
}

bool Johnson::do_johnson()
{
    vector<vector<pair<int, int> > > new_edges = edges;
    new_edges.resize(nodes_number + 1);
    for(unsigned int i = 0; i < nodes_number; ++i)
        new_edges[nodes_number].push_back(make_pair(i, 0));
    vector<int> h;
    if(! bellman_ford(nodes_number, h, new_edges, (nodes_number + 1) ))
    {
        cout << "there is negative cycle" << endl;
        negative_cycle = true;
        return false;
    }
    vector<vector<pair<int, int> > > modified_edges = edges;
    for(unsigned int i = 0; i < modified_edges.size() ; ++i)
        for(unsigned int j = 0; j < modified_edges[i].size(); ++j)
        {
            modified_edges[i][j].second = modified_edges[i][j].second + h[i] - h[modified_edges[i][j].first];
        }
    modified_distance_map.resize(nodes_number);
    for(unsigned int i = 0; i < nodes_number; ++i)
    {
        dijkstra(i, modified_distance_map[i], modified_edges, shortest_paths[i]);
    }
    return true;


}

void Johnson::dijkstra(int ver, vector<int>& dist,  const vector<vector<pair<int, int> > >& graph_edges, vector< vector<int> >& path)
{
    dist.resize(nodes_number);
    dist.assign(nodes_number, inf);
    dist[ver] = 0;
    vector<bool> visit (nodes_number, false);
    vector<int> p(nodes_number, -1);
    path.resize(nodes_number);
    auto comparator = [&](int a_ver, int b_ver){return (dist[a_ver] ) > (dist[b_ver] );};
    vector<int> ver_queue;
    ver_queue.push_back(ver);
    std::make_heap(ver_queue.begin(), ver_queue.end(), comparator);
    while(!ver_queue.empty())
    {
        int ver_for_visit = ver_queue.front();
        std::pop_heap (ver_queue.begin(),ver_queue.end(), comparator);
        ver_queue.pop_back();
        if(visit[ver_for_visit])
            continue;

        for(unsigned int j = 0; j < edges[ver_for_visit].size(); ++j)
        {
            int next_ver = edges[ver_for_visit][j].first;
            int weight = edges[ver_for_visit][j].second;
            if ((dist[ver_for_visit] + weight)  < dist[next_ver])
            {
                dist[next_ver] = dist[ver_for_visit] + weight;
                p[next_ver] = ver_for_visit;
                ver_queue.push_back(next_ver);
                std::push_heap (ver_queue.begin(),ver_queue.end(), comparator);
            }
        }
        visit[ver_for_visit] = true;
    }

    for(unsigned int i = 0; i < p.size(); ++i)
    {
        int j = i;
        while(1)
        {
            if ((j == ver) || (j == -1))
                break;
            path[i].push_back(p[j]);
            j = p[j];

        }
        reverse(path[i].begin(), path[i].end());
        path[i].push_back(i);
    }

}

bool Johnson::bellman_ford(int ver, vector<int>& dist, const vector<vector<pair<int, int> > >& graph_edges, unsigned int nodes_num)
{

    dist.resize(nodes_num);
    dist.assign(nodes_num, inf);
    dist[ver] = 0;
    int x;
//extra iteration for negative cycle check
    for(unsigned int k = 0; k < nodes_num ; ++k)
    {
        x = -1;
        for (unsigned int i = 0; i < nodes_num; ++i)
        {
            for (unsigned j = 0; j < graph_edges[i].size(); ++j)
            {
                if((dist[graph_edges[i][j].first] < inf) || (i == ver))
                {
                    if (dist[graph_edges[i][j].first] > (dist[i] + graph_edges[i][j].second))
                    {
                        dist[graph_edges[i][j].first] = std::max(-inf, dist[i] + graph_edges[i][j].second);
                        x = graph_edges[i][j].first;
                    }
                }
            }


        }

    }

    if (x == -1)
        return true;
    else
        return false;
}

vector<vector<int> > Johnson::get_real_distance_map()
{
    real_distance_map.resize(nodes_number);
    for(unsigned int i = 0; i < nodes_number; ++i)
    {
        real_distance_map[i].resize(nodes_number);
        real_distance_map[i].assign(nodes_number, 0);
        if(negative_cycle)
            continue;
        for(unsigned int j = 0; j < nodes_number; ++j)
        {
            for (int k = 0; k < shortest_paths[i][j].size() - 1; ++k )
            {
                if( modified_distance_map[i][j] == inf)
                {
                    real_distance_map[i][j] = inf;
                    break;
                }
                else {
                    for(unsigned int m = 0; m < edges[shortest_paths[i][j][k]].size() ; ++m)
                    {
                        if (edges[shortest_paths[i][j][k]][m].first == shortest_paths[i][j][k + 1])
                            real_distance_map[i][j] += edges[shortest_paths[i][j][k]][m].second;
                    }
                }
            }
        }

    }
    return real_distance_map;
}

void Johnson::print_results()
{
    get_real_distance_map();
    for (unsigned int i = 0 ; i < nodes_number; ++i) {
        for (unsigned int j = 0; j < nodes_number; ++j) {
            if( modified_distance_map[i][j] != inf) {
                cout << i << " " << j << " with modified distance = " << modified_distance_map[i][j] << "\t with real distance = " << real_distance_map[i][j] <<  "\t path: ";
                for (auto k:shortest_paths[i][j])
                {
                    cout << k << " ";
                }
                cout << endl;
            }
        }
    }
}
