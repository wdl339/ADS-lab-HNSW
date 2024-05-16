#pragma once
#include <iostream>
#include "base.hpp"
#include <vector>
#include <cstring>

#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <random>
#include <cassert>

namespace HNSWLab {
    
    
    class HNSW : public AlgorithmInterface {
    public:
        // you can add more parameter to initialize HNSW
        HNSW() {}

        void insert(const int *item, int label);

        std::vector<int> query(const int *query, int k);

        ~HNSW() {}
    };

    /**
     * input:
     * iterm: the vector to be inserted
     * label: the label(id) of the vector
    */
    void HNSW::insert(const int *item, int label) {
        //TODO
    }

    /**
     * input:
     * query: the vector to be queried
     * k: the number of nearest neighbors to be returned
     * 
     * output:
     * a vector of labels of the k nearest neighbors
    */
    std::vector<int> HNSW::query(const int *query, int k) {
        std::vector<int> res;
        //TODO 
        return res;
    }

}