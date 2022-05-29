
#include <iostream>
#include <list>
#include <utility>

using namespace std;

using list_int_t=list<int>;
using rslt_pair_t = pair<list_int_t::iterator, int>;

/*
rslt_pair_t find_kth_to_last_rec(list_int_t &l, list_int_t::iterator it, size_t k) {
    rslt_pair_t rslt(l.end(), 0);
    if (next(it) != l.end()) {
        rslt = find_kth_to_last_rec(l, next(it), k);
    }
    if (rslt.second == k) {
        rslt.first = it;
    }
    cout << "it: " << *it << endl;
    ++rslt.second; 
    return rslt;
}
*/

list_int_t::iterator find_kth_to_last1(list_int_t &l, int k) {
    std::function<rslt_pair_t(list_int_t::iterator)> find_kth_to_last_rec;
    find_kth_to_last_rec = 
        [&l, k, &find_kth_to_last_rec](list_int_t::iterator it) -> rslt_pair_t {
            rslt_pair_t rslt;
            rslt.first = l.end();
            rslt.second = 0;
            if (next(it) != l.end()) {
                rslt = find_kth_to_last_rec( next(it) );
            }
            if (rslt.second == k) {
                rslt.first = it;
            }
            ++rslt.second; 
            return rslt;
        };

    rslt_pair_t rslt = find_kth_to_last_rec( l.begin() );
    return rslt.first;
}

list_int_t::iterator find_kth_to_last(list_int_t &l, int k) {
    auto it1 = l.begin();
    auto it2 = l.end();
    advance(it1, k);
    if (it1 != l.end()) {
        it2 = l.begin();
        it1++;
    }
    while (it1 != l.end()) {
        it1++;
        it2++;
    }
    return it2;
}


int main(int argc, char **argv) {
    list_int_t the_list1 {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    for (int k = 0; k < 10; k++) {
        auto rslt = find_kth_to_last(the_list1, k);

        cout << "found: " << *rslt << endl;
        assert(rslt != the_list1.end());
        assert(*rslt == 10-k);
    }
    auto rslt = find_kth_to_last(the_list1, 10);
    if (rslt != the_list1.end()) {
        cout << "found: " << *rslt << endl;
    } else {
        cout << "found: <end-of-list>" << endl; 
    }
    assert(rslt == the_list1.end());

    // advance is stupid it increments on end()
    rslt = find_kth_to_last(the_list1, 11);
    if (rslt != the_list1.end()) {
        cout << "found: " << *rslt << endl;
    } else {
        cout << "found: <end-of-list>" << endl; 
    }
    assert(rslt == the_list1.end());

    return 0;
}