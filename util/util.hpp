#pragma once

#include "parameter.hpp"
#include <chrono>
#include <iostream>
#include <algorithm>
#include <random>

namespace HNSWLab {
    // This is a helper function to generate random levels for the inserting node
    
    std::default_random_engine level_generator_ = std::default_random_engine(0);
    
    int get_random_level() {
        std::uniform_real_distribution<double> distribution(0.0, 1.0);
        double r = -log(distribution(level_generator_)) * mult_;
        return (int) r;
    }

    long l2distance(const int *a, const int *b, int vec_dim) {
        long dis = 0;
        for (int i = 0; i < vec_dim; ++i) {
            long tmp = a[i] - b[i];
            dis += (tmp * tmp);
        }
        return dis;
    }
    
    class TimeRecord {
        std::chrono::steady_clock::time_point time_begin;
    public:
        TimeRecord() {
            time_begin = std::chrono::steady_clock::now();
        }

        float get_elapsed_time_micro() {
            std::chrono::steady_clock::time_point time_end = std::chrono::steady_clock::now();
            return (std::chrono::duration_cast<std::chrono::microseconds>(time_end - time_begin).count());
        }

        void reset() {
            time_begin = std::chrono::steady_clock::now();
        }

    };
}