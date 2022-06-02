
//
// Riddle:
//
//  Given a list of n strings, write a function that returns true if a pair of strings with editing distance of 1 can be found.
//  
//  Example:  has_pair_with_dist_1 ({ "hello", "hllo", "blabla"})  --> true becaue one insert or delete will make "hello" and "hllo" equal.
//
//  Further questioning will lead to definition of legal edits:
//    - insert
//    - delete
//    - replace
//    - (swap)  <-- follow up question
//
//  Follow up: function allowing up to "n" edits (harder!)
//

#include <iostream>
#include <string>
#include <vector>

using namespace std;

using vec_string_t = vector<string>;

bool is_dist_1(const string &s1, const string &s2) {

    return false;
}

bool has_pair_with_dist_1(const vec_string_t &v) {
    for (int i = 0; i < v.size(); i++) {
        for (int j = i+1; j < v.size(); i++) {
            bool rslt = is_dist_1(v[i], v[j]);
            if (rslt) {
                cout << "found: " << v[i] << " --> " << v[j] << endl;
                return true;
            }
        }
    }
    return false;
}

int main(int argc, char **argv) {
    vec_string_t input_vec {
        "hello", "hllo", "potatoe", "hexlo"
    };

    return 0;
}

