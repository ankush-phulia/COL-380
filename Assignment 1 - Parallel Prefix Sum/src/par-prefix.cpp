#include <bits/stdc++.h>
#include <openssl/md5.h>
#include <pthread.h>
#include <chrono>
#include <cstring>
#include <ctime>
#define thread_count 2

using namespace std;

struct bundle {
    int id;
    int size;
    int stride;
};

int *vec;
int n;

void *upsweep(/*int id, int size, int stride*/ void *args) {
    struct bundle *bndl;
    bndl = (struct bundle *)args;
    int id = bndl->id;
    int size = bndl->size;
    int stride = bndl->stride;
    for (int i = id * size; i < (id + 1) * size && (i + stride * 2 - 1 < n);
         i += stride * 2) {
        vec[i + stride * 2 - 1] += vec[i + stride - 1];
    }
}

void *downsweep(/*int id, int size, int stride*/ void *args) {
    struct bundle *bndl;
    bndl = (struct bundle *)args;
    int id = bndl->id;
    int size = bndl->size;
    int stride = bndl->stride;
    for (int i = id * size; i < (id + 1) * size && (i + stride * 2 - 1 < n);
         i += stride * 2) {
        int t = vec[i + stride - 1];
        vec[i + stride - 1] = vec[i + stride * 2 - 1];
        vec[i + stride * 2 - 1] += t;
    }
}

int main(int argc, char const *argv[]) {
    int element;

    string infile = "input.txt";  // get input file from command line
    string outfile = "output.txt";
    ifstream f_in;
    ofstream f_out;
    f_in.open(infile);

    int i = 0;
    if (f_in.is_open()) {  // parsing file and writing to vec
        f_in >> n;
        vec = new int[n];
        while (f_in >> element) {
            vec[i] = element;
            i++;
        }
    }

    // set up threads
    int d = atol(argv[1]);
    (d > n / 2) ? (d = n / 2) : (d = d);
    d = (pow(2, (int)log2(d)));

    int chunk = n / d;
    vector<pthread_t> threads(d);
    vector<struct bundle> threadargs(d);

    // set up timer, http://en.cppreference.com/w/cpp/chrono
    std::chrono::time_point<std::chrono::system_clock> start, end;
    start = std::chrono::system_clock::now();

    // algorithm courtesy Guy Bleloch, nvidia GPU gems @ developer.nvidia.com
    // upsweep
    for (int stride = 1; stride < n; stride *= 2) {
        for (int i = 0; i < d; i++) {
            // threads[i] = thread(upsweep, i, max(chunk,stride*2), stride);
            threadargs[i].id = i;
            threadargs[i].size = max(chunk, stride * 2);
            threadargs[i].stride = stride;
            pthread_create(&threads[i], NULL, upsweep, (void *)&threadargs[i]);
        }
        for (int i = 0; i < d; i++) {
            // threads[i].join();
            pthread_join(threads[i], NULL);
        }
    }

    int last = vec[n - 1];
    vec[n - 1] = 0;

    // downsweep
    for (int stride = n / 2; stride > 0; stride /= 2) {
        for (int i = 0; i < d; i++) {
            threadargs[i].id = i;
            threadargs[i].size = max(chunk, stride * 2);
            threadargs[i].stride = stride;
            pthread_create(&threads[i], NULL, downsweep,
                           (void *)&threadargs[i]);
        }
        for (int i = 0; i < d; i++) {
            // threads[i].join();
            pthread_join(threads[i], NULL);
        }
    }

    // calculate the time
    end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = end - start;

    // print output
    string output;
    for (int i = 1; i < n; i++) {
        output += to_string(vec[i]);
        output.append(" ");
        std::cout << vec[i] << " ";
    }
    output += to_string(last);
    std::cout << last << std::endl;

    f_out.open(outfile);
    // md5 computation courtesy stack overflow
    unsigned char md5[MD5_DIGEST_LENGTH];
    int size = (output.length());
    unsigned char *temp = new unsigned char[size];
    copy(output.begin(), output.end(), temp);

    f_out << "Threads: " << d << std::endl;
    f_out << "Time: " << elapsed_seconds.count() << std::endl;
    MD5(temp, size, md5);

    // write to md5 array for printing
    char buf[32];
    string s;
    for (int i = 0; i < sizeof(md5); i++) {
        sprintf(buf, "%02x", md5[i]);
        s.append(buf);
    }

    f_out << "MD5 Sum: " << s << std::endl;

    return 0;
}
