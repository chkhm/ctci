
//
//  example { 7 1 6 } + { 5 9 2 } = 617 + 295 = 912 --> { 2 1 9 }
//


#include <iostream>
#include <list>
#include <stack>
#include <algorithm>

using namespace std;
using int_list_t = list<int>;
using iterator_t = int_list_t::const_iterator;

ostream &operator<<(ostream &o, const int_list_t &l) {
    copy(l.begin(), l.end(), ostream_iterator<int>(o, " "));
    return o;
}

int_list_t operator+(const int_list_t &l1, const int_list_t &l2) {
    iterator_t it1 = l1.begin();
    iterator_t it2 = l2.begin();
    int_list_t l_result;
    
    bool done = ((it1 == l1.end()) && (it2 == l2.end()));
    int carry = 0;
    while (!done) {
        int digit1 = 0;
        int digit2 = 0;
        if (it1 != l1.end()) { digit1 = *it1; it1++; }
        if (it2 != l2.end()) { digit2 = *it2; it2++; }
        int rslt = carry + digit1 + digit2;
        carry = rslt / 10;
        int digit = rslt % 10;
        l_result.push_back(digit);
        done = ((it1 == l1.end()) && (it2 == l2.end()));
    }
    if (carry != 0) {
        l_result.push_back(carry);
    }
    return l_result;
}

void copy(const int_list_t &l1, stack<int> s_rslt) {
    for (int v : l1) {
        s_rslt.push(v);
    }
}

// if the list holds the digits in reversed order
int_list_t sum(const int_list_t &l1, const int_list_t &l2) {
    stack<int> s1; 
    stack<int> s2;
    copy(l1, s1);
    copy(l2, s2);
    stack<int> s_rslt;
    bool done = ((s1.size() > 0) && (s2.size() > 0));
    int carry = 0;
    while (!done) {
        int digit1 = 0;
        int digit2 = 0;
        if (s1.size() > 0) { digit1 = s1.top(); s1.pop(); }
        if (s2.size() > 0) { digit2 = s2.top(); s2.pop(); }
        int rslt = carry + digit1 + digit2;
        carry = rslt / 10;
        int digit = rslt % 10;
        s_rslt.push(digit);
        done = ((s1.size() > 0) && (s2.size() > 0));
    }
    if (carry != 0) {
        s_rslt.push(carry);
    }
    int_list_t l_result;
    while (s_rslt.size() >0) {
        l_result.push_back(s_rslt.top());
        s_rslt.pop();
    }
    return l_result;
}


int main(int argc, char **argv) {

    int_list_t l1 { 7, 1, 6 };
    int_list_t l2 { 5, 9, 2 };
    int_list_t l_expected_result {2, 1, 9};

    int_list_t l_result = l1 + l2;
    cout << l_result << endl;
    assert(l_result == l_expected_result);
    return 0;
}