//
// Created by Артем Аросланкин on 06.12.2019.
//

#ifndef BRIDGES_TESTS_H
#define BRIDGES_TESTS_H


#include "algorithms.h"



using namespace std;


enum  verify_choice {one_bridge, two_bridge};


class verifier
{
    const CSRgraph * g;
    vector<pair<edge, edge>> two_bridges;
    vector<edge> one_bridges;
    vector<bool> visited;
    bool verified = true;
    verify_choice verify_type;
    vector<edge> deleted_edges;
    unsigned number_vertices_in_component = 0;
    unsigned int miss_count = 0;
    double precision = 0.0;
    int number_components = 0;
public:
    verifier(const CSRgraph *g_,
             const vector<pair<edge, edge>> &two_bridges_,
             verify_choice verify_type_)
    {
        g = g_;
        this->visited.assign(this->g->num_vert, false);
        two_bridges = two_bridges_;
        verify_type = verify_type_;

        //cout << "check start";
        switch(verify_type)
        {
            case two_bridge:
            {
                verify_two_bridges();
                break;
            }
            case one_bridge:
            {
                verify_one_bridges();
                break;
            }
        }
    }



    verifier(const CSRgraph *g_,
             const vector<edge> one_bridges_,
             verify_choice verify_type_)
    {
        g = g_;
        this->visited.assign(this->g->num_vert, false);
        one_bridges = one_bridges_;
        verify_type = verify_type_;

        //cout << "check start";
        switch(verify_type)
        {
            case two_bridge:
            {
                verify_two_bridges();
                break;
            }
            case one_bridge:
            {
                verify_one_bridges();
                break;
            }
        }
    }


    void verify_one_bridges()
    {
        if(one_bridges.size() == 0)
        {
            precision = 1.0;
            return;
        }


        for(auto i: one_bridges)
        {
            //cout << "check" << endl << endl;
            number_vertices_in_component = 0;
            deleted_edges.clear();

            dfs_init(i.start_ver);

            unsigned int component_size_before_deleting = number_vertices_in_component;
           // cout << component_size_before_deleting << " \n";
            number_vertices_in_component = 0;
            deleted_edges.clear();
            deleted_edges.push_back(i);
            dfs_init(i.start_ver);

            if(component_size_before_deleting == number_vertices_in_component)
            {
                miss_count++;
                verified = false;
                continue;
            }
        }

        precision = 1 - ((double)miss_count / (double)one_bridges.size());


    }

    void verify_two_bridges()
    {
        if(two_bridges.size() == 0)
        {
            precision = 1.0;
            return;
        }

        for(auto i: two_bridges)
        {
            number_vertices_in_component = 0;
            deleted_edges.clear();

            dfs_init(i.first.start_ver);

            unsigned int component_size_before_deleting = number_vertices_in_component;
            number_vertices_in_component = 0;
            deleted_edges.clear();
            deleted_edges.push_back(i.first);
            dfs_init(i.first.start_ver);

            if(component_size_before_deleting != number_vertices_in_component) // one bridge
            {
                miss_count++;
                verified = false;
                continue;
            }

            number_vertices_in_component = 0;
            deleted_edges.clear();
            deleted_edges.push_back(i.second);
            dfs_init(i.second.start_ver);

            if(component_size_before_deleting != number_vertices_in_component) // one bridge
            {
                miss_count++;
                verified = false;
                continue;
            }

            number_vertices_in_component = 0;
            deleted_edges.clear();
            deleted_edges.push_back(i.first);
            deleted_edges.push_back(i.second);
            dfs_init(i.second.start_ver);

            if(component_size_before_deleting == number_vertices_in_component)
            {
                miss_count++;
                verified = false;
                continue;
            }
        }
        precision = 1 - ((double)miss_count / (double)two_bridges.size());

    }

    bool is_verifed() const {
        return verified;
    }


    void randomized_dfs( unsigned int v, unsigned int parent)
    {
        this->visited[v] = true;
        number_vertices_in_component++;
        pair<unsigned int, unsigned int> Nv = this->g->get_neighbors(v);

        for (unsigned int to_ = Nv.first; to_ < Nv.second; to_++)
        {
            unsigned int to = this->g->cols[to_];

            if(to == parent)
                continue;

            bool is_deleted = false;
            for (unsigned int i = 0; i < deleted_edges.size(); i++)
            {
                if (create_edge(to, v) == deleted_edges[i] || create_edge(v, to) == deleted_edges[i]) {
                    is_deleted = true;
                    break;
                }
            }

            if (is_deleted == true || this->visited[to] == true) {
                continue;
            }
                //cout << v << " " << to << endl;
            else {
                randomized_dfs(to, v);
            }
        }
    }


    void dfs_init( unsigned int v)
    {
        this->visited.assign(this->g->num_vert, false);
        this->visited[v] = true;
        number_vertices_in_component++;

        pair<unsigned int, unsigned int> Nv = this->g->get_neighbors(v);

        for (unsigned int to_ = Nv.first; to_ < Nv.second; to_++)
        {
            unsigned int to = this->g->cols[to_];


            bool is_deleted = false;
            for (unsigned int i = 0; i < deleted_edges.size(); i++) {
                if (create_edge(to, v) == deleted_edges[i] || create_edge(v, to) == deleted_edges[i]) {
                    is_deleted = true;
                    break;
                }
            }

            if (is_deleted == true || this->visited[to] == true) {
                continue;
            }
            else {
                randomized_dfs(to, v);
            }
        }
    }

/*
    void verify_two_bridges()
    {
        if(two_bridges.size() == 0)
        {
            precision = 1.0;
            return;
        }

        for(auto i: two_bridges)
        {
            number_components = 0;
            number_vertices_in_component = 0;
            deleted_edges.clear();

            dfs_init(i.first.start_ver);

            unsigned int component_size_before_deleting = number_components ;
            std::cout << "number " <<  number_components << std::endl;
            number_components = 0;
            number_vertices_in_component = 0;
            deleted_edges.clear();
            deleted_edges.push_back(i.first);
            dfs_init(i.first.start_ver);
            std::cout << number_components << std::endl;
            if(component_size_before_deleting != number_components) // one bridge
            {
                miss_count++;
                verified = false;
                continue;
            }
            number_components = 0;
            number_vertices_in_component = 0;
            deleted_edges.clear();
            deleted_edges.push_back(i.second);
            dfs_init(i.second.start_ver);
            if(component_size_before_deleting != number_components) // one bridge
            {
                miss_count++;
                verified = false;
                continue;
            }
            number_components = 0;
            number_vertices_in_component = 0;
            deleted_edges.clear();
            deleted_edges.push_back(i.first);
            deleted_edges.push_back(i.second);
            dfs_init(i.second.start_ver);
            std::cout << number_components << std::endl;
            if(component_size_before_deleting == number_components)
            {
                miss_count++;
                verified = false;
                continue;
            }
        }
        precision = 1 - ((double)miss_count / (double)two_bridges.size());

    }
    bool is_verifed() const {
        return verified;
    }


    void randomized_dfs( unsigned int v)
    {
        this->visited[v] = true;
        pair<unsigned int, unsigned int> Nv = this->g->get_neighbors(v);

        for (unsigned int to_ = Nv.first; to_ < Nv.second; to_++)
        {
            unsigned int to = this->g->cols[to_];

            bool is_deleted = false;
            for (unsigned int i = 0; i < deleted_edges.size(); i++)
            {
                if (create_edge(to, v) == deleted_edges[i] || create_edge(v, to) == deleted_edges[i])
                {
                    is_deleted = true;
                    break;
                }
            }

            if (is_deleted == true || this->visited[to] == true)
            {
                continue;
            }
            else {
                randomized_dfs(to);
            }
        }
    }


    void dfs_init( unsigned int v)
    {
        this->visited.assign(this->g->num_vert, false);
       // visited[v] = true;
        for (int i = 0; i < this->g->num_vert; ++i)
        {
            if (!visited[i])
            {
                randomized_dfs(i);
                number_components++;
            }
        }

    }
*/
    double get_precision() const {
        return precision;
    }
    unsigned int get_miss_count() const
    {
        return miss_count;
    }
};



#endif //BRIDGES_TESTS_H
