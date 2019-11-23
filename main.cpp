#include <iostream>
#include <unistd.h>
#include "graph.h"
#include "algorithms.h"
#include "benchmark.h"


using namespace std;


int main() {

    //vector<int> rows = {0,2,4,6,9,12,14,16,18,19,20};
    //vector<int> cols = {1,2,0,3,0,3,1,2,4,3,5,7,4,6,5,7,4,6,9,8};
    vector<int> rows = {0,3,6,10,14,19,22,25,28, 31, 33 , 35, 38};
    vector<int> cols = {1,2,3,0,2,3,0,1,3,4,0,1,2,4,2,3,5,6,7,4,6,7,4,5,7,4,5,6, 9, 10, 11, 8,11,8, 11,8,9,10};
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
    cout << endl;
    //randomized_two_bridges<short > solution1(&g, bucket_sort);




/*
    auto t1 = std::chrono::high_resolution_clock::now();
    CSRgraph g1 = create_graph(1000, 0.001);
    auto t2 = std::chrono::high_resolution_clock::now();
    std::cout << "generating took "

    << std::chrono::duration_cast<std::chrono::milliseconds>(t2-t1).count()
            << " milliseconds\n";



    cout << g1.num_vert << " " << g1.num_edges << endl;
    //g1.print_CSR();
    t1 = std::chrono::high_resolution_clock::now();
    dfs_bridges solution2(g1);
    t2 = std::chrono::high_resolution_clock::now();
    std::cout << "generating took "
            << std::chrono::duration_cast<std::chrono::microseconds>(t2-t1).count()
            << " microseconds\n";
*/


    randomized_two_bridges_combo(bucket_sort);


    return 0;
}