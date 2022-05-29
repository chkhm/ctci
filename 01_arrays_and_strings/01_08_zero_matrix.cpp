#include <iostream>
#include <iomanip>
#include <cassert>
#include <vector>

using namespace std;
using int2d_t = vector<vector<int>>;

ostream& operator<<(ostream & o, const int2d_t &m) {
    int N = m.size();
    for (int i = 0; i < N; i++){
        int M = m[i].size();
        for (int j = 0; j < M; j++) {
            o << setw(2) << m[i][j] << " ";
        }
        o << endl;
    }
    return o;
}


void zero_matrix( int2d_t &m) {
    if (m.size() < 1 || m[0].size() < 1) {
        return ;
    }
    int N = m.size();
    int M = m[0].size();
    bool zeros_in_rows[N];
    bool zeros_in_columns[N];

    memset(zeros_in_rows, 0, sizeof(zeros_in_rows));
    memset(zeros_in_columns, 0, sizeof(zeros_in_columns));

    for (int i = 0; i < N; i++){
        for (int j = 0; j < M; j++) {
            if (m[i][j] == 0) {
                zeros_in_rows[i] = true;
                zeros_in_columns[j] = true;
            }
        }
    }
    for (int i = 0; i < N; i++) {
        for (int j= 0; j < M; j++) {
            if (zeros_in_rows[i] || zeros_in_columns[j]) {
                m[i][j] = 0;
            }
        }
    }
}

int main(int argc, char **argv) {

    int2d_t m = { {  0,  1,  2,  3,  4,  5,  6,  7,  8,  9},
                  { 10, 11, 12, 13, 14, 15, 16, 17, 18, 19},
                  { 20, 21, 22, 23, 24,  0, 26, 27, 28, 29},
                  { 30, 31, 32, 33, 34, 35, 36, 37,  0, 39},
                  { 40, 41, 42, 43, 44, 45, 46, 47, 48, 49} };

    zero_matrix(m);

    cout << m << endl;

    return 0;
}