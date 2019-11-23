//
// Created by Артем Аросланкин on 22.11.2019.
//

#ifndef BRIDGES_BENCHMARK_H
#define BRIDGES_BENCHMARK_H

#include <unistd.h>
#include "graph.h"
#include "algorithms.h"

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




void randomized_bridges_combo() {
    vector<double> p_vec;
    double start_p = 0.0001;
    for (int scale = 1; scale < 2; scale++) {

        for (int s = 1; s < 10; s++) {
            p_vec.push_back(start_p * s);
        }
        start_p = start_p * 10;
    }

    for (int n = 10000; n < 10001; n = n * 10) {

        for (int j = 0; j < p_vec.size(); j++) {
            CSRgraph g_inner = create_graph(n, p_vec[j]);

            auto t1 = std::chrono::high_resolution_clock::now();
            randomized_bridges<int> solution1(&g_inner);
            auto t2 = std::chrono::high_resolution_clock::now();
            std::cout << "n = " << n << ";\tp = " << p_vec[j] << "\ttime: "
                      << std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count()
                      << " microseconds\n";
        }
    }
}

void randomized_two_bridges_combo(sort_choice sort_type) {

    cout << "Benchamrk started" << endl << endl;
    vector<double> p_vec;
    double start_p = 0.0001;
    for (int scale = 1; scale < 4; scale++) {

        for (int s = 1; s < 10; s++) {
            p_vec.push_back(start_p * s);
        }
        start_p = start_p * 10;
    }

    for (int n = 100; n < 10001; n = n * 10) {

        for (int j = 0; j < p_vec.size(); j++) {
            CSRgraph g_inner = create_graph(n, p_vec[j]);

            auto t1 = std::chrono::high_resolution_clock::now();
            randomized_two_bridges<int> solution1(&g_inner,std_sort);
            auto t2 = std::chrono::high_resolution_clock::now();
            std::cout << "Std sort:n = " << n << ";\tp = " << p_vec[j] << "\ttime: "
                      << std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count()
                      << " microseconds\n";


            t1 = std::chrono::high_resolution_clock::now();
            randomized_two_bridges<int> solution3(&g_inner,bucket_sort);
            t2 = std::chrono::high_resolution_clock::now();
            std::cout << " Bucket sort:    n = " << n << ";\tp = " << p_vec[j] << "\ttime: "
                      << std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count()
                      << " microseconds\n";

            t1 = std::chrono::high_resolution_clock::now();
            randomized_two_bridges<int> solution2(&g_inner,radix_sort);
            t2 = std::chrono::high_resolution_clock::now();
            std::cout << "Radix sort:n = " << n << ";\tp = " << p_vec[j] << "\ttime: "
                      << std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count()
                      << " microseconds\n";


        }
    }
}






#endif //BRIDGES_BENCHMARK_H
