#pragma once
#include <iostream>
#include <vector>

namespace HNSWLab {

    class AlgorithmInterface {
    public:
        virtual void insert(const int *item, int label) = 0;

        virtual std::vector<int> query(const int * query, int k)=0;

        virtual ~AlgorithmInterface() {
        }

    };

}
