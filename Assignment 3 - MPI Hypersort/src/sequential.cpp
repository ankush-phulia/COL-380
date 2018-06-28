#include <bits/stdc++.h>
#include <chrono>

using namespace std;

int main(int argc, char* argv[]) {
    std::chrono::time_point<std::chrono::system_clock> startt, endd;
    ifstream f_in;
    f_in.open(argv[1]);
    int n;
    f_in >> n;
    int64_t* input;
    input = new int64_t[n];
    for (int64_t i = 0; i < n; i++) {
        f_in >> input[i];
    }
    f_in.close();
    startt = std::chrono::system_clock::now();
    sort(input, input + n);
    endd = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = endd - startt;
    cout << "Time : " << elapsed_seconds.count() << endl;
}
