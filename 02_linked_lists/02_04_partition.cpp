
//
//  list: 3 5 8 5 10 2 1  partition at 5
//  rslt: 3 2 1 5 5 8 10
//

#include <iostream>
#include <list>

using namespace std;

using list_int_t=list<int>;
using iterator_t = list_int_t::iterator;

ostream &operator<<(ostream &o, const list_int_t &l) {
    copy(l.begin(), l.end(), ostream_iterator<int>(o, " "));
    return o;
}

iterator_t move_element_to(list_int_t &l, iterator_t &it, iterator_t &pivot) {
    l.insert(pivot, *it);
    pivot--;
    return l.erase(it);
}

iterator_t move_element_to_end(list_int_t &l, iterator_t it) {
    l.push_back(*it);
    return l.erase(it);
}


iterator_t partition(list_int_t &list, int p) {
    iterator_t pivot = list.end();
    iterator_t it   = list.begin();
    // int count = 0;
    while (it != pivot) {
        int v = *it;
        // cout << count << ". round: v= " << v << endl;
        // cout << list << " --> ";
        if (*it == p) {
            it = move_element_to(list, it, pivot);
        } else if (*it > p) {
            it = move_element_to_end(list, it);
        } else {
            it++;
        }
        // cout << list << endl;
        // if (count++ > 10) { return pivot; }
    }
    // cout << "pivot: " << *pivot << " prev: " << *(prev(pivot)) << " succ: " << *(next(pivot)) << endl;
    return pivot;
}

bool is_partitioned(list_int_t &l, iterator_t pivot) {
    iterator_t it = l.begin();
    while (it != pivot) {
        if (*it >= *pivot) { return false; }
        it++;
    }
    while (it != l.end() && *it == *pivot) {
        it++;
    }
    while (it != l.end()) {
        if (*it <= *pivot) { return false; }
        it++;
    }
    return true;
} 

int main(int argc, char **argv) {
    list_int_t l = {3, 5, 8, 5, 10, 2, 1};
    cout << l << endl;
    iterator_t pivot = partition(l, 5);
    cout << l << endl;
    assert(is_partitioned(l, pivot));
    return 0;
}