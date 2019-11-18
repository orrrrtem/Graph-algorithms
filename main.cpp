#include <iostream>
#include <unistd.h>
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

    auto t1 = std::chrono::high_resolution_clock::now();
    CSRgraph g1 = create_graph(1000, 0.001);
    auto t2 = std::chrono::high_resolution_clock::now();
    std::cout << "generating took "

    << std::chrono::duration_cast<std::chrono::milliseconds>(t2-t1).count()
            << " milliseconds\n";



    cout << g1.num_vert << " " << g1.num_edges << endl;
    //g1.print_CSR();
    t1 = std::chrono::high_resolution_clock::now();
    dfs_bridges solution1(g1);
    t2 = std::chrono::high_resolution_clock::now();
    std::cout << "generating took "
            << std::chrono::duration_cast<std::chrono::microseconds>(t2-t1).count()
            << " microseconds\n";


    vector<double> p_vec ;
    double start_p = 0.0001;
    for (int scale = 1; scale < 5;scale++)
    {

        for (int s = 1; s < 10; s++)
        {
            p_vec.push_back(start_p*s);
        }
        start_p = start_p * 10;
    }

    for(int n = 100; n < 10001; n=n*10)
    {

        for(int j = 0; j < p_vec.size(); j++)
        {
            CSRgraph g_inner = create_graph(n, p_vec[j]);

            t1 = std::chrono::high_resolution_clock::now();
            dfs_bridges solution1(g_inner);
            t2 = std::chrono::high_resolution_clock::now();
            std::cout << "n = " << n << ";\tp = " << p_vec[j] << "\ttime: "
                      << std::chrono::duration_cast<std::chrono::microseconds>(t2-t1).count()
                      << " microseconds\n";
        }
    }

    return 0;
}