#pragma once
#include <cmath>

namespace HNSWLab {
    int M = 30;
    int M_max = 30;
    int ef_construction = 100;
    int ef_search = 100;
    double mult_ = 1 / log(1.0 * M);
}