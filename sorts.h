//
// Created by Артем Аросланкин on 22.11.2019.
//

#ifndef BRIDGES_SORTS_H
#define BRIDGES_SORTS_H

#include <iostream>
#include <vector>
#include <algorithm>
#include "algorithms.h"


using namespace std;

// A function to do counting sort of arr[] according to
// the digit represented by exp.
template< class weight_type>
weight_type get_max(const vector<pair <weight_type, pair<int,int> >> & weights_for_component)
{
    weight_type mx = 0;
    for (int i = 0; i < weights_for_component.size(); i++)
        if (weights_for_component[i].first > mx)
            mx = weights_for_component[i].first;
    return mx;
}



template< class weight_type>
weight_type get_min(const vector<pair <weight_type, pair<int,int> >> & weights_for_component)
{
    weight_type mn = weights_for_component[0].first;
    for (int i = 0; i < weights_for_component.size(); i++)
        if (weights_for_component[i].first < mn)
            mn = weights_for_component[i].first;
    return mn;
}

template< class weight_type>
void count_sort(vector<pair <weight_type, pair<int,int> >> & weights_for_component, int exp)
{
    vector<pair <weight_type, pair<int,int> >>  output(weights_for_component.size()); // output array
    int i;
    vector<int> count(10,0);

    for (i = 0; i < weights_for_component.size(); i++)
        count[ (weights_for_component[i].first / exp) % 10 ]++;

    for (i = 1; i < 10; i++)
        count[i] += count[i - 1];

    for (i = weights_for_component.size() - 1; i >= 0; i--)
    {
        output[count[ (weights_for_component[i].first / exp) % 10 ] - 1].first = weights_for_component[i].first;
        output[count[ (weights_for_component[i].first / exp) % 10 ] - 1].second = weights_for_component[i].second;
        count[ (weights_for_component[i].first / exp) % 10 ]--;
    }

    for (i = 0; i < weights_for_component.size(); i++)
        weights_for_component.at(i) = output[i];
}




template< class weight_type>
void radixsort(vector<pair <weight_type, pair<int,int> >> & weights_for_component)
{
    int m = get_max(weights_for_component);
    for (int exp = 1; m/exp > 0; exp *= 10)
        count_sort(weights_for_component, exp);
}




template< class weight_type>
void bucketSort(vector<pair <weight_type, pair<int,int> >> & weights_for_component)
{
    //cout << "sad";
    /*
    int n = (int) weights_for_component.size();
    weight_type mx = get_max(weights_for_component);
    weight_type mn = get_min(weights_for_component);
    vector<vector<pair <weight_type, pair<int,int> >>> bucket(n) ;
    //vector<pair <weight_type, pair<int,int> >> bucket[10];
    //cout << "s        SSSSSSSS";
    auto bucket_range = ((mx-mn + 1) / n);


    for (int i=0; i < n; i++)
    {
        int bucket_index = weights_for_component[i].first / bucket_range;
        bucket[bucket_index].push_back(&weights_for_component[i]);
    }


    vector<pair <weight_type, pair<int,int> >>* bucket = new vector<pair <weight_type, pair<int,int> >>[n];
    int bucket_range = (numeric_limits<weight_type>::max() ) / n;
    for (int i=0; i < n; i++)
    {
        int bucket_index = weights_for_component[i].first / bucket_range;
        bucket[bucket_index].push_back(weights_for_component[i]);
    }


    for (int i=0; i<n; i++)
        std::sort(bucket[i].begin(), bucket[i].end());

    int index = 0;
    for (int i = 0; i < n; i++)
        for (int j = 0; j < bucket[i].size(); j++)
            weights_for_component[index++] = bucket[i][j];

*/
}









#endif //BRIDGES_SORTS_H
