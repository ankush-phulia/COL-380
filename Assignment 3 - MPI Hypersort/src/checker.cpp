#include <bits/stdc++.h>

using namespace std;

int main(int argc, char *argv[]) {
    ifstream f_in(argv[1]);
    long temp1, temp2;
    bool chk = true;
    f_in >> temp1;
    while (f_in >> temp2) {
        if (temp2 < temp1) {
            chk = false;
            break;
        } else {
            temp1 = temp2;
        }
    }
    f_in.close();
    if (chk) {
        cout << "ALL GOOD" << endl;
    } else {
        cout << "ERROR" << endl;
    }
}
