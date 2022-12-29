#include "union-find.hpp"
#include<cstdlib>
#include<iostream>
#include<cassert>

void MakeSetsTest() {
    std::cout << "[TEST] Make Set Test..." << std::endl;

    UNION::UnionFind<int> uf = UNION::UnionFind<int>();
    for (int i = 0; i < 1000; ++i) {
        size_t index = uf.MakeSet();
        assert(index == i + 1);
    }

    std::cout << "[TEST] Make Set Test:\tSUCCESS" << std::endl;
}

void JoinTest() {
    std::cout << "[TEST] Make Join Test..." << std::endl;

    UNION::UnionFind<int> uf = UNION::UnionFind<int>();
    for (int i = 0; i < 22; ++i) {
        size_t index = uf.MakeSet();
    }

    for (int i = 1; i <=22; ++i) {
        if (i % 2 == 0) {
            uf.Join(2, i);
        } else {
            uf.Join(1, i);
        }
    }

    std::cout << "[TEST] Make Join Test:\tSUCCESS" << std::endl;
}


int main() {
    MakeSetsTest();
    JoinTest();
    return 0;
}