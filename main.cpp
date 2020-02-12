#include <iostream>
#include <unistd.h>
#include "graph.h"
#include "algorithms.h"
#include "benchmark.h"
#include "reader.h"
#include <sstream>

//#include "sorts.h"

using namespace std;


int main() {
    /*
    CSRgraph g_inner = create_graph(10000, 0.04);

    cout << "|V| = " << g_inner.get_num_vert() << "\t |E| = " << g_inner.get_num_edges() << endl;

    auto t1 = std::chrono::high_resolution_clock::now();
    randomized_two_bridges<unsigned long long> s1(&g_inner,std_sort);
    auto t2 = std::chrono::high_resolution_clock::now();
    std::cout << "Std sort:n = " << 10000 << ";\tp = " << 0.005 << "\ttime: "
              << std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count()
              << " microseconds\n";
    verifier test1(&g_inner,s1.get_answer(), two_bridge);
    cout << "Precision is: " << test1.get_precision() <<"\t num missings" << test1.get_miss_count() << endl;


    t1 = std::chrono::high_resolution_clock::now();
    randomized_two_bridges<unsigned long long> solution3(&g_inner,bucket_sort);
    t2 = std::chrono::high_resolution_clock::now();
    std::cout << " Bucket sort:    n = "  << ";\tp = "  << "\ttime: "
              << std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count()
              << " microseconds\n";

    verifier test3(&g_inner,solution3.get_answer(), two_bridge);
    cout << "Precision is: " << test3.get_precision()  << "\t num missings" << test3.get_miss_count() << endl;

    t1 = std::chrono::high_resolution_clock::now();
    randomized_two_bridges<unsigned long long> solution2(&g_inner,radix_sort);
    t2 = std::chrono::high_resolution_clock::now();
    std::cout << "Radix sort:n = " << ";\tp = "  << "\ttime: "
              << std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count()
              << " microseconds\n";


    verifier test2(&g_inner,solution2.get_answer(), two_bridge);
    cout << "Precision is: " << test2.get_precision() <<"\t num missings" << test2.get_miss_count() << endl;
    cout << endl;

*/

    real_graph_becnhmark<uint64_t>("/Users/artemaroslankin/Downloads/soc-loc-brightkite.edges");
    real_graph_becnhmark<uint64_t>("/Users/artemaroslankin/Downloads/web-NotreDame.edges");

    real_graph_becnhmark<uint64_t>("/Users/artemaroslankin/Downloads/socfb-Dartmouth6.mtx");

    real_graph_becnhmark<uint64_t>("/Users/artemaroslankin/Downloads/cit-DBLP.edges");
    real_graph_becnhmark<uint64_t>("/Users/artemaroslankin/Downloads/road-luxembourg-osm.mtx");




    reader USA("/Users/artemaroslankin/Downloads/cit-DBLP.edges");


    vector<unsigned int> rows = {0,3,6,10,14,19,22,25,28, 31, 33 , 35, 38,39,40,41,42,42};
    vector<unsigned int> cols = {1,2,3,0,2,3,0,1,3,4,0,1,2,4,2,3,5,6,7,4,6,7,4,5,7,4,5,6, 9, 10, 11, 8,11,8, 11,8,9,10,13,12,15,14};

    CSRgraph g(rows,cols);


    //dfs_bridges sol1(g);
    //cout << endl
    randomized_two_bridges<unsigned long long> solution1(&g, std_sort);
    auto t1 = std::chrono::high_resolution_clock::now();
    //CSRgraph g1 = create_graph(10000, 0.007);//(adj_list);
    CSRgraph g1(USA.get_adj_list());
    auto t2 = std::chrono::high_resolution_clock::now();
    std::cout << "generating took "
              << std::chrono::duration_cast<std::chrono::milliseconds>(t2-t1).count()
              << " milliseconds\n";
    randomized_two_bridges<unsigned long long> solution3(&g1, std_sort);

    t1 = std::chrono::high_resolution_clock::now();
    randomized_bridges<unsigned int> solution2(&g1);
    t2 = std::chrono::high_resolution_clock::now();
    std::cout << "\t "
            << std::chrono::duration_cast<std::chrono::duration<double>>(t2-t1).count()
            << " seconds\n";




    auto beee = solution3.get_answer();
    //for(auto i : beee)
     //  cout << i.first << " " << i.second << " " << endl;
    verifier test1(&g1,solution2.get_answer(), one_bridge);
    cout << "Precision is: " << test1.get_precision() <<"\t num missings" << test1.get_miss_count() << endl;


    auto be = solution2.get_answer();
    verifier test2(&g1,solution2.get_answer(), one_bridge);
    cout << "Precision is: " << test2.get_precision() <<"\t num missings" << test2.get_miss_count() << endl;

   // dfs_based_combo();

    //randomized_bridges_combo<uint8_t>();

    //randomized_two_bridges_combo<uint64_t>();


    return 0;
}