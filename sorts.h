//
// Created by Артем Аросланкин on 22.11.2019.
//

#ifndef BRIDGES_SORTS_H
#define BRIDGES_SORTS_H

#include <iostream>
#include <vector>
#include <algorithm>
#include "algorithms.h"
#include <math.h>


using namespace std;

// A function to do counting sort of arr[] according to
// the digit represented by exp.
template< class weight_type>
weight_type get_max(const vector<pair <weight_type, pair<unsigned int,unsigned int> >> & weights_for_component)
{
    weight_type mx = 0;
    for (int i = 0; i < weights_for_component.size(); i++)
        if (weights_for_component[i].first > mx)
            mx = weights_for_component[i].first;
    return mx;
}



template< class weight_type>
weight_type get_min(const vector<pair <weight_type, pair<unsigned int,unsigned int> >> & weights_for_component)
{
    weight_type mn = weights_for_component[0].first;
    for (auto i = 0; i < weights_for_component.size(); i++)
        if (weights_for_component[i].first < mn)
            mn = weights_for_component[i].first;
    return mn;
}

template< class weight_type>
void count_sort(vector<pair <weight_type, pair<unsigned int,unsigned int> >> & weights_for_component, unsigned int exp, const unsigned int radix_size)
{
    vector<pair <weight_type, pair<unsigned int, unsigned int> >>  output(weights_for_component.size()); // output array
    vector<unsigned int> count(pow(2, radix_size),0);


    for (unsigned int i = 0; i < weights_for_component.size(); i++)
        count[ (weights_for_component[i].first
        << (sizeof(weight_type) * 8 - radix_size - exp * radix_size))
        >> (sizeof(weight_type) * 8 -radix_size)
        ]++;


    for (unsigned int i = 1; i < count.size(); i++)
        count[i] += count[i - 1];



    for (auto i = 0; i < weights_for_component.size(); i++)
    {
        auto count_index = ((weights_for_component[i].first
                << (sizeof(weight_type) * 8 - radix_size - exp * radix_size))
                >> (sizeof(weight_type) * 8 -radix_size ));

        output[count[count_index] - 1].first = weights_for_component[i].first;
        output[count[count_index] - 1].second = weights_for_component[i].second;
        count[count_index]--;
    }


//swap TODO
    weights_for_component.swap(output);
}




template< class weight_type>
void radixsort(vector<pair <weight_type, pair<unsigned int,unsigned int> >> & weights_for_component, const unsigned int radix_size)
{

    auto m = get_max(weights_for_component);
    unsigned int max_hex_bit = 0;
    unsigned int m_for_bits = m;
    while(m_for_bits != 0)
    {
        max_hex_bit++;
        m_for_bits = m_for_bits >> radix_size;
    }
    //TODO make exp 2^16 with >>

    for (unsigned int exp = 0; exp < max_hex_bit; exp++)
    {
        count_sort(weights_for_component, exp, radix_size);
    }

}



template< class weight_type>
void bucketSort(vector<pair <weight_type, pair<unsigned int,unsigned int> >> & weights_for_component)
{
    auto n = weights_for_component.size();
    if (n == 0)
    {
        return;
    }

    weight_type mx = get_max(weights_for_component);
    weight_type mn = get_min(weights_for_component);
    vector<vector<pair <weight_type, pair<unsigned int, unsigned int> > > > bucket(n) ;
    auto bucket_range = ((mx-mn) / n) + 1;


    for (unsigned int i=0; i < n; i++)
    {
        unsigned int bucket_index = (weights_for_component[i].first - mn) / bucket_range;
        bucket[bucket_index].push_back(weights_for_component[i]);
    }



    for(auto i = 0; i < bucket.size(); i++)
    {
        sort(bucket[i].begin(),bucket[i].end());
    }

    unsigned int index = 0;
    for (unsigned int i = 0; i < n; i++)
        for (unsigned int j = 0; j < bucket[i].size(); j++)
            weights_for_component[index++] = bucket[i][j];


}



template< class weight_type>
void bucketTreesSort(vector<pair <weight_type, pair<unsigned int,unsigned int> >> & weights_for_component)
{
    auto n = weights_for_component.size();
    if (n == 0)
    {
        return;
    }

    weight_type mx = get_max(weights_for_component);
    weight_type mn = get_min(weights_for_component);
    vector<set<pair <weight_type, pair<unsigned int,unsigned int> > > > bucket(n) ;
    auto bucket_range = ((mx-mn) / n) + 1;


    for (unsigned int i=0; i < n; i++)
    {
        unsigned int bucket_index = (weights_for_component[i].first - mn) / bucket_range;
        bucket[bucket_index].insert(weights_for_component[i]);
    }



    unsigned int index = 0;
    for (unsigned i = 0; i < n; i++)
        for (auto j: bucket[i])
            weights_for_component[index++] = j;


}









#endif //BRIDGES_SORTS_H
