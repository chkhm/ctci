

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

using namespace std;

using vector_int=vector<int>;
using stack_int=stack<int, vector_int>;


ostream &operator<<(ostream& os, const vector_int &v) {
    copy(v.begin(), v.end(), ostream_iterator<int>(cout, " "));
    return os;
}

stack_int make_stack(const vector_int &input_vector) {
    // with a normal stack we can also use: 
    // stack_int(deque<int>(input_vector.begin(), input_vector.end()));
    return stack_int(input_vector);
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

void sort_merge(stack_int &s) {
    
}

int main(int argc, char **argv) {
    vector_int v1 {1, 3, 2, 4};
    stack_int s1(v1);
    cout << "Before sort: ";
    cout << v1 << endl;
    sort(s1);
    cout << "After sort:  ";
    cout << v1 << endl;
    cout << s1.top() << endl;

    while (!s1.empty()) {
        cout << s1.top() << " ";
        s1.pop();
    }
    cout << endl;

    return 0;
}