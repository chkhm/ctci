
#include <cassert>
#include <iostream>
#include <algorithm>
#include <iterator>
#include <list>
#include <unordered_set>
#include <vector>

using namespace std;

using list_int_t = list<int>;
using list_it = list_int_t::iterator;
using int_set = unordered_set<int>;


ostream &operator<<(ostream &o, const list_int_t &l) {
    copy(l.begin(), l.end(), ostream_iterator<int>(o, " "));
    return o;
}

void remove_dup(list_int_t &l) {
    int_set map;
    for (auto it = l.begin(); it != l.end(); it++) {
        auto search_it = map.find(*it);
        if (search_it == map.end()) {
            map.insert(*it);
        } else {
            auto tmp = it;
            it--;
            l.erase(tmp);
        }
    }
}

void remove_dup1(list_int_t &l) {
    for (auto it1 = l.begin(); it1 != l.end(); it1++) {
        for (auto it2 = next(it1); it2 != l.end(); it2++) {
            if (*it1 == *it2) {
                auto tmp = it2;
                it2--;
                l.erase(tmp);
            }
        }
    }
}

int main(int argc, char **argv) {
    int values[] = { 3, 2, 3, 5, 7, 7, 9, 4, 3};
    int cmpVal[] = { 3, 2, 5, 7, 9, 4};
    auto val_len = sizeof(values) / sizeof(int);
    auto cmp_len = sizeof(cmpVal) / sizeof(int);
    list_int_t l(values, values + val_len);
    list_int_t cmp_l(cmpVal, cmpVal + cmp_len);
    list_int_t l1 {1, 2, 3, 4};
    list_int_t cmp_l1 {1, 2, 3, 4};

    remove_dup1(l);
    cout << l << endl;    
    assert(l == cmp_l);
    cout << "ok" << endl;

    remove_dup(l1);
    cout << l1 << endl;
    assert( l1 == cmp_l1);
    cout << "ok" << endl;

    return 0;
}