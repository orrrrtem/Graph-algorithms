//
// Created by Артем Аросланкин on 22.11.2019.
//

#ifndef BRIDGES_BENCHMARK_H
#define BRIDGES_BENCHMARK_H

#include <unistd.h>
#include "graph.h"
#include "algorithms.h"
#include "tests.h"
#include <fstream>


using namespace std;


//TODO make verifier
void dfs_based_combo() {
    vector<double> p_vec;
    double start_p = 0.0001;
    for (int scale = 1; scale < 5; scale++) {

        for (int s = 1; s < 10; s++) {
            p_vec.push_back(start_p * s);
        }
        start_p = start_p * 10;
    }

    for (int n = 100; n < 10001; n = n * 10) {

        for (int j = 0; j < p_vec.size(); j++) {
            CSRgraph g_inner = create_graph(n, p_vec[j]);

            auto t1 = std::chrono::high_resolution_clock::now();
            dfs_bridges solution1(g_inner);
            auto t2 = std::chrono::high_resolution_clock::now();
            std::cout << "n = " << n << ";\tp = " << p_vec[j] << "\ttime: "
                      << std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count()
                      << " microseconds\n";
        }
    }
}



template <class type_for_rand>
void randomized_bridges_combo() {
    vector<double> p_vec;
    double start_p = 0.0001;
    for (unsigned int scale = 1; scale < 2; scale++) {

        for (unsigned int s = 1; s < 10; s++) {
            p_vec.push_back(start_p * s);
        }
        start_p = start_p * 10;
    }

    for (unsigned int n = 10; n < 10001; n = n * 10) {

        for (unsigned int j = 0; j < p_vec.size(); j++) {
            CSRgraph g_inner = create_graph(n, p_vec[j]);

            auto t1 = std::chrono::high_resolution_clock::now();

            randomized_bridges<type_for_rand> solution1(&g_inner);

            auto t2 = std::chrono::high_resolution_clock::now();
            std::cout << "n = " << n << ";\tp = " << p_vec[j] << "\ttime: "
                      << std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count()
                      << " microseconds\n";


            verifier test1(&g_inner,solution1.get_answer(), one_bridge);
            cout << "Precision is: " << test1.get_precision() <<"\t num missings" << test1.get_miss_count() << endl;
        }
    }
}


template <class type_for_rand>
void randomized_two_bridges_combo() {

    cout << "Benchamrk started" << endl << endl;
    const int n_start = 1000;
    const int n_end = 10000;
    const int n_step = 500;
    vector<double> p_vec;
    double start_p = 0.0001;
    double step_p = 0.0005;
    double end_p = 0.05;

    double p = start_p;
    while(p<=end_p)
    {
        p_vec.push_back(p);
        p+= step_p;
    }

    cout << p_vec.size() << endl;

//
//    for (unsigned int scale = 1; scale < 4; scale++) {
//
//        for (unsigned int s = 1; s < 10; s++) {
//            p_vec.push_back(start_p * s);
//        }
//        start_p = start_p * 10;
//    }
    vector < tuple<int, double, double>> results_std, results_radix, results_bucket;

    for (unsigned int n = n_start; n < n_end; n = n + n_step) {

        for (unsigned int j = 0; j < p_vec.size(); j++) {
            CSRgraph g_inner = create_graph(n, p_vec[j]);

            //cout << "|V| = " << g_inner.get_num_vert() << "\t |E| = " << g_inner.get_num_edges() << endl;

            auto t1 = std::chrono::high_resolution_clock::now();
            randomized_two_bridges<type_for_rand> solution1(&g_inner,std_sort);
            auto t2 = std::chrono::high_resolution_clock::now();
           // std::cout << "Std sort:n = " << n << ";\tp = " << p_vec[j] << "\ttime: "
            //          << std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count()
              //        << " microseconds\n";
            verifier test1(&g_inner,solution1.get_answer(), two_bridge);
           // cout << "Precision is: " << test1.get_precision() <<"\t num missings" << test1.get_miss_count() << endl;

            results_std.push_back(make_tuple(n,p_vec[j],std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count()));

            t1 = std::chrono::high_resolution_clock::now();
            randomized_two_bridges<type_for_rand> solution3(&g_inner,bucket_sort);
            t2 = std::chrono::high_resolution_clock::now();
           // std::cout << " Bucket sort:    n = " << n << ";\tp = " << p_vec[j] << "\ttime: "
               //       << std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count()
                 //     << " microseconds\n";

            verifier test3(&g_inner,solution3.get_answer(), two_bridge);
            //cout << "Precision is: " << test3.get_precision()  << "\t num missings" << test3.get_miss_count() << endl;

            results_bucket.push_back(make_tuple(n,p_vec[j],std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count()));



            t1 = std::chrono::high_resolution_clock::now();
            randomized_two_bridges<type_for_rand> solution2(&g_inner,radix_sort);
            t2 = std::chrono::high_resolution_clock::now();

            //std::cout << "Radix sort:n = " << n << ";\tp = " << p_vec[j] << "\ttime: "
                    //  << std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count()
                 //     << " microseconds\n";


            verifier test2(&g_inner,solution2.get_answer(), two_bridge);
           // cout << "Precision is: " << test2.get_precision() <<"\t num missings" << test2.get_miss_count() << endl;
           // cout << endl;



            results_radix.push_back(make_tuple(n,p_vec[j],std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count()));


        }
        if(n>2000)
            cout << n << " collected" << endl;
    }

    std::ofstream std_myfile;
    std_myfile.open ("/Users/artemaroslankin/Documents/Gribanov/std_sort.csv");
    //std_myfile << ",";
    for(int i = 0; i < p_vec.size(); i++)
        std_myfile << "," << p_vec[i];
    std_myfile << "\n";
    int index = 0;
    for (unsigned int n = n_start; n < n_end; n = n + n_step)
    {
        std_myfile << n;
        for (unsigned int j = 0; j < p_vec.size(); j++, index++)
        {
            std_myfile << "," << get<2>(results_std[index]);
        }
        std_myfile << "\n";
    }

    //myfile << "This is the first cell in the first column.\n";

    std_myfile.close();



    //std::ofstream bucket_myfile;
    std_myfile.open ("/Users/artemaroslankin/Documents/Gribanov/bucket_sort.csv");
    //std_myfile << ",";
    for(int i = 0; i < p_vec.size(); i++)
        std_myfile << "," << p_vec[i];
    std_myfile << "\n";
     index = 0;
    for (unsigned int n = n_start; n < n_end; n = n + n_step)
    {
        std_myfile << n;
        for (unsigned int j = 0; j < p_vec.size(); j++, index++)
        {
            std_myfile << "," << get<2>(results_bucket[index]);
        }
        std_myfile << "\n";
    }

    //myfile << "This is the first cell in the first column.\n";

    std_myfile.close();



    std_myfile.open ("/Users/artemaroslankin/Documents/Gribanov/radix_sort.csv");
    //std_myfile << ",";
    for(int i = 0; i < p_vec.size(); i++)
        std_myfile << "," << p_vec[i];
    std_myfile << "\n";
    index = 0;
    for (unsigned int n = n_start; n < n_end; n = n + n_step)
    {
        std_myfile << n;
        for (unsigned int j = 0; j < p_vec.size(); j++, index++)
        {
            std_myfile << "," << get<2>(results_radix[index]);
        }
        std_myfile << "\n";
    }

    //myfile << "This is the first cell in the first column.\n";

    std_myfile.close();
}






#endif //BRIDGES_BENCHMARK_H
