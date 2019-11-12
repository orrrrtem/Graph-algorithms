#include <iostream>
#include "graph.h"

using namespace std;


int main() {
    std::cout << "Hello, World!" << std::endl;
    vector<int> rows = {0,1,3,4};
    vector<int> cols = {1,0,2,1};
    CSRgraph g(rows,cols);
    pair<int, int> Ni = g.get_neighbors(1);
    cout << g.get_size_neighbors(1) << endl;
    for(int i_ = Ni.first; i_ < Ni.second; i_++ )
    {
        int i = g.cols[i_];
        cout << i << " ";
    }


    return 0;
}