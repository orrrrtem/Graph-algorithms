#include <iostream>
#include "graph.h"
#include "algorithms.h"

using namespace std;


int main() {
    std::cout << "Hello, World!" << std::endl;
    vector<int> rows = {0,2,4,6,9,12,14,16,18};
    vector<int> cols = {1,2,0,3,0,3,1,2,4,3,5,7,4,6,5,7,4,6};
    CSRgraph g(rows,cols);
    pair<int, int> Ni = g.get_neighbors(1);
    cout << g.get_size_neighbors(1) << endl;
    for(int i_ = Ni.first; i_ < Ni.second; i_++ )
    {
        int i = g.cols[i_];
        cout << i << " ";
    }
    cout << endl;


    dfs_bridges sol1(g);

    return 0;
}