#pragma once
#include <bits/stdc++.h>
#include <mpi.h>

using namespace std;

struct Info {
    int comm_size;
    int proc_Id;
    int64_t *input;
    int64_t input_size;
    int64_t *proc_array;
    int64_t proc_load_size;
    int64_t *low, *high;
};

template <typename T>
void print(vector<T> &vec) {
    for (const auto &e : vec) {
        cout << e << ' ';
    }
    cout << endl;
}

template <typename T>
void sort(vector<T> &arr) {
    sort(arr.begin(), arr.end());
}
