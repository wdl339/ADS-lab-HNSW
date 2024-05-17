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

#include "../util/util.hpp"

namespace HNSWLab {

    class Node {
    public:
        const int *data;
        int label;
        std::unordered_map<int,std::unordered_set<Node*>> neighbors;

        Node(const int *data, int label) : data(data), label(label) {}

        void set_neighbors(std::unordered_set<Node*> neighbors, int lc) {
            this->neighbors[lc] = neighbors;
        }

        void add_neighbors(std::unordered_set<Node*> neighbors, int lc) {
            this->neighbors[lc].insert(neighbors.begin(), neighbors.end());
        }

        ~Node() {}
    };
    
    class HNSW : public AlgorithmInterface {
    public:
        int maxlevel;
        Node* enterpoint;
        int vec_dim;

        HNSW() {
            maxlevel = 10;
            enterpoint = nullptr;
            vec_dim = 128;
        }

        void insert(const int *item, int label);

        std::vector<int> query(const int *query, int k);

        std::unordered_set<Node*> search_layer(const int *q, Node* ep, int ef, int lc);

        std::unordered_set<Node*> search_neighbors(const int *q, std::unordered_set<Node*> W, int num, int lc);

        Node* get_nearest(std::unordered_set<Node*> W, const int *q);

        Node* get_furthest(std::unordered_set<Node*> W, const int *q);

        ~HNSW() {}
    };

    /**
     * input:
     * iterm: the vector to be inserted
     * label: the label(id) of the vector
    */
    void HNSW::insert(const int *item, int label) {
        std::unordered_set<Node*> W;

        if (enterpoint == nullptr) {
            enterpoint = new Node(item, label);
            return;
        }

        Node* ep = enterpoint;
        int maxL = maxlevel;

        int L = get_random_level();

        for (int lc = maxL; lc >= L + 1; lc--) {
            W = search_layer(item, ep, 1, lc);
            ep = get_nearest(W, item);
        }

        Node* q = new Node(item, label);

        for (int lc = std::min(L, maxL); lc >= 0; lc--) {
            W = search_layer(item, ep, ef_construction, lc);

            std::unordered_set<Node*> neighbors = search_neighbors(item, W, M, lc);
            q->set_neighbors(neighbors, lc);

            for (auto n : neighbors) {
                n->add_neighbors({q}, lc);
                if (n->neighbors[lc].size() > M_max) {
                    n->neighbors[lc].erase(get_furthest(n->neighbors[lc], n->data));
                }
            }

            ep = get_nearest(W, item);
        }

        if (L > maxL) {
            maxL = L;
            enterpoint = q;
        }
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

        std::unordered_set<Node*> W;

        if (enterpoint == nullptr) {
            return res;
        }

        Node* ep = enterpoint;
        int maxL = maxlevel;

        for (int lc = maxL; lc >= 0; lc--) {
            W = search_layer(query, ep, 1, lc);
            ep = get_nearest(W, query);
        }

        W = search_layer(query, ep, ef_search, 0);
        std::unordered_set<Node*> neighbors = search_neighbors(query, W, k, 0);

        for (auto n : neighbors) {
            res.push_back(n->label);
        }

        return res;
    }

    /**
     * input:
     * q: the vector to be inserted
     * ep: the enterpoint
     * lc: the level of the current layer
     * 
     * output:
     * a set of nodes
    */
    std::unordered_set<Node*> HNSW::search_layer(const int *q, Node* ep, int ef, int lc) {
        std::unordered_set<Node*> W;
        std::unordered_set<Node*> v;
        std::unordered_set<Node*> C;

        W.insert(ep);
        v.insert(ep);
        C.insert(ep);

        while (C.size() > 0) {
            Node* c = get_nearest(C, q);
            C.erase(c);
            Node* f = get_furthest(W, q);
            if (l2distance(c->data, q, vec_dim) > l2distance(f->data, q, vec_dim))
                break;

            for (auto e : c->neighbors[lc]) {
                if (v.find(e) == v.end()) {
                    v.insert(e);
                    Node* f = get_furthest(W, q);
                    if (l2distance(e->data, q, vec_dim) < l2distance(f->data, q, vec_dim) || W.size() < ef) {
                        C.insert(e);
                        W.insert(e);
                        if (W.size() > ef) {
                            W.erase(f);
                        }
                    }
                }
            }
        }

        return W;
    }

    std::unordered_set<Node*> HNSW::search_neighbors(const int *q, std::unordered_set<Node*> W, int num, int lc){
        // 在W中选择q最近邻的num个节点
        std::unordered_set<Node*> res;

        auto comp = [](std::pair<long, Node*> a, std::pair<long, Node*> b) {
            return a.first > b.first;
        };
        
        std::priority_queue<std::pair<long, Node*>, std::vector<std::pair<long, Node*>>, decltype(comp)> pq(comp);

        for (auto w : W) {
            long dis = l2distance(w->data, q, vec_dim);
            pq.push(std::make_pair(dis, w));
        }

        for (int i = 0; i < num; i++) {
            res.insert(pq.top().second);
            pq.pop();
        }

        return res;
    }

    Node* HNSW::get_nearest(std::unordered_set<Node*> W, const int *q) {
        Node* res = nullptr;
        long min_dis = -1;
        for (auto w : W) {
            long dis = l2distance(w->data, q, vec_dim);
            if (min_dis == -1) {
                min_dis = dis;
                res = w;
                continue;
            }
            if (dis < min_dis) {
                min_dis = dis;
                res = w;
            }
        }
        return res;
    }

    Node* HNSW::get_furthest(std::unordered_set<Node*> W, const int *q) {
        Node* res = nullptr;
        long max_dis = -1;
        for (auto w : W) {
            long dis = l2distance(w->data, q, vec_dim);
            if (max_dis == -1) {
                max_dis = dis;
                res = w;
                continue;
            }
            if (dis > max_dis) {
                max_dis = dis;
                res = w;
            }
        }
        return res;
    }
}