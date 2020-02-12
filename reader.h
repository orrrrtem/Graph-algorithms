//
// Created by Артем Аросланкин on 09.12.2019.
//

#ifndef BRIDGES_READER_H
#define BRIDGES_READER_H


#include "graph.h"

#include <fstream>
#include <iostream>
#include <string>
#include <sstream>

using namespace std;


class reader
{
public:
    vector<set<unsigned  int>> adj_list;
    unsigned int maxID = 0;
    CSRgraph graph;




    reader(const string& filename )
    {
        vector<unsigned int> source;
        vector<unsigned int> dest;
        FILE *fp;
        char str[256];
        fp = fopen(filename.c_str(), "r");
        if (fp == NULL){
            exit(1);
        }
        int total = 0;
        while (fgets(str, 256, fp) != NULL )
        {
            string str2(str);
            istringstream str_stream(str2);
            string number;
            getline(str_stream, number, ' ');
            unsigned int src_ = stol(number);
            getline(str_stream, number, ' ');
            unsigned int dst_ = stol(number);
            source.push_back(src_);
            dest.push_back(dst_);
            total++;
        }
        fclose(fp);

        maxID =0;
        for(unsigned int i = 0; i < source.size(); ++i) {
            if(source[i] > maxID) maxID = source[i];
            if(dest[i] > maxID) maxID = dest[i];
        }
        adj_list.clear();
        adj_list.resize(maxID + 1);

        for(unsigned int i = 0; i < source.size(); ++i) {
            adj_list[source[i]].insert(dest[i]);
            adj_list[dest[i]].insert(source[i]);
        }


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
