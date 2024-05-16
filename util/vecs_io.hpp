#pragma once
#include <iostream>
#include <fstream>

namespace HNSWLab {
    int *read_ivecs(int n_vec, int vec_dim, char *path) {
        std::ifstream input(path, std::ios::binary);
        int *vecs_l = new int[n_vec * vec_dim];
        for (int i = 0; i < n_vec; i++) {
            int t;
            input.read((char *) &t, 4);
            input.read((char *) (vecs_l + vec_dim * i), t * 4);
            if (t != vec_dim) {
                std::cout << "load ground truth err" << std::endl;
                return nullptr;
            }
        }
        input.close();
        return vecs_l;
    }

    int *read_bvecs(int n_vec, int vec_dim, char *path) {
        int *vecs_l = new int[n_vec * vec_dim];
        unsigned char *tmp_vecs_l = new unsigned char[vec_dim];
        std::ifstream input(path, std::ios::binary);

        for (int i = 0; i < n_vec; i++) {
            int in = 0;
            input.read((char *) &in, 4);
            if (in != vec_dim) {
                std::cout << "load query dimension error" << std::endl;
                return nullptr;
            }
            input.read((char *) tmp_vecs_l, in);
            for (int j = 0; j < vec_dim; j++) {
                vecs_l[i * vec_dim + j] = tmp_vecs_l[j];
            }

        }
        input.close();
        return vecs_l;
    }

    float *read_fvecs(int n_vec, int vec_dim, char *path) {
        float *vecs_l = new float[n_vec * vec_dim];
        float *tmp_vecs_l = new float[vec_dim];
        std::ifstream input(path, std::ios::binary);

        for (int i = 0; i < n_vec; i++) {
            int in = 0;
            input.read((char *) &in, 4);
            if (in != vec_dim) {
                std::cout << "load query dimension error" << std::endl;
                return nullptr;
            }
            input.read((char *) tmp_vecs_l, in * 4);
            for (int j = 0; j < vec_dim; j++) {
                vecs_l[i * vec_dim + j] = tmp_vecs_l[j];
            }

        }
        input.close();
        return vecs_l;
    }

}
