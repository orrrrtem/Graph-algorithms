#include <iostream>
#include <unistd.h>
#include "graph.h"
#include "algorithms.h"
#include "benchmark.h"
#include "reader.h"

//#include "sorts.h"

using namespace std;


int main() {



    auto t11 = std::chrono::high_resolution_clock::now();
    //sleep(2);
    auto t22 = std::chrono::high_resolution_clock::now();
    std::cout << "time: "
              << std::chrono::duration_cast<std::chrono::microseconds>(t22 - t11).count()
              << " microseconds\n";
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




    ifstream file("/Users/artemaroslankin/Downloads/roadNet-CA.txt");

    //reader USA("/Users/artemaroslankin/Downloads/roadNet-CA.txt");

    //vector<unsigned int> rows = {0,2,4,6,9,12,14,16,18,19,20};
   // vector<unsigned int> cols = {1,2,0,3,0,3,1,2,4,3,5,7,4,6,5,7,4,6,9,8};
    vector<unsigned int> rows = {0,3,6,10,14,19,22,25,28, 31, 33 , 35, 38,39,40,41,42,42};
    vector<unsigned int> cols = {1,2,3,0,2,3,0,1,3,4,0,1,2,4,2,3,5,6,7,4,6,7,4,5,7,4,5,6, 9, 10, 11, 8,11,8, 11,8,9,10,13,12,15,14};

    CSRgraph g(rows,cols);


    //dfs_bridges sol1(g);
    //cout << endl;

    randomized_two_bridges<unsigned long long> solution1(&g, std_sort);

    vector<set<unsigned int>> adj_list(1000);

    for(int i = 0; i < adj_list.size();i++)
    {
        for(int j = 0; j < adj_list.size(); j+=1)
        {
            if(i != j)
                adj_list[i].insert(j);
        }

    }

    auto t1 = std::chrono::high_resolution_clock::now();

    CSRgraph g1 = create_graph(10000, 0.007);//(adj_list);
    //CSRgraph g1(USA.get_adj_list());
    auto t2 = std::chrono::high_resolution_clock::now();
    std::cout << "generating took "

    << std::chrono::duration_cast<std::chrono::milliseconds>(t2-t1).count()
            << " milliseconds\n";

    for(int i = 0; i < g1.get_num_vert();i++)
    {
        auto ni = g1.get_neighbors(i);
        //cout << i <<":   ";
        for(auto j = ni.first; j < ni.second; j++)
        {
            //cout << g1.cols[j] << " ";
        }
        //cout << endl;
    }

    //g1 = USA.get_graph();
    randomized_two_bridges<unsigned long long> solution3(&g1, std_sort);


    cout << g1.num_vert << " " << g1.num_edges << endl;
    //g1.print_CSR();


    t1 = std::chrono::high_resolution_clock::now();
    //randomized_two_bridges<unsigned long long> solution2(&g1,std_sort);
    randomized_bridges<unsigned int> solution2(&g1);
    t2 = std::chrono::high_resolution_clock::now();
    std::cout << "\t "
            << std::chrono::duration_cast<std::chrono::microseconds>(t2-t1).count()
            << " microseconds\n";




    auto beee = solution3.get_answer();
    //for(auto i : beee)
     //  cout << i.first << " " << i.second << " " << endl;
    verifier test1(&g1,solution2.get_answer(), one_bridge);
    cout << "Precision is: " << test1.get_precision() <<"\t num missings" << test1.get_miss_count() << endl;


    auto be = solution2.get_answer();
    verifier test2(&g1,solution2.get_answer(), one_bridge);
    cout << "Precision is: " << test2.get_precision() <<"\t num missings" << test2.get_miss_count() << endl;

    //randomized_bridges_combo<unsigned long long>();

    randomized_two_bridges_combo<uint64_t>();


    return 0;
}