
#include <utility>
#include <iostream>
#include <iomanip>
#include <cassert>
#include <vector>

using namespace std;

//
//      0 1 2 3 4 5           u o i c 6 0
//      6 7 8 9 a b           v p j d 7 1
//      c d e f g h    ==>    w q k e 8 2
//      i j k l m n           x r l f 9 3
//      o p q r s t           y s m g a 4
//      u v w x y z           z t n h b 5

// Idea: (copy approach)
//   column becomes a reverse row
//   Round #1
//      u o i c 6 0
//      1 7 8 9 a b
//      2 d e f g h
//      3 j k l m n
//      4 p q r s t
//      5 v w x y z
//   Round #2
//      u o i c 6 0
//      v p j d 7 1
//      2 8 e f g h
//      3 9 k l m n
//      4 a q r s t
//      5 b w x y z
//   Round #3
//      u o i c 6 0
//      v p j d 7 1
//      w q k e 8 2
//      3 j f l m n
//      4 p g r s t
//      5 v h x y z
//   Round #4
//      u o i c 6 0
//      v p j d 7 1
//      w q k e 8 2
//      4 p g r s t
//      5 v h x y z
//  Too complex!
// 

// for i in 0 to 5
//       for j in i to 5-i
//             tmp = m[i][j]
//             swap(tmp, m[j][5-i])
//             swap(tmp, m[5-i][5-j])
//             swap(tmp, m[5-j][i])
//             swap(tmp, m[i][j])

// const int N = 6;
//using int2d_t = int[N][N];

using int2d_t = vector<vector<int>>;

ostream& operator<<(ostream & o, const int2d_t &m) {
    int N = m.size();
    for (int i = 0; i < N; i++){
        for (int j = 0; j < N; j++) {
            o << setw(2) << m[i][j] << " ";
        }
        o << endl;
    }
    return o;
}


bool operator==(const int2d_t &m, const int2d_t &r) {
    int N = m.size();
    for (int i = 0; i < N; i++){
        for (int j = 0; j < N; j++) {
            if (m[i][j] != r[i][j]) {
                return false;
            }
        }
    }
    return true;
}

void rotate(int2d_t &m) {
    int N = m.size();
    int n = N-1;
    for (int i = 0; i < n; i++) {
        for (int j = i; j < n-i; j++) {
            int tmp = m[i][j];
            swap(tmp, m[j][5-i]);
            swap(tmp, m[5-i][5-j]);
            swap(tmp, m[5-j][i]);
            swap(tmp, m[i][j]);
        }
    }
}

int main(int argc, char **argv) {

    
    int2d_t m = {{ 0,  1,  2,  3,  4,  5},
                 { 6,  7,  8,  9, 10, 11},
                 {12, 13, 14, 15, 16, 17},
                 {18, 19, 20, 21, 22, 23},
                 {24, 25, 26, 27, 28, 29},
                 {30, 31, 32, 33, 34, 35}};
    
    int2d_t r = {{30, 24, 18, 12,  6,  0},
                 {31, 25, 19, 13,  7,  1},
                 {32, 26, 20, 14,  8,  2},
                 {33, 27, 21, 15,  9,  3},
                 {34, 28, 22, 16, 10,  4},
                 {35, 29, 23, 17, 11,  5}};
    
    int2d_t m1 = {{1}};
    int2d_t r1 = {{1}};

    rotate(m);

    cout << m << endl;
    assert(m == r);

    rotate(m1);
    cout << m1 << endl;
    assert(m1 == r1);

    return 0;
}

