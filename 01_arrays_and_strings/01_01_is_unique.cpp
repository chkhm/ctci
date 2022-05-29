
#include <iostream>
#include <string>
#include <unordered_set>
#include <algorithm>

// not recommended but to speed up the writing...
using namespace std;

bool is_unique0(const string &s) {
    unordered_set<char> dict;
    for (auto c : s) {
        if (dict.contains(c)) {
            return false;
        } else {
            dict.insert(c);
        }
    }
    return true;
}

bool is_unique(const string &s) {
    bool dict[256] = {false};

    if (s.length() > 256) {
        return false;
    }

    for (auto c : s) {
        if (dict[c]) {
            return false;
        } else {
            dict[c] = true;
        }
    }
    return true;
}

bool is_unique2(const string &s) {
    string s1 = s;
    sort(s1.begin(), s1.end());
    for (int i=1; i < s1.length(); i++) {
        if (s1[i] == s1[i-1]) {
            return false;
        }
    }
    return true;
}

int main (int argc, char **argv) {

    string s1 = "abc";
    string s2 = "abca";
    string s3 = "";

    assert(is_unique(s1));
    assert(is_unique(s2) == false);
    assert(is_unique(s3));

    cout << "all good" << endl;

    return 0;
}