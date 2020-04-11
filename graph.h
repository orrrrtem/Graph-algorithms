//
// Created by Артем Аросланкин on 12.11.2019.
//

#ifndef BRIDGES_GRAPH_H
#define BRIDGES_GRAPH_H

#include <iostream>
#include <vector>
#include <algorithm>
#include <cinttypes>
#include <random>
#include <chrono>
#include <set>

using namespace std;

static std::random_device rd;
static std::mt19937 gen(rd());



struct edge
{
    unsigned int start_ver = 0;
    unsigned int end_ver = 0;
};

bool operator == (const edge& one, const edge& two)
{
    if ((one.start_ver == two.start_ver) && (one.end_ver == two.end_ver))
        return true;
    else
        return false;
}

edge create_edge(unsigned int start_ver_, unsigned int end_ver_)
{
    edge new_edge = {start_ver_, end_ver_};
    return new_edge;
}

template<typename weight_type>
struct weight_edge
{
    unsigned int start_ver = 0;
    unsigned int end_ver = 0;
    weight_type weight = 0;
};

template<typename weight_type>
bool operator < (const weight_edge<weight_type>& one, const weight_edge<weight_type>& two)
{
    if (one.weight < two.weight)
        return true;
    else
        return false;
}

class Adj_list
{
private:
    vector<vector<pair<int, int> > >  adj_list;
    unsigned int num_edges = 0;
    bool check_connection(unsigned int start_ver, unsigned int end_ver)
    {
        if(!adj_list[start_ver].empty())
        {
            for(unsigned int i = 0; i < adj_list[start_ver].size(); i++)
            {
                if(adj_list[start_ver][i].first == end_ver)
                    return true;
            }
        }
        else
            return false;
        return false;
    }

public:
    vector<vector<pair<int, int> > > get_adj_list() const
    {
        return adj_list;
    }
    unsigned int get_num_edges() const
    {
        return num_edges;
    }
    void print_adj_list() const
    {
        for(unsigned int i = 0 ; i < adj_list.size(); i++)
        {
            for(unsigned int j = 0; j < adj_list[i].size(); j++)
                cout << i << " " << adj_list[i][j].first << " weight: " << adj_list[i][j].second << endl;
        }
    }
    vector<vector<pair<int, int> > > create_graph(unsigned int n, double p_in)
    {
        if(n < 0)
            exit(-1);
        if(p_in < 0 || p_in > 1)
            exit(-1);
        adj_list.resize(n);

        //random_device generator;
        bernoulli_distribution pd(p_in);
        for(unsigned int i = 0; i < n; i++)
        {
            for (unsigned int j = 0; j < n; j++)
            {
                if(i == j)
                    continue;
                if (pd(gen) && (!check_connection(j, i)))
                {
                    adj_list[i].push_back(make_pair(j, ((gen() % 100))));
                    num_edges++;
                }
            }
        }

        return adj_list;

    }
};
class CSRgraph
{
public:
    unsigned int num_vert;
    unsigned int num_edges;

    vector<unsigned int> rows;
    vector<unsigned int> cols;
    vector<unsigned int> weight;


    CSRgraph(){}

    CSRgraph(vector<unsigned int> rows, vector<unsigned int> cols)
    {
        this->rows = rows;
        this->cols = cols;
        num_vert = rows.size() - 1;
        num_edges = rows.back();

        cout << "|V|=" <<num_vert << "  |E|=" << num_edges << endl;
    }

    CSRgraph(const vector<set<unsigned int>>& adj_list)
    {
        vector<unsigned int> rows(adj_list.size()+1, 0);
        vector<unsigned int> cols;
        unsigned int prefix_sum = 0;
        for(unsigned int i = 0; i < adj_list.size(); i ++)
        {
            prefix_sum += adj_list[i].size();
            rows[i+1] = prefix_sum;

            for (auto j: adj_list[i])
            {
                cols.push_back(j);
            }
        }
        this->rows = rows;
        this->cols = cols;
        num_vert = rows.size() - 1;
        num_edges = rows.back();
        //cout << "|V|=" <<num_vert << "  |E|=" << num_edges << endl;
    }

    unsigned int get_num_vert() const {
        return num_vert;
    }

    unsigned int get_num_edges() const {
        return num_edges;
    }

    pair<unsigned int, unsigned int> get_neighbors(unsigned int i) const
    {
        return make_pair(rows[i], rows[i+1]);
    }
    unsigned int get_size_neighbors(unsigned int i) const
    {
        return (rows[i+1] - rows[i]);
    }

    void print_CSR() const
    {
        for(unsigned int i = 0; i < num_vert; i++)
        {
            cout << "Vert " << i <<": ";
            auto Ni = get_neighbors(i);
            for(unsigned int j_ = Ni.first; j_ < Ni.second; j_++)
            {
                unsigned int j = cols[j_];
                cout << j << " ";
            }
            cout << endl;
        }
    }
};





//creating graph with probability of edge creating p_in. Edge's wight characteristics are math expection and dispersion.
CSRgraph create_graph(unsigned int n, double p_in)
{
    if(n < 0)
        exit(-1);
    if(p_in < 0 || p_in > 1)
        exit(-1);
    vector<set<unsigned int>> adj_list(n);

    //random_device generator;
    bernoulli_distribution pd(p_in);

    //std::poisson_distribution<int> pd((double)n*p_in);
    for(unsigned int i = 0; i < n; i++)
    {
        for (unsigned int j = i+1; j < n; j++)
        {
            if (pd(gen))
            //if((rand() % n) < (p_in*n))
            {
                adj_list[i].insert(j);
                adj_list[j].insert(i);
            }
        }
    }

     CSRgraph graph(adj_list);

    return graph;

}

//creating block structure graph , where each block is subgraph from create_graph function.
// With addition of p_out - probability of edge between clusters and their weight characteristics
/*
vector<vector<double>> create_block_graph(int num_clusters, int num_members,  double p_in, double p_out, double mu_weight_in, double dispersion_of_weight_in, double mu_weight_out, double dispersion_of_weight_out){
    if(num_clusters < 0)
        exit(-1);
    if(num_members < 0)
        exit(-1);
    if(p_out < 0 || p_out > 1)
        exit(-1);

    vector<vector<double>> graph(num_clusters * num_members, vector<double> (num_clusters*num_members, 0));
    vector<vector<double>> tmp;

    random_device generator;
    bernoulli_distribution distribution_of_p_out(p_out);
    normal_distribution<double> distribution_of_weight_out(mu_weight_out, dispersion_of_weight_out);

    for(int k=0; k < num_clusters; k++) {
        //block planting
        tmp = create_graph(num_members, p_in, mu_weight_in, dispersion_of_weight_in);
        for(int i = 0; i < tmp.size(); i++)
            for( int j = 0; j < tmp[i].size(); j++)
                graph[k * num_members + i][k * num_members + j] = tmp[i][j];


        //addition of external edges
        for(int i = k * num_members; i < tmp.size() + k * num_members; i++)
            for(int j = (k+1) * num_members; j < graph.size(); j++)
                if (distribution_of_p_out(generator)){
                    int count = 0;
                    while(graph[i][j]<=0 || count < 100) {
                        graph[i][j] = distribution_of_weight_out(generator);
                        count++;
                    }
                    if(graph[i][j]<=0)
                        graph[i][j] = abs(distribution_of_weight_out(generator));
                    graph[j][i]=graph[i][j];
                }
    }

    return graph;
}
*/



class Timer {
public:
    Timer() {}

    void Start() {
        elapsed_time_ = start_time_ = std::chrono::high_resolution_clock::now();
    }

    void Stop() {
        elapsed_time_ = std::chrono::high_resolution_clock::now();
    }

    double Seconds() const {
        return std::chrono::duration_cast<std::chrono::duration<double>>(elapsed_time_ - start_time_).count();
    }

    double Millisecs() const {
        return std::chrono::duration_cast<std::chrono::duration<double, std::milli>>(elapsed_time_ - start_time_).count();
    }

    double Microsecs() const {
        return std::chrono::duration_cast<std::chrono::duration<double, std::micro>>(elapsed_time_ - start_time_).count();
    }

private:
    std::chrono::high_resolution_clock::time_point start_time_, elapsed_time_;
};

// Times op's execution using the timer t
#define TIME_OP(t, op) { t.Start(); (op); t.Stop(); }










#endif //BRIDGES_GRAPH_H
