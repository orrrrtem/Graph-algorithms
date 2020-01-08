//
// Created by Артем Аросланкин on 09.12.2019.
//

#ifndef BRIDGES_READER_H
#define BRIDGES_READER_H


#include "graph.h"

#include <fstream>
#include <iostream>
#include <string>
using namespace std;


class reader
{
public:
    vector<set<unsigned  int>> adj_list;
    vector<unsigned int> src;
    vector<unsigned int> dst;
    unsigned int maxID = 0;
    CSRgraph graph;




    reader(const string file_path )
    {
        ifstream file(file_path);
        //ifstream file("/Users/artemaroslankin/Documents/Progacpp/untitled1/input.txt");
        char symbol;
        unsigned int  src_, dst_;
        src.clear();
        dst.clear();
        int count = 0;
        adj_list.reserve(2000001);
        adj_list.resize(2000000);
        cout << "reserved" << endl;
        if(file.is_open()){

/*
            while(! file.eof() )
            {
                    string src__;
                    string dst__;
                    getline(file,src__, ' ');
                    getline(file,dst__, ' ');
                    src.push_back(src_);
                    dst.push_back(dst_);
                    if(maxID < src_) maxID = src_;
                    if(maxID < dst_) maxID = dst_;
            }
            */
            while(file >> src_ >> dst_) {
                //file >> src_;
                //cout << v2 << "\n";

                adj_list[src_].insert(dst_);
                adj_list[dst_].insert(src_);
                //src.push_back(src_);
               // file >> dst_;
               // dst.push_back(dst_);
                //cout <<  src.back() << " " << dst.back() << endl;

               //if(src_ > 20) break;
              count++;
              //cout << count;
            }
        }
        file.close();
        //cout << count << endl;
        //cout << " edge list read" << endl;
        //cout << src.size() << " ";
        //cout <<  src[0] << " " << src[10];

/*

        for(unsigned int i = 0; i < src.size(); i ++)
        {
            adj_list[src[i]].insert(dst[i]);
            adj_list[dst[i]].insert(src[i]);
        }
*/
        CSRgraph g(adj_list);
        graph = g;
    }

    CSRgraph get_graph() const
    {
        return graph;
    }

    vector<set<unsigned  int >> get_adj_list() const {
        return adj_list;
    }
};


#endif //BRIDGES_READER_H
