//
// Created by Артем Аросланкин on 12.11.2019.
//

#ifndef BRIDGES_GRAPH_H
#define BRIDGES_GRAPH_H

#include <iostream>
#include <vector>
#include <algorithm>
#include <cinttypes>
#include <random>
#include <chrono>
#include <set>

using namespace std;

static std::random_device rd;
static std::mt19937 gen(rd());

class CSRgraph
{
public:
    int num_vert;
    int num_edges;

    vector<int> rows;
    vector<int> cols;


    CSRgraph(){}

    CSRgraph(vector<int> rows, vector<int> cols)
    {
        this->rows = rows;
        this->cols = cols;
        num_vert = rows.size() - 1;
        num_edges = rows.back();
        cout << "|V|=" <<num_vert << "  |E|=" << num_edges << endl;
    }

    CSRgraph(const vector<set<int>>& adj_list)
    {
        vector<int> rows(adj_list.size()+1, 0);
        vector<int> cols;
        int prefix_sum = 0;
        for(int i = 0; i < adj_list.size(); i ++)
        {
            prefix_sum += adj_list[i].size();
            rows[i+1] = prefix_sum;

            for (auto j: adj_list[i])
            {
                cols.push_back(j);
            }
        }
        this->rows = rows;
        this->cols = cols;
        num_vert = rows.size() - 1;
        num_edges = rows.back();
        //cout << "|V|=" <<num_vert << "  |E|=" << num_edges << endl;
    }


    pair<int, int> get_neighbors(int i) const
    {
        return make_pair(rows[i], rows[i+1]);
    }
    int get_size_neighbors(int i) const
    {
        return (rows[i+1] - rows[i]);
    }

    void print_CSR() const
    {
        for(int i = 0; i < num_vert; i++)
        {
            cout << "Vert " << i <<": ";
            auto Ni = get_neighbors(i);
            for(int j_ = Ni.first; j_ < Ni.second; j_++)
            {
                int j = cols[j_];
                cout << j << " ";
            }
            cout << endl;
        }
    }
};





//creating graph with probability of edge creating p_in. Edge's wight characteristics are math expection and dispersion.
CSRgraph create_graph(int n, double p_in)
{
    if(n < 0)
        exit(-1);
    if(p_in < 0 || p_in > 1)
        exit(-1);
    vector<set<int>> adj_list(n);

    //random_device generator;
    bernoulli_distribution pd(p_in);

    //std::poisson_distribution<int> pd((double)n*p_in);
    for(int i = 0; i < n; i++)
    {
        for (int j = i+1; j < n; j++)
        {
            if (pd(gen))
            //if((rand() % n) < (p_in*n))
            {
                adj_list[i].insert(j);
                adj_list[j].insert(i);
            }
        }
    }

     CSRgraph graph(adj_list);

    return graph;

}

//creating block structure graph , where each block is subgraph from create_graph function.
// With addition of p_out - probability of edge between clusters and their weight characteristics
/*
vector<vector<double>> create_block_graph(int num_clusters, int num_members,  double p_in, double p_out, double mu_weight_in, double dispersion_of_weight_in, double mu_weight_out, double dispersion_of_weight_out){
    if(num_clusters < 0)
        exit(-1);
    if(num_members < 0)
        exit(-1);
    if(p_out < 0 || p_out > 1)
        exit(-1);

    vector<vector<double>> graph(num_clusters * num_members, vector<double> (num_clusters*num_members, 0));
    vector<vector<double>> tmp;

    random_device generator;
    bernoulli_distribution distribution_of_p_out(p_out);
    normal_distribution<double> distribution_of_weight_out(mu_weight_out, dispersion_of_weight_out);

    for(int k=0; k < num_clusters; k++) {
        //block planting
        tmp = create_graph(num_members, p_in, mu_weight_in, dispersion_of_weight_in);
        for(int i = 0; i < tmp.size(); i++)
            for( int j = 0; j < tmp[i].size(); j++)
                graph[k * num_members + i][k * num_members + j] = tmp[i][j];


        //addition of external edges
        for(int i = k * num_members; i < tmp.size() + k * num_members; i++)
            for(int j = (k+1) * num_members; j < graph.size(); j++)
                if (distribution_of_p_out(generator)){
                    int count = 0;
                    while(graph[i][j]<=0 || count < 100) {
                        graph[i][j] = distribution_of_weight_out(generator);
                        count++;
                    }
                    if(graph[i][j]<=0)
                        graph[i][j] = abs(distribution_of_weight_out(generator));
                    graph[j][i]=graph[i][j];
                }
    }

    return graph;
}
*/




static const int64_t kRandSeed = 27491095;


void PrintLabel(const std::string &label, const std::string &val) {
    printf("%-21s%7s\n", (label + ":").c_str(), val.c_str());
}

void PrintTime(const std::string &s, double seconds) {
    printf("%-21s%3.5lf\n", (s + ":").c_str(), seconds);
}

void PrintStep(const std::string &s, int64_t count) {
    printf("%-14s%14" PRId64 "\n", (s + ":").c_str(), count);
}

void PrintStep(int step, double seconds, int64_t count = -1) {
    if (count != -1)
        printf("%5d%11" PRId64 "  %10.5lf\n", step, count, seconds);
    else
        printf("%5d%23.5lf\n", step, seconds);
}

void PrintStep(const std::string &s, double seconds, int64_t count = -1) {
    if (count != -1)
        printf("%5s%11" PRId64 "  %10.5lf\n", s.c_str(), count, seconds);
    else
        printf("%5s%23.5lf\n", s.c_str(), seconds);
}

// Runs op and prints the time it took to execute labelled by label
#define TIME_PRINT(label, op) {   \
  Timer t_;                       \
  t_.Start();                     \
  (op);                           \
  t_.Stop();                      \
  PrintTime(label, t_.Seconds()); \
}


template <typename T_>
class RangeIter {
    T_ x_;
public:
    explicit RangeIter(T_ x) : x_(x) {}
    bool operator!=(RangeIter const& other) const { return x_ != other.x_; }
    T_ const& operator*() const { return x_; }
    RangeIter& operator++() {
        ++x_;
        return *this;
    }
};

template <typename T_>
class Range{
    T_ from_;
    T_ to_;
public:
    explicit Range(T_ to) : from_(0), to_(to) {}
    Range(T_ from, T_ to) : from_(from), to_(to) {}
    RangeIter<T_> begin() const { return RangeIter<T_>(from_); }
    RangeIter<T_> end() const { return RangeIter<T_>(to_); }
};









class Timer {
public:
    Timer() {}

    void Start() {
        elapsed_time_ = start_time_ = std::chrono::high_resolution_clock::now();
    }

    void Stop() {
        elapsed_time_ = std::chrono::high_resolution_clock::now();
    }

    double Seconds() const {
        return std::chrono::duration_cast<std::chrono::duration<double>>(elapsed_time_ - start_time_).count();
    }

    double Millisecs() const {
        return std::chrono::duration_cast<std::chrono::duration<double, std::milli>>(elapsed_time_ - start_time_).count();
    }

    double Microsecs() const {
        return std::chrono::duration_cast<std::chrono::duration<double, std::micro>>(elapsed_time_ - start_time_).count();
    }

private:
    std::chrono::high_resolution_clock::time_point start_time_, elapsed_time_;
};

// Times op's execution using the timer t
#define TIME_OP(t, op) { t.Start(); (op); t.Stop(); }









// Used to hold node & weight, with another node it makes a weighted edge
template <typename NodeID_, typename WeightT_>
struct NodeWeight {
    NodeID_ v;
    WeightT_ w;
    NodeWeight() {}
    NodeWeight(NodeID_ v) : v(v), w(1) {}
    NodeWeight(NodeID_ v, WeightT_ w) : v(v), w(w) {}

    bool operator< (const NodeWeight& rhs) const {
        return v == rhs.v ? w < rhs.w : v < rhs.v;
    }

    // doesn't check WeightT_s, needed to remove duplicate edges
    bool operator== (const NodeWeight& rhs) const {
        return v == rhs.v;
    }

    // doesn't check WeightT_s, needed to remove self edges
    bool operator== (const NodeID_& rhs) const {
        return v == rhs;
    }

    operator NodeID_() {
        return v;
    }
};

template <typename NodeID_, typename WeightT_>
std::ostream& operator<<(std::ostream& os,
                         const NodeWeight<NodeID_, WeightT_>& nw) {
    os << nw.v << " " << nw.w;
    return os;
}

template <typename NodeID_, typename WeightT_>
std::istream& operator>>(std::istream& is, NodeWeight<NodeID_, WeightT_>& nw) {
    is >> nw.v >> nw.w;
    return is;
}

// Syntatic sugar for an edge
template <typename SrcT, typename DstT = SrcT>
struct EdgePair {
    SrcT u;
    DstT v;

    EdgePair() {}

    EdgePair(SrcT u, DstT v) : u(u), v(v) {}
};


typedef int32_t SGID;
typedef EdgePair<SGID> SGEdge;
typedef int64_t SGOffset;

template <typename NodeID_, typename DestID_ = NodeID_,
        typename WeightT_ = NodeID_>
class Generator {
    typedef EdgePair<NodeID_, DestID_> Edge;
    typedef EdgePair<NodeID_, NodeWeight<NodeID_, WeightT_>> WEdge;
    typedef vector<Edge> EdgeList;

public:
    Generator(int scale, int degree) {
        scale_ = scale;
        num_nodes_ = 1l << scale;
        num_edges_ = num_nodes_ * degree;
        if (num_nodes_ > std::numeric_limits<NodeID_>::max()) {
            std::cout << "NodeID type (max: " << std::numeric_limits<NodeID_>::max();
            std::cout << ") too small to hold " << num_nodes_ << std::endl;
            std::cout << "Recommend changing NodeID (typedef'd in src/benchmark.h)";
            std::cout << " to a wider type and recompiling" << std::endl;
            std::exit(-31);
        }
    }

    void PermuteIDs(EdgeList &el) {
        vector<NodeID_> permutation(num_nodes_);
        std::mt19937 rng(kRandSeed);
#pragma omp parallel for
        for (NodeID_ n=0; n < num_nodes_; n++)
            permutation[n] = n;
        shuffle(permutation.begin(), permutation.end(), rng);
#pragma omp parallel for
        for (int64_t e=0; e < num_edges_; e++)
            el[e] = Edge(permutation[el[e].u], permutation[el[e].v]);
    }

    EdgeList MakeUniformEL() {
        EdgeList el(num_edges_);
#pragma omp parallel
        {
            std::mt19937 rng;
            std::uniform_int_distribution<NodeID_> udist(0, num_nodes_-1);
#pragma omp for
            for (int64_t block=0; block < num_edges_; block+=block_size) {
                rng.seed(kRandSeed + block/block_size);
                for (int64_t e=block; e < std::min(block+block_size, num_edges_); e++) {
                    el[e] = Edge(udist(rng), udist(rng));
                }
            }
        }
        return el;
    }

    EdgeList MakeRMatEL() {
        const float A = 0.57f, B = 0.19f, C = 0.19f;
        EdgeList el(num_edges_);
#pragma omp parallel
        {
            std::mt19937 rng;
            std::uniform_real_distribution<float> udist(0, 1.0f);
#pragma omp for
            for (int64_t block=0; block < num_edges_; block+=block_size) {
                rng.seed(kRandSeed + block/block_size);
                for (int64_t e=block; e < std::min(block+block_size, num_edges_); e++) {
                    NodeID_ src = 0, dst = 0;
                    for (int depth=0; depth < scale_; depth++) {
                        float rand_point = udist(rng);
                        src = src << 1;
                        dst = dst << 1;
                        if (rand_point < A+B) {
                            if (rand_point > A)
                                dst++;
                        } else {
                            src++;
                            if (rand_point > A+B+C)
                                dst++;
                        }
                    }
                    el[e] = Edge(src, dst);
                }
            }
        }
        PermuteIDs(el);
        // TIME_PRINT("Shuffle", std::shuffle(el.begin(), el.end(),
        //                                    std::mt19937()));
        return el;
    }

    EdgeList GenerateEL(bool uniform) {
        EdgeList el;
        Timer t;
        t.Start();
        if (uniform)
            el = MakeUniformEL();
        else
            el = MakeRMatEL();
        t.Stop();
        PrintTime("Generate Time", t.Seconds());
        return el;
    }

    static void InsertWeights(vector<EdgePair<NodeID_, NodeID_>> &el) {}

    // Overwrites existing weights with random from [1,255]
    static void InsertWeights(vector<WEdge> &el) {
#pragma omp parallel
        {
            std::mt19937 rng;
            std::uniform_int_distribution<int> udist(1, 255);
            int64_t el_size = el.size();
#pragma omp for
            for (int64_t block=0; block < el_size; block+=block_size) {
                rng.seed(kRandSeed + block/block_size);
                for (int64_t e=block; e < std::min(block+block_size, el_size); e++) {
                    el[e].v.w = static_cast<WeightT_>(udist(rng));
                }
            }
        }
    }

private:
    int scale_;
    int64_t num_nodes_;
    int64_t num_edges_;
    static const int64_t block_size = 1<<18;
};

void generateGraph()
{

}








#endif //BRIDGES_GRAPH_H
