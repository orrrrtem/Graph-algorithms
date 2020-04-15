//
// Created by Артем Аросланкин on 22.11.2019.
//

#ifndef BRIDGES_BENCHMARK_H
#define BRIDGES_BENCHMARK_H

//#include <unistd.h>
#include "graph.h"
#include "algorithms.h"
#include "FL_SSSP.h"
#include "tests.h"
#include "reader.h"
#include <fstream>
#include <tuple>

using namespace std;

void dfs_based_combo() {
    cout << "Benchamrk started" << endl << endl;
    const int n_start = 1000;
    const int n_end = 20000;
    const int n_step = 100;
    vector<double> p_vec;
    double start_p = 0.001;
    double step_p = 0.005;
    double end_p = 0.004;

    double p = start_p;
    while(p<=end_p)
    {
        p_vec.push_back(p);
        p+= step_p;
    }

    cout << p_vec.size() << endl;

    vector < tuple<int, double, double>> results;

    for (unsigned int n = n_start; n < n_end; n = n + n_step) {

        for (unsigned int j = 0; j < p_vec.size(); j++) {
            CSRgraph g_inner = create_graph(n, p_vec[j]);
            auto t1 = std::chrono::high_resolution_clock::now();
            dfs_bridges solution1(g_inner);
            auto t2 = std::chrono::high_resolution_clock::now();

            results.push_back(make_tuple(n,p_vec[j],std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count()));
        }
        if(n>2000)
            cout << n << " collected" << endl;
    }

    std::ofstream std_myfile;
    std_myfile.open ("/Users/artemaroslankin/Documents/Gribanov/determ_bridge.csv");
    for(int i = 0; i < p_vec.size(); i++)
        std_myfile << "," << p_vec[i];
    std_myfile << "\n";
    int index = 0;
    for (unsigned int n = n_start; n < n_end; n = n + n_step)
    {
        std_myfile << n;
        for (unsigned int j = 0; j < p_vec.size(); j++, index++)
        {
            std_myfile << "," << get<2>(results[index]);
        }
        std_myfile << "\n";
    }
    std_myfile.close();
}



template <class type_for_rand>
void randomized_bridges_combo() {
    cout << "Benchamrk started" << endl << endl;
    const int n_start = 1000;
    const int n_end = 20000;
    const int n_step = 100;
    vector<double> p_vec;
    double start_p = 0.001;
    double step_p = 0.005;
    double end_p = 0.004;

    double p = start_p;
    while(p<=end_p)
    {
        p_vec.push_back(p);
        p+= step_p;
    }

    vector < tuple<int, double, double>> results;
    vector < tuple<int, double, double>> accuracy_results;

    for (unsigned int n = n_start; n < n_end; n = n + n_step) {

        for (unsigned int j = 0; j < p_vec.size(); j++) {
            CSRgraph g_inner = create_graph(n, p_vec[j]);

            auto t1 = std::chrono::high_resolution_clock::now();

            randomized_bridges<type_for_rand> solution1(&g_inner);

            auto t2 = std::chrono::high_resolution_clock::now();
            verifier test1(&g_inner,solution1.get_answer(), one_bridge);
            results.push_back(make_tuple(n,p_vec[j],std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count()));
            accuracy_results.push_back(make_tuple(n,p_vec[j],test1.get_precision()));

        }
        if(n>2000)
            cout << n << " collected" << endl;
    }

    std::ofstream std_myfile;
    std_myfile.open ("/Users/artemaroslankin/Documents/Gribanov/randomized_bridge.csv");
    for(int i = 0; i < p_vec.size(); i++)
        std_myfile << "," << p_vec[i];
    std_myfile << "\n";
    int index = 0;
    for (unsigned int n = n_start; n < n_end; n = n + n_step)
    {
        std_myfile << n;
        for (unsigned int j = 0; j < p_vec.size(); j++, index++)
        {
            std_myfile << "," << get<2>(results[index]);
        }
        std_myfile << "\n";
    }
    std_myfile.close();


    std_myfile.open ("/Users/artemaroslankin/Documents/Gribanov/random_bridges_accuracy.csv");
    for(int i = 0; i < p_vec.size(); i++)
        std_myfile << "," << p_vec[i];
    std_myfile << "\n";
    index = 0;
    for (unsigned int n = n_start; n < n_end; n = n + n_step)
    {
        std_myfile << n;
        for (unsigned int j = 0; j < p_vec.size(); j++, index++)
        {
            std_myfile << "," << get<2>(accuracy_results[index]);
        }
        std_myfile << "\n";
    }
    std_myfile.close();
}


template <class type_for_rand>
void randomized_two_bridges_combo() {

    cout << "Benchamrk started" << endl << endl;
    const int n_start = 1000;
    const int n_end = 20000;
    const int n_step = 100;
    vector<double> p_vec;
    double start_p = 0.001;
    double step_p = 0.005;
    double end_p = 0.004;
    double p = start_p;
    while(p<=end_p)
    {
        p_vec.push_back(p);
        p+= step_p;
    }
    vector < tuple<int, double, double>> results_std, results_radix, results_bucket;
    vector < tuple<int, double, double>> accuracy_results_std, accuracy_results_radix, accuracy_results_bucket;
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
            accuracy_results_std.push_back(make_tuple(n,p_vec[j],test1.get_precision()));

            t1 = std::chrono::high_resolution_clock::now();
            randomized_two_bridges<type_for_rand> solution3(&g_inner,bucket_sort);
            t2 = std::chrono::high_resolution_clock::now();
           // std::cout << " Bucket sort:    n = " << n << ";\tp = " << p_vec[j] << "\ttime: "
               //       << std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count()
                 //     << " microseconds\n";

            verifier test3(&g_inner,solution3.get_answer(), two_bridge);
            //cout << "Precision is: " << test3.get_precision()  << "\t num missings" << test3.get_miss_count() << endl;

            results_bucket.push_back(make_tuple(n,p_vec[j],std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count()));
            accuracy_results_bucket.push_back(make_tuple(n,p_vec[j],test3.get_precision()));




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
            accuracy_results_radix.push_back(make_tuple(n,p_vec[j],test2.get_precision()));

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
    std_myfile.close();

    std_myfile.open ("/Users/artemaroslankin/Documents/Gribanov/radix_sort.csv");
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
    std_myfile.close();

    std_myfile.open ("/Users/artemaroslankin/Documents/Gribanov/two_bridges_accuracy.csv");
    for(int i = 0; i < p_vec.size(); i++)
        std_myfile << "," << p_vec[i];
    std_myfile << "\n";
    index = 0;
    for (unsigned int n = n_start; n < n_end; n = n + n_step)
    {
        std_myfile << n;
        for (unsigned int j = 0; j < p_vec.size(); j++, index++)
        {
            std_myfile << "," << get<2>(accuracy_results_std[index]);
        }
        std_myfile << "\n";
    }
    std_myfile.close();
}


template<class type>
void real_graph_becnhmark_bridges(const string& file_path) {
    cout << "Real graph measure started" << endl;
    Timer time;
    time.Start();
    reader USA(file_path);
    time.Stop();
    std::cout << "read in\t "
              << time.Seconds()
              << " seconds from " <<file_path <<"\n";

    time.Start();
    CSRgraph g(USA.get_adj_list());
    time.Stop();
    std::cout << "CSR build took\t "
              << time.Seconds()
              << " seconds\n";

    cout << "|V|=" << g.get_num_vert() <<"   |E|=" << g.get_num_edges() << endl;

    time.Start();
    randomized_bridges<type> solution1(&g);
    time.Stop();
    std::cout << "randomized bridges\t "
              << time.Seconds()
              << " seconds\n";

    //verifier test1(&g,solution1.get_answer(), one_bridge);
    //cout << "one_bridge precision is: " << test1.get_precision() <<"\t num missings" << test1.get_miss_count() << endl;
    cout << "number of one bridges: " <<solution1.get_answer().size() << endl;

    time.Start();
    randomized_two_bridges<type> solution2(&g,std_sort);
    time.Stop();
    std::cout << "randomized two bridges with std sort\t "
              << time.Seconds()
              << " seconds\n";
    cout << "number of two bridges: " <<solution2.get_answer().size() << endl;
    //verifier test2(&g,solution2.get_answer(), two_bridge);
    //cout << "precision is: " << test2.get_precision() <<"\t num missings" << test2.get_miss_count() << endl;

    time.Start();
    randomized_two_bridges<type> solution3(&g,radix_sort);
    time.Stop();
    std::cout << "randomized two bridges with radix sort\t "
              << time.Seconds()
              << " seconds\n";

    //verifier test3(&g,solution3.get_answer(), two_bridge);
    //cout << "precision is: " << test3.get_precision() <<"\t num missings" << test3.get_miss_count() << endl;


    time.Start();
    randomized_two_bridges<type> solution4(&g,bucket_sort);
    time.Stop();
    std::cout << "randomized two bridges with bucket sort\t "
              << time.Seconds()
              << " seconds\n";

    //verifier test4(&g,solution4.get_answer(), two_bridge);
    //cout << " precision is: " << test4.get_precision() <<"\t num missings" << test4.get_miss_count() << endl;

}




template<class weight_type>
void real_graph_becnhmark_shortest_paths(const string& file_path, bool to_display = false) {
    cout << "Real graph measure started" << endl;
    Timer time;
    time.Start();
    reader graph(file_path,true);
    time.Stop();
    std::cout << "read in\t "
              << time.Seconds()
              << " seconds from " <<file_path <<"\n";

    time.Start();
    vector<vector<weight_type>> g = adjacency_from_COO(graph.source, graph.dest, graph.weights);
    time.Stop();
    std::cout << "Adj matrix build took\t "
              << time.Seconds()
              << " seconds\n";

    cout << "|V|=" << g.size() << endl;

    time.Start();
    apsp_floid<weight_type, unsigned int> sd(g);
    time.Stop();
    std::cout << "APSP:\t "
              << time.Seconds()
              << " seconds\n";

    if (to_display == true) {
        sd.print_result();
    }
}



#endif //BRIDGES_BENCHMARK_H
