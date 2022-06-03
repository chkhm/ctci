
//
// Riddle:
//
//  Given a list of n strings, write a function that returns true if a pair of strings with editing distance of 1 can be found.
//  
//  Example:  has_pair_with_dist_1 ({ "hello", "hllo", "blabla"})  --> true becaue one insert or delete will make "hello" and "hllo" equal.
//
//  "abcde", "bcdfe" --> 2
//
//  Further questioning will lead to definition of legal edits:
//    - insert
//    - delete
//    - replace
//    - (swap)  <-- follow up question
//
//  Follow up: function allowing up to "n" edits (harder!)
//

#include <cassert>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

using vec_string_t = vector<string>;

bool is_dist_1(const string &s1, const string &s2) {
    int l1 = s1.length();
    int l2 = s2.length();
    int diff_length = abs(l1 - l2);

    if (diff_length > 1) {
        return false;
    }
    
    const string *ps1, *ps2;
    if (s1.length() > s2.length()) {
        ps1 = &s1; ps2 = &s2;
    } else {
        ps1 = &s2; ps2 = &s1;
    }

    auto it_pair = mismatch(ps1->begin(), ps1->end(), ps2->begin());
    if (it_pair.first == ps1->end()) {
        return true;
    }
    if (diff_length == 0) {
        // *it_pair.second =  *it_pair.first;
        it_pair.first++; it_pair.second++;
    } else {
        // diff_len == 1
        // ps1->erase(it_pair.first)
        it_pair.first++;
    }
    it_pair = mismatch(it_pair.first, ps1->end(), it_pair.second);
    if (it_pair.first != ps1->end()) {
        return false;
    }
    return true;
}

bool has_pair_with_dist_1(const vec_string_t &v) {
    for (int i = 0; i < v.size(); i++) {
        for (int j = i+1; j < v.size(); j++) {
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
        "hello", "potatoe", "hllo", "hexlo"
    };

    bool rslt = is_dist_1("hello", "hallo");
    cout << rslt << endl;
    assert(rslt == true);
    rslt = is_dist_1("hello", "hllo");
    assert(rslt == true);
    rslt = is_dist_1("hello", "hllx");
    assert(rslt == false);
    rslt = is_dist_1("hello", "hellx");
    assert(rslt == true);

    rslt = has_pair_with_dist_1(input_vec);
    assert(rslt == true);

    return 0;
}

