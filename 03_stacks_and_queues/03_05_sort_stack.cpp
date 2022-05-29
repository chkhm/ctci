

//
// sort a stack. Use only one additional stack
//
// follow up: improvements if you had as many stacks as you like
//

#include <cassert>
#include <iostream>
#include <stack>
#include <vector>

using namespace std;

using vector_int=vector<int>;
using stack_int=stack<int, vector_int>;


ostream &operator<<(ostream& os, const vector_int &v) {
    
    return os;
}

stack_int make_stack(const vector_int &input_vector) {
    // with a normal stack we can also use: 
    // stack_int(deque<int>(input_vector.begin(), input_vector.end()));
    return stack_int(input_vector);
}

void sort(stack_int &s) {

}

int main(int argc, char **argv) {
    vector_int v1 {3, 2, 1};
    stack_int s1 = make_stack(v1);

    sort(s1);
    cout << s1 << endl;

    return 0;
}