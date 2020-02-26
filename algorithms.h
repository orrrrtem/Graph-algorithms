//
// Created by Артем Аросланкин on 13.11.2019.
//

#ifndef BRIDGES_ALGORITHMS_H
#define BRIDGES_ALGORITHMS_H

#include <limits.h>
#include "graph.h"

#include "sorts.h"


void print_bridge(int v, int to) {
    //cout << "Bridge: (" << v << "," << to << ")" << endl;
}



class dfs_bridges {

    vector<bool> visited;
    vector<int> tin, low;
    int timer;
    int bridges_counter = 0;
    vector<edge> one_bridges;


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
                    edge new_bridge = {unsigned(v), unsigned(to)};
                    one_bridges.push_back(new_bridge);
                    bridges_counter++;
                }

            }
        }
    }
public:
    vector<edge> get_answer() const
    {
        return one_bridges;
    }
    void find_bridges(const CSRgraph &g)
    {
        timer = 0;
        visited.assign(g.num_vert, false);
        tin.assign(g.num_vert, -1);
        low.assign(g.num_vert, -1);
        one_bridges.clear();
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
        //cout << " num_bridges " << bridges_counter << "\t";
    }
    dfs_bridges(){}
};


template<class weight_type>
class randomized_init
{
public:
    vector<bool> visited;
    unsigned int bridges_counter = 0;

    vector<weight_type> weights;
    vector<bool> is_weight_set_;
    const CSRgraph * g;
  //  static std::random_device rd;
//    static std::mt19937 gen(rd());

    //uniform_int_distribution<weight_type> probability;
   // std::uniform_int_distribution<weight_type> probability(0, std::numeric_limits<weight_type>::max());



    void set_weight_by_pos(unsigned int pos, weight_type weight)
    {
        weights[pos] = weight;
        is_weight_set_[pos] = true;
    }

    void set_random_weight_by_pos(unsigned int pos)
    {
        //weights[pos] = (rand() % (numeric_limits<weight_type>::max() ));//deleted + 1
        std::uniform_int_distribution<weight_type> probability(0, std::numeric_limits<weight_type>::max());
        weights[pos] = probability(gen);
        is_weight_set_[pos] = true;
    }


    void set_weight(unsigned int i, unsigned int j, weight_type weight)
    {
        auto neighbors_i = g->get_neighbors(i);


        unsigned int pos_j = lower_bound(g->cols.begin() + neighbors_i.first, g->cols.begin() + neighbors_i.second, j) - g->cols.begin();
        weights[pos_j] = weight;
        is_weight_set_[pos_j] = true;

    }

    weight_type get_weight_by_pos(unsigned int pos) const
    {
        return weights[pos];
    }


    bool is_weight_set(const unsigned int pos) const
    {
        return is_weight_set_[pos];
    }

public:
    randomized_init(const CSRgraph * g_){
        g = g_;

    }
    randomized_init(){}

};



//using namespace randomized_init<weight_type>;

template< class weight_type>
class randomized_bridges : public randomized_init<weight_type>
        {
    vector<edge> one_bridges;
public:
/*
    vector<bool> visited;
    unsigned int bridges_counter = 0;

    vector<weight_type> weights;
    vector<bool> is_weight_set_;
    const CSRgraph * g;
    uniform_int_distribution<unsigned int> probability;


    void set_weight_by_pos(unsigned int pos, weight_type weight)
    {
        weights[pos] = weight;
        is_weight_set_[pos] = true;
    }

    void set_random_weight_by_pos(unsigned int pos)
    {
        //weights[pos] = (rand() % (numeric_limits<weight_type>::max() ));//deleted + 1
        weights[pos] = probability(gen);
        is_weight_set_[pos] = true;
    }


    void set_weight(unsigned int i, unsigned int j, weight_type weight)
    {
        auto neighbors_i = g->get_neighbors(i);


        unsigned int pos_j = lower_bound(g->cols.begin() + neighbors_i.first, g->cols.begin() + neighbors_i.second, j) - g->cols.begin();
        weights[pos_j] = weight;
        is_weight_set_[pos_j] = true;

    }

    weight_type get_weight_by_pos(unsigned int pos) const
    {
        return weights[pos];
    }


    bool is_weight_set(const unsigned int pos) const
    {
        return is_weight_set_[pos];
    }
    */

    randomized_bridges(const CSRgraph * g_)
    {
        //cout << "Randimized searching bridges with dfs method started" << endl;
        this->g = g_;
        find_bridges();
        //cout << " num_bridges " << this->bridges_counter << "\t";
    }
    randomized_bridges(){}

    weight_type randomized_dfs( unsigned int v, unsigned int parent)
    {
        this->visited[v] = true;

        weight_type sum = 0;
        pair<unsigned int, unsigned int> Nv = this->g->get_neighbors(v);
        for (unsigned int to_ = Nv.first; to_ < Nv.second; to_++)
        {
            unsigned int to = this->g->cols[to_];
            if (to == parent) continue;
            if (this->visited[to] == false)
            {
                sum = sum ^ randomized_dfs(to, v);
            }
            else
            {
                if (this->is_weight_set(to_) == false)
                {
                    this->set_random_weight_by_pos(to_);
                    this->set_weight(to, v, this->get_weight_by_pos(to_));
                }

                sum = sum ^ ( this->get_weight_by_pos(to_) );
            }
        }
        if(sum == 0)
        {
            //print_bridge(v,parent);
            edge new_bridge = {v, parent};
            one_bridges.push_back(new_bridge);
            this->bridges_counter++;
        }
        return sum;
    }


    void randomized_dfs_init( unsigned int v)
    {
        this->visited[v] = true;
        weight_type sum = 0;
        pair<unsigned int, unsigned int> Nv = this->g->get_neighbors(v);
        for (unsigned int to_ = Nv.first; to_ < Nv.second; to_++)
        {
            auto to = this->g->cols[to_];
            if (this->visited[to] == false)
            {
                sum = sum ^ randomized_dfs(to, v);
            }
            else
            {
                if (this->is_weight_set(to_) == false)
                {
                    this->set_random_weight_by_pos(to_);
                    this->set_weight(to, v, this->get_weight_by_pos(to_));
                }

                sum = sum ^ ( this->get_weight_by_pos(to_) );
            }
        }
    }




    void find_bridges()
    {
        this->visited.assign(this->g->num_vert, false);
        this->weights.resize(this->g->num_edges);
        this->is_weight_set_.assign(this->g->num_edges, false);

        for (int i = 0; i < this->g->num_vert; ++i)
        {
            if (!(this->visited[i]))
                randomized_dfs_init(i);
        }


    }

    vector<edge> get_answer() const
    {
        return one_bridges;
    }



};



enum  sort_choice {std_sort, bucket_sort, radix_sort };


template< class weight_type>
class randomized_two_bridges: public randomized_init<weight_type>
        {
    vector<weight_edge<weight_type>> weights_for_component;
    sort_choice sort_type;
    vector<pair<edge, edge>> two_bridges;

    weight_type randomized_dfs( unsigned int v, unsigned int parent)
    {
        this->visited[v] = true;

        weight_type sum = 0;
        pair<unsigned int, unsigned int> Nv = this->g->get_neighbors(v);
        for (unsigned int to_ = Nv.first; to_ < Nv.second; to_++)
        {
            unsigned int to = this->g->cols[to_];
            if (to == parent) continue; //to skip setting weight for parent
            if (this->visited[to] == false)
            {
                sum = sum ^ this->randomized_dfs(to, v);
            }
            else
            {
                if (this->is_weight_set(to_) == false)
                {
                    this->set_random_weight_by_pos(to_);
                    this->set_weight(to, v, this->get_weight_by_pos(to_));
                    if(sum != 0)
                    {
                        weight_edge<weight_type> new_weight_edge = {to, v, this->get_weight_by_pos(to_)};
                        weights_for_component.push_back(new_weight_edge);
                    }
                }

                sum = sum ^ this->get_weight_by_pos(to_);
            }
        }

        if(sum!=0)
        {
            weight_edge<weight_type> new_weight_edge = {parent, v, sum};
            weights_for_component.push_back(new_weight_edge);
        }
        return sum;
    }


    void randomized_dfs_init( unsigned int v)
    {
        this->visited[v] = true;
        weight_type sum = 0;

        pair<unsigned int, unsigned int> Nv = this->g->get_neighbors(v);
        for (unsigned int to_ = Nv.first; to_ < Nv.second; to_++)
        {
            unsigned int to = this->g->cols[to_];

            if (this->visited[to] == false)
            {
                sum = sum ^ this->randomized_dfs(to, v);
            }
            else
            {
                if (this->is_weight_set(to_) == false)
                {
                    this->set_random_weight_by_pos(to_);
                    this->set_weight(to, v, this->get_weight_by_pos(to_));
                }

                sum = sum ^ this->get_weight_by_pos(to_);
            }
        }

        switch(sort_type)
        {
            case radix_sort:
            {
                unsigned int radix_size = 16; //available any power of 2, but less then size of weight type in bits
                radixsort(weights_for_component,radix_size);
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

        for(auto i = 0; i + 1 < weights_for_component.size(); i++)
        {
            if( weights_for_component[i].weight == weights_for_component[i+1].weight)
            {
                edge first = {weights_for_component[i].start_ver, weights_for_component[i].end_ver};
                edge second = {weights_for_component[i + 1].start_ver, weights_for_component[i + 1].end_ver};
                two_bridges.push_back(make_pair(first, second));
                this->bridges_counter++;
                for(auto j = i + 2; j < weights_for_component.size(); j++)
                {
                    if( weights_for_component[i].weight == weights_for_component[j+1].weight)
                    {
                        edge first = {weights_for_component[i].start_ver, weights_for_component[i].end_ver};
                        edge second = {weights_for_component[j].start_ver, weights_for_component[j].end_ver};
                        two_bridges.push_back(make_pair(first, second));
                        this->bridges_counter++;
                    }
                    else
                    {
                        break;
                    }
                }
            }
        }
    }


public:

    vector<pair<edge, edge>> get_answer() const
    {
        return two_bridges;
    }
    void find_bridges()
    {
        this->visited.assign(this->g->num_vert, false);
        this->weights.resize(this->g->num_edges);
        this->is_weight_set_.assign(this->g->num_edges, false);
        two_bridges.clear();
        for (int i = 0; i < this->g->num_vert; ++i)
        {
            if (!this->visited[i])
            {
                weights_for_component.clear();
                randomized_dfs_init(i);
            }
        }
    }


    randomized_two_bridges(const CSRgraph * g_, sort_choice sort_type)
    {
        //cout << "Randimized searching two-bridges with dfs method started" << endl;
        this->g = g_;
        this->sort_type = sort_type;
        find_bridges();
        //cout << " num two-bridges " << this->bridges_counter << "\t";
    }
    randomized_two_bridges(){}
};

#endif //BRIDGES_ALGORITHMS_H
