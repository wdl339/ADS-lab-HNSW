#pragma once
#include <iostream>
#include <vector>
#include <unordered_set>
#include <fstream>
#include <sys/stat.h>
#include <sys/types.h>

namespace HNSWLab {

    std::vector<std::unordered_set<int>> change_gnd_type(int n_query, int topk, int *gnd) {
        std::vector<std::unordered_set<int>> res(n_query);
        for (int i = 0; i < n_query; i++) {
            for (int j = 0; j < topk; j++) {
                res[i].insert(gnd[i * topk + j]);
            }
        }
        return res;
    }

    double count_recall(int n_query, int topk, std::vector<std::vector<int>> test_gnd_l, int *gnd) {
        std::vector<std::unordered_set<int>> gnd_set = change_gnd_type(n_query, topk, gnd);
        std::vector<double> recall_l;
        double avg_recall = 0;

        for (int i = 0; i < n_query; i++) {
            int match = 0;
            int test_gnd_size = test_gnd_l[i].size();
            for (int j = 0; j < test_gnd_size; j++) {
                if (gnd_set[i].find(test_gnd_l[i][j]) != gnd_set[i].end()) { //find
                    match++;
                }
            }
            double recall = (double) match / topk;
            recall_l.push_back(recall);
            avg_recall += recall;
        }
        return avg_recall / n_query;
    }

}
