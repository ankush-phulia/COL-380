#pragma once
#include <math.h>
#include <time.h>
#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;

template <typename T>
void print(vector<T>& vec) {
    for (const auto& e : vec) {
        cout << e << ' ';
    }
    cout << endl;
}

// Fisher-Yates shuffle from http://ideone.com/3A3cv
template <class bidiiter>
bidiiter random_unique(bidiiter begin, bidiiter end, size_t num_random) {
    size_t left = std::distance(begin, end);
    while (num_random--) {
        bidiiter r = begin;
        advance(r, rand() % left);
        swap(*begin, *r);
        ++begin;
        --left;
    }
    return begin;
}
