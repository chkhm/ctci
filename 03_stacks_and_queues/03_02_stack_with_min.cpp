
//
// stack with a min function
//

#include <cassert>
#include <vector>
#include <stack>
#include <iostream>

using namespace std;

class StackMin {
public:
    void push(int v) {
        if (m_values.size() == 0) {
            m_mins.push(v);
            m_values.push(v);
        } else {
            int next_min = (v < m_mins.top()) ? v : m_mins.top();
            m_mins.push(next_min);
            m_values.push(v);
        } 
    }

    int pop() {
        if (m_values.size() == 0) {
            return 9999;
        }
        int rslt = m_values.top();
        m_values.pop();
        m_mins.pop();
        return rslt;
    }

    int min() {
        if (m_values.size() == 0) {
            return 9999;
        }
        return m_mins.top();
    }

private:
    stack<int> m_values;
    stack<int> m_mins;
};


int main(int argc, char **argv) {
    vector<int> input_values = { 5, 3, 7, 7, 6, 2, 3, 1 };
    vector<int> minim_values = { 5, 3, 3, 3, 3, 2, 2, 1 };

    StackMin sm;
    assert(sm.min() == 9999);

    for (int i =0; i < input_values.size(); i++) {
        int val = input_values[i];
        int min = minim_values[i];
        sm.push(val);
        cout << "val: " << val << " min: " << min << " sm.min: " << sm.min() << endl;
        assert(sm.min() == min);
    }
    cout << "popping" << endl;
    for (int i = input_values.size()-1; i > 0; i--) {
        int val = input_values[i];
        int min = minim_values[i-1];
        int v = sm.pop();
        cout << "val: " << val << " min: " << min << " sm.min: " << sm.min() << endl;
        assert(v == val);
        assert(sm.min() == min);
    }
    int val = input_values[0];
    int min = 9999;
    int v = sm.pop();
    assert(v == val);
    assert(sm.min() == min);

    return 0;
}