#include <iostream>
#include <fstream>
#include "src/hnsw.hpp"
#include "util/util.hpp"
#include "util/vecs_io.hpp"
#include "util/ground_truth.hpp"
#include "util/parameter.hpp"
#include <thread>
#include <mutex>

using namespace std;
using namespace HNSWLab;

int main() {

    {
        printf("single thread\n");
        printf("load ground truth\n");
        int gnd_n_vec = 100;
        int gnd_vec_dim = 10;
        char *path = "./data/siftsmall/gnd.ivecs";
        int *gnd = read_ivecs(gnd_n_vec, gnd_vec_dim, path);

        printf("load query\n");
        int query_n_vec = 100;
        int query_vec_dim = 128;
        path = "./data/siftsmall/query.bvecs";
        int *query = read_bvecs(query_n_vec, query_vec_dim, path);

        printf("load base\n");
        int base_n_vec = 10000;
        int base_vec_dim = 128;
        path = "./data/siftsmall/base.bvecs";
        int *base = read_bvecs(base_n_vec, base_vec_dim, path);

        HNSW hnsw;

        printf("inserting\n");
        double single_insert_time;
        TimeRecord insert_record;

        for (int i = 0; i < base_n_vec; i++) {
            hnsw.insert(base + base_vec_dim * i, i);
        }

        single_insert_time = insert_record.get_elapsed_time_micro() / base_n_vec * 1e-3;
        printf("single insert time %.1f ms\n", single_insert_time);

        printf("querying with single thread\n");
        vector <vector<int>> test_gnd_l;
        double single_query_time;
        TimeRecord query_record;
        for (int i = 0; i < gnd_n_vec; i++) {
            vector<int> test_gnd = hnsw.query(query + i * query_vec_dim, gnd_vec_dim);
            test_gnd_l.push_back(test_gnd);
        }
        single_query_time = query_record.get_elapsed_time_micro() / query_n_vec * 1e-3;

        double recall = count_recall(gnd_n_vec, gnd_vec_dim, test_gnd_l, gnd);
        printf("With single thread: average recall: %.3f, single query time %.1f ms\n\n", recall, single_query_time);
    }

    {
        printf("multi-thread\n");
        printf("load ground truth\n");
        int gnd_n_vec = 100;
        int gnd_vec_dim = 10;
        char *path = "./data/siftsmall/gnd.ivecs";
        int *gnd = read_ivecs(gnd_n_vec, gnd_vec_dim, path);

        printf("load query\n");
        int query_n_vec = 100;
        int query_vec_dim = 128;
        path = "./data/siftsmall/query.bvecs";
        int *query = read_bvecs(query_n_vec, query_vec_dim, path);

        printf("load base\n");
        int base_n_vec = 10000;
        int base_vec_dim = 128;
        path = "./data/siftsmall/base.bvecs";
        int *base = read_bvecs(base_n_vec, base_vec_dim, path);

        HNSW hnsw;

        printf("inserting\n");
        double single_insert_time;
        TimeRecord insert_record;

        for (int i = 0; i < base_n_vec; i++) {
            hnsw.insert(base + base_vec_dim * i, i);
        }

        single_insert_time = insert_record.get_elapsed_time_micro() / base_n_vec * 1e-3;
        printf("single insert time %.1f ms\n", single_insert_time);

        printf("querying with multi-thread\n");
        vector <vector<int>> test_gnd_l(gnd_n_vec);
        double single_query_time;
        TimeRecord query_record;

        vector<thread> threads;

        for (int i = 0; i < gnd_n_vec; i++) {
            threads.push_back(thread([&hnsw, &query, &test_gnd_l, i, query_vec_dim, gnd_vec_dim](){
                vector<int> test_gnd = hnsw.query(query + i * query_vec_dim, gnd_vec_dim);
                test_gnd_l[i] = test_gnd;
            }));
        }

        for (auto &t : threads) {
            t.join();
        }

        single_query_time = query_record.get_elapsed_time_micro() / query_n_vec * 1e-3;

        double recall = count_recall(gnd_n_vec, gnd_vec_dim, test_gnd_l, gnd);
        printf("With multi-thread: average recall: %.3f, single query time %.1f ms\n", recall, single_query_time);
    }

    return 0;
}