//
// Created by Артем Аросланкин on 13.11.2019.
//

#ifndef BRIDGES_ALGORITHMS_H
#define BRIDGES_ALGORITHMS_H

#include <limits.h>
#include "graph.h"

#include "sorts.h"

void print_bridge(int v, int to) {
    cout << "Bridge: (" << v << "," << to << ")" << endl;
}



class dfs_bridges {

    vector<bool> visited;
    vector<int> tin, low;
    int timer;
    int bridges_counter = 0;





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
                {
                    print_bridge(v, to);
                    bridges_counter++;
                }

            }
        }
    }
public:
    void find_bridges(const CSRgraph &g)
    {
        timer = 0;
        visited.assign(g.num_vert, false);
        tin.assign(g.num_vert, -1);
        low.assign(g.num_vert, -1);
        for (int i = 0; i < g.num_vert; ++i)
        {
            if (!visited[i])
                dfs(g, i);
        }
    }


    dfs_bridges(const CSRgraph & g)
    {
        //cout << "Searching bridges with dfs method started" << endl;
        find_bridges(g);
        cout << " num_bridges " << bridges_counter << "\t";
    }
    dfs_bridges(){}
};




template< class weight_type>
class randomized_bridges {

    vector<bool> visited;
    //vector<int> tin;
    //int timer;
    int bridges_counter = 0;
    vector<weight_type> weights;
    const CSRgraph * g;


    void set_weight_by_pos(int pos, weight_type weight)
    {
        weights[pos] = weight;
    }

    void set_random_weight_by_pos(int pos)
    {
        weights[pos] = (rand() % (numeric_limits<weight_type>::max() )) + 1;
    }

    void set_weight(int i, int j, weight_type weight)
    {
        auto neighbors_i = g->get_neighbors(i);
        for(auto k = neighbors_i.first; k < neighbors_i.second; k++) //TODO binary search
        {
            if(g->cols[k] == j)
            {
                weights[k] = weight;
                break;
            }
        }
    }

    int get_weight_by_pos(int pos) const
    {
        return weights[pos];
    }






    int randomized_dfs( int v, int parent)
    {
        visited[v] = true;
        //tin[v] = timer++;
        weight_type sum = 0;
        pair<int, int> Nv = g->get_neighbors(v);
        for (int to_ = Nv.first; to_ < Nv.second; to_++)
        {
            int to = g->cols[to_];
            if (to == parent) continue;
            if (visited[to] == false)
            {
                sum = sum ^ randomized_dfs(to, v);
            }
            else
            {
                if (get_weight_by_pos(to_) == -1)
                {
                    set_random_weight_by_pos(to_);
                    set_weight(to, v, get_weight_by_pos(to_));
                }

                sum = sum ^ get_weight_by_pos(to_);
            }
        }
        if(sum == 0)
        {
            print_bridge(v,parent);
            bridges_counter++;
        }
        return sum;
    }


    void randomized_dfs_init( int v, int parent = -1)
    {
        visited[v] = true;
        //tin[v] = timer++;
        weight_type sum = 0;
        pair<int, int> Nv = g->get_neighbors(v);
        for (int to_ = Nv.first; to_ < Nv.second; to_++)
        {
            int to = g->cols[to_];
            if (to == parent) continue;
            if (visited[to] == false)
            {
                sum = sum ^ randomized_dfs(to, v);
            }
            else
            {
                if (get_weight_by_pos(to_) == -1)
                {
                    set_random_weight_by_pos(to_);
                    set_weight(to, v, get_weight_by_pos(to_));
                }

                sum = sum ^ get_weight_by_pos(to_);
            }
        }
    }







public:
    void find_bridges()
    {
        //timer = 0;
        //g = g_;
        visited.assign(g->num_vert, false);
        //tin.assign(g.num_vert, -1);
        //label_type random_number = rand() % INT_MAX;
        weights.resize(g->num_edges);
        fill(weights.begin(), weights.end(), -1);

        for (int i = 0; i < g->num_vert; ++i)
        {
            if (!visited[i])
                randomized_dfs_init(i);
        }


    }


    randomized_bridges(const CSRgraph * g_)
    {
        cout << "Randimized searching bridges with dfs method started" << endl;
        g = g_;
        find_bridges();
        cout << " num_bridges " << bridges_counter << "\t";
    }
    randomized_bridges(){}
};



enum  sort_choice {std_sort, bucket_sort, radix_sort };




template< class weight_type>
class randomized_two_bridges {

    vector<bool> visited;
    vector<pair <weight_type, pair<int,int> >> weights_for_component;

    int bridges_counter = 0;
    vector<weight_type> weights;
    const CSRgraph * g;
    sort_choice sort_type;




    void set_weight_by_pos(int pos, weight_type weight)
    {
        weights[pos] = weight;
    }

    void set_random_weight_by_pos(int pos)
    {
        weights[pos] = (rand() % (numeric_limits<weight_type>::max() )) + 1;
    }


    void print_two_bridge(int i, int j) const
    {
        cout <<"("<< weights_for_component[i].second.first << ", " << weights_for_component[i].second.second <<")  "<<
                "("<< weights_for_component[j].second.first << ", " << weights_for_component[j].second.second <<")"<<
                endl;
    }

    void set_weight(int i, int j, weight_type weight)
    {
        auto neighbors_i = g->get_neighbors(i);
        for(auto k = neighbors_i.first; k < neighbors_i.second; k++) //TODO binary search
        {
            if(g->cols[k] == j)
            {
                weights[k] = weight;
                break;
            }
        }
    }

    int get_weight_by_pos(int pos) const
    {
        return weights[pos];
    }






    int randomized_dfs( int v, int parent)
    {
        visited[v] = true;

        weight_type sum = 0;
        pair<int, int> Nv = g->get_neighbors(v);
        for (int to_ = Nv.first; to_ < Nv.second; to_++)
        {
            int to = g->cols[to_];
            if (to == parent) continue;
            if (visited[to] == false)
            {
                sum = sum ^ randomized_dfs(to, v);
            }
            else
            {
                if (get_weight_by_pos(to_) == -1)
                {
                    set_random_weight_by_pos(to_);
                    set_weight(to, v, get_weight_by_pos(to_));
                    weights_for_component.push_back(std::make_pair(get_weight_by_pos(to_), std::make_pair(to,v)));
                }

                sum = sum ^ get_weight_by_pos(to_);
            }
        }

        weights_for_component.push_back(std::make_pair(sum, std::make_pair(parent, v)));

        return sum;
    }


    void randomized_dfs_init( int v, int parent = -1)
    {
        visited[v] = true;

        weight_type sum = 0;
        pair<int, int> Nv = g->get_neighbors(v);
        for (int to_ = Nv.first; to_ < Nv.second; to_++)
        {
            int to = g->cols[to_];
            if (to == parent) continue;
            if (visited[to] == false)
            {
                sum = sum ^ randomized_dfs(to, v);
            }
            else
            {
                if (get_weight_by_pos(to_) == -1)
                {
                    set_random_weight_by_pos(to_);
                    set_weight(to, v, get_weight_by_pos(to_));
                }

                sum = sum ^ get_weight_by_pos(to_);
            }
        }

        switch(sort_type)
        {
            case radix_sort:
            {
                radixsort(weights_for_component);
                break;

            }
            case bucket_sort:
            {
                bucketSort(weights_for_component);
                //bucketTreesSort(weights_for_component);
                break;
            }

            case std_sort:
            {
                std::sort(weights_for_component.begin(), weights_for_component.end());
                break;
            }

        }

        for(int i = 0; i + 1 < weights_for_component.size(); i++)
        {
            if( weights_for_component[i].first == weights_for_component[i+1].first)
            {
                //print_two_bridge(i, i + 1);
                bridges_counter++;
                for(int j = i + 2; j < weights_for_component.size(); j++)
                {
                    if( weights_for_component[i].first == weights_for_component[j+1].first)
                    {
                        //print_two_bridge(i, j);
                        bridges_counter++;
                    }
                }
            }
        }


    }







public:
    void find_bridges()
    {
        visited.assign(g->num_vert, false);
        weights.resize(g->num_edges);
        fill(weights.begin(), weights.end(), -1);
        for (int i = 0; i < g->num_vert; ++i)
        {
            if (!visited[i])
            {
                weights_for_component.clear();
                //cout << weights_for_component.size() << endl;
                randomized_dfs_init(i);
            }
        }

    }


    randomized_two_bridges(const CSRgraph * g_, sort_choice sort_type)
    {
        //cout << "Randimized searching two-bridges with dfs method started" << endl;
        g = g_;
        this->sort_type = sort_type;
        find_bridges();
        cout << " num two-bridges " << bridges_counter << "\t";
    }
    randomized_two_bridges(){}
};

#endif //BRIDGES_ALGORITHMS_H
