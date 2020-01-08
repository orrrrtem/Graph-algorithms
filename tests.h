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
    vector<pair<pair  <unsigned int, unsigned int>, pair  <unsigned int, unsigned int> > > two_bridges;
    vector<pair<unsigned int, unsigned int>> one_bridges;
    vector<bool> visited;
    bool verified = true;
    verify_choice verify_type;
    vector<pair<unsigned int, unsigned int>> deleted_edges;
    unsigned number_vertices_in_component = 0;
    unsigned int miss_count = 0;
    double precision = 0.0;
public:
    verifier(const CSRgraph *g_,
            const vector<
                         pair
                              <pair  <unsigned int, unsigned int>, pair  <unsigned int, unsigned int>
                              >
                        >
                            &two_bridges_
                            ,verify_choice verify_type_)
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
             const vector<pair<unsigned int, unsigned int>> one_bridges_
            ,verify_choice verify_type_)
    {
        g = g_;
        this->visited.assign(this->g->num_vert, false);
        one_bridges = one_bridges_;
        verify_type = verify_type_;

        cout << "check start";
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

            dfs_init(i.first);

            unsigned int component_size_before_deleting = number_vertices_in_component;
           // cout << component_size_before_deleting << " \n";
            number_vertices_in_component = 0;
            deleted_edges.clear();
            deleted_edges.push_back(i);
            dfs_init(i.first);

            if(component_size_before_deleting == number_vertices_in_component)
            {
                //cout << "first check" << endl;
                //cout << component_size_before_deleting << " " << number_vertices_in_component << endl;
                //cout << i.first.first << " " << i.first.second << " " << i.second.first << " " << i.second.second << endl;
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
            //cout << "check" << endl << endl;
            number_vertices_in_component = 0;
            deleted_edges.clear();

            dfs_init(i.first.first);

            unsigned int component_size_before_deleting = number_vertices_in_component;
            number_vertices_in_component = 0;
            deleted_edges.clear();
            deleted_edges.push_back(i.first);
            dfs_init(i.first.first);

            if(component_size_before_deleting != number_vertices_in_component) // one bridge
            {
                //cout << "first check" << endl;
                //cout << component_size_before_deleting << " " << number_vertices_in_component << endl;
                //cout << i.first.first << " " << i.first.second << " " << i.second.first << " " << i.second.second << endl;
                miss_count++;
                verified = false;
                continue;
            }

            number_vertices_in_component = 0;
            deleted_edges.clear();
            deleted_edges.push_back(i.second);
            dfs_init(i.second.first);

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
            dfs_init(i.second.first);

            if(component_size_before_deleting == number_vertices_in_component)
            {
                miss_count++;
                //cout << component_size_before_deleting << " \n";

                //cout << i.first.first << "   " <<g->get_size_neighbors(i.first.first) << endl;
                //cout << i.first.second << "   " <<g->get_size_neighbors(i.first.second) << endl;
               // cout << i.second.first << "   " <<g->get_size_neighbors(i.second.first) << endl;
                //cout << i.second.second << "   " <<g->get_size_neighbors(i.second.second) << endl;
               // exit(1);
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
                if (make_pair(to, v) == deleted_edges[i] || make_pair(v, to) == deleted_edges[i]) {
                    //cout << to <<  " " << v << " " << deleted_edges[i].first << " " << deleted_edges[i].second << endl;
                    //cout << " deleted";
                    //if (deleted_edges.size() == 2) cout << " deleted path" << endl;
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
                if (make_pair(to, v) == deleted_edges[i] || make_pair(v, to) == deleted_edges[i]) {
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

    double get_precision() const {
        return precision;
    }
    unsigned int get_miss_count() const
    {
        return miss_count;
    }
};



#endif //BRIDGES_TESTS_H
