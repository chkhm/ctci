
#include <iostream>
#include <string>
#include <utility> // pair
#include <cstdlib>  // abs

using namespace std;

using str_pair_t= pair<string, string>;

constexpr int max_edit_count = 2;

string remove(const string &s, int idx) {
    return s.substr(0, idx) + s.substr(idx+1);
}

string replace(const string &s, int idx, char c) {
    return s.substr(0, idx) + c + s.substr(idx+1);
}

bool one_away(const string &s1, string s2, int i, int edit_count) {
    cout << "edit_count: " << edit_count << " i = " << i << "  s1 = \"" << s1 << "\" s2 = \"" << s2 << "\"" << endl;
    if (edit_count > max_edit_count) {
        return false;
    }
    while ( i < s1.length() && s1[i] == s2[i] ) {
        i++;
    }
    // cout << "i: " << i << " s1[i] = " << s1[i] << " s2[i] = " << s2[i] << endl;
    // cout << "i:" << i << " s1.len:" << s1.length() << " s2.len:" << s2.length() << endl;
    bool rslt = true;
    // we either are through the string or there was a mismatch
    if (i < s1.length()) {
        if (i >= s2.length()) {
            return false;
        }
        // mismatch
        edit_count++;
        // we try removing the offending char from s2
        rslt = one_away(s1, remove(s2, i), i, edit_count);
        // cout << "After one_away_remove:" << rslt << endl;
        if (!rslt) {
            // removing did not work
            // we try replacing the offending char in s2 with the right one from s1
            rslt = one_away(s1, replace(s2, i, s1[i]), i, edit_count);
        }
    } else {
        // we reached the end of s1
        // check if s2 has more chars we need to cut it.
        if (i < s2.length()) {
            edit_count++;
            rslt  = one_away(s1, remove(s2, i), i, edit_count);
        }
    }
    // cout << "edit_count: " << edit_count << "rslt: " << rslt << endl;
    return rslt;
}


bool one_away(const str_pair_t &p) {
    string s1;
    string s2;

    int delta = p.first.length() - p.second.length();
    if ( abs(delta) >= max_edit_count ) {
        return false;
    }
    if (delta < 0) {
        s1 = p.first;
        s2 = p.second;
    } else {
        s1 = p.second;
        s2 = p.first;
    }
    return one_away(s1, s2, 0, 0);
}

int main(int argc, char **argv) {
    str_pair_t p1 = { "pale", "ple" };
    str_pair_t p2 = { "pales", "pale"};
    str_pair_t p3 = { "pale", "bale" };
    str_pair_t p4 = { "pale", "bake"};

    assert(one_away(p1));
    cout << "1 done" << endl;
    assert(one_away(p2));
    cout << "2 done" << endl;
    assert(one_away(p3));
    cout << "3 done" << endl;
    assert(one_away(p4));
    cout << "4 done" << endl;

    return 0;
}