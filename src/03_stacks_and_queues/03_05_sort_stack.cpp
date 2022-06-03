
//
// sort a stack. Use only one additional stack
//
// follow up: improvements if you had as many stacks as you like
//
// Idea:
//   move stack to temp and keep track of largest value
//   push largest found value and then the rest of the stack
//   repeat except for found largest item --> leads to second largest and so on
//

#include <cassert>
#include <iostream>
#include <limits>
#include <stack>
#include <vector>
#include <iterator>
#include <algorithm>
#include <utility>

using namespace std;

using vector_int=vector<int>;
using stack_int=stack<int, vector_int>;

stack_int make_stack(const vector_int &input_vector) {
    // with a normal stack we can also use: 
    // stack_int(deque<int>(input_vector.begin(), input_vector.end()));
    return stack_int(input_vector);
}

// not beautiful, just a quick hack to
// to make the stack printable
// the input parameter is "by value" on purpose so that the caller's stack is not emptied
vector_int copy_to_vector(stack_int input_stack) {
    vector_int rslt;
    while (input_stack.size() > 0) {
        rslt.push_back(input_stack.top());
        input_stack.pop();
    }
    return rslt;
}


ostream &operator<<(ostream& os, const vector_int &v) {
    copy(v.begin(), v.end(), ostream_iterator<int>(cout, " "));
    return os;
}

ostream &operator<<(ostream& os, const stack_int &s) {
    vector_int vec = copy_to_vector(s);
    os << vec;
    return os;
}

void sort(stack_int &s) {
    stack_int s_tmp;
    int current_max = numeric_limits<int>::min();
    int move_items = s.size();

    auto move_one_item = [&current_max](auto &s, auto &s_tmp) {
        int v = s.top();
        s.pop();
        if (v > current_max) {
            s_tmp.push(current_max);
            current_max = v;
        } else {
            s_tmp.push(v);
        }
    };
    auto move_all_items = [] (auto &s, auto &s_tmp) {
        while (s_tmp.size() > 0) {
            int v = s_tmp.top();
            s_tmp.pop();
            s.push(v);
        }
    };

    while (move_items > 0) {
        current_max = s.top();
        s.pop();
        for (int i = 1; i < move_items; i++) {
            move_one_item(s, s_tmp);
        } // for
        s.push(current_max);
        move_all_items(s, s_tmp);
        --move_items;
    } // while
}

using pair_stack_t = pair<stack_int, stack_int>;

pair_stack_t split(stack_int &s) {
    pair_stack_t rslt;
    int h = s.size()/2;
    for (int i=0; i< h; i++) {
        rslt.first.push(s.top());
        s.pop();
    }
    while (s.size() > 0) {
        rslt.second.push(s.top());
        s.pop();
    }
    return rslt;
}

void merge(stack_int &s, pair_stack_t &sub_stacks, bool ascending) {
    // need to be type specific with the lambdas, otherwise can't be assigned to comparator
    // note that the lambdas are not needed with a proper C++ compiler and -std=C++20
    auto cmp_greater = [] (int a, int b) -> bool { return a > b; };
    auto cmp_less    = [] (int a, int b) -> bool { return a < b; };
    auto comparator = (ascending) ? cmp_greater : cmp_less;
    int init_value = (ascending) ? numeric_limits<int>::min() : numeric_limits<int>::max();

    while (sub_stacks.first.size() > 0 || sub_stacks.second.size() > 0) {
        int v1 = init_value;
        int v2 = init_value;
        if (sub_stacks.first.size() > 0) { v1 = sub_stacks.first.top(); }
        if (sub_stacks.second.size() > 0) { v2 = sub_stacks.second.top(); }
        if (comparator(v1, v2)) {
            s.push(v1);
            sub_stacks.first.pop();
        } else {
            s.push(v2);
            sub_stacks.second.pop();
        }
    }
}

void merge_sort(stack_int &s, bool ascending) {
    if (s.size() > 1) {
        pair_stack_t sub_stacks = split(s);
        merge_sort(sub_stacks.first, !ascending);
        merge_sort(sub_stacks.second, !ascending);
        merge(s, sub_stacks, ascending);
    }
}

int main(int argc, char **argv) {
    vector_int v1 {1, 3, 2, 4, 6, 5, 8, 7};
    vector_int v1_rslt {1, 2, 3, 4, 5, 6, 7, 8};
    stack_int s1(v1);
    bool ascending = true;
    cout << "stack print order: left is top, right is bottom." << endl;
    cout << "Before sort: ";
    cout << s1 << endl;
    // sort(s1);
    merge_sort(s1, ascending);
    cout << "After sort:  ";
    cout << s1 << endl;
    cout << "top: " << s1.top() << endl;
    
    assert(copy_to_vector(s1) == v1_rslt);

    return 0;
}