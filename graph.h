//
// Created by Артем Аросланкин on 12.11.2019.
//

#ifndef BRIDGES_GRAPH_H
#define BRIDGES_GRAPH_H

#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

class CSRgraph
{
public:
    int num_vert;
    int num_edges;

    vector<int> rows;
    vector<int> cols;


    CSRgraph(){}

    CSRgraph(vector<int> rows, vector<int> cols)
    {
        this->rows = rows;
        this->cols = cols;
        num_edges = rows.size() - 1;
        num_vert = rows.back();
    }


    pair<int, int> get_neighbors(int i)
    {
        return make_pair(rows[i], rows[i+1]);
    }
    int get_size_neighbors(int i)
    {
        return (rows[i+1] - rows[i]);
    }
};
#endif //BRIDGES_GRAPH_H
