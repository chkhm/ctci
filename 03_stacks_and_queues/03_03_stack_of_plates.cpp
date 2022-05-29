

//
// split stack of plates when a certain hight is reached
//
// follow up: implement pop(stack_no) which pops from the stack of given index
//

#include <cassert>
#include <iostream>
#include <stack>
#include <vector>
#include <memory>

using namespace std;

using stack_int_t = stack<int>;
using ptr_stack_t = shared_ptr<stack_int_t>;
using vec_stack_t = vector<ptr_stack_t>;

class MultiStack {
public:
    MultiStack(int max_height)
        : m_max_height(max_height)
    {
        ptr_stack_t s= make_shared<stack_int_t>();
        m_stacks.push_back(s);
    }

    void push(int v) {
        if (m_stacks.back()->size() >= m_max_height) {
            ptr_stack_t s= make_shared<stack_int_t>();
            m_stacks.push_back(s);
        }
        m_stacks.back()->push(v);
    }

    int pop() {
        int rslt = m_stacks.back()->top();
        m_stacks.back()->pop();
        if ( (m_stacks.size() > 1) && (m_stacks.back()->size() == 0) ) {
            m_stacks.pop_back();
        }
        return rslt;
    }

    int top() {
        int rslt = m_stacks.back()->top();
        return rslt;
    }

    int top(int substack_no) {
        int rslt = m_stacks[substack_no]->top();
        return rslt;
    }

    int pop(int substack_no) {
        int rslt = m_stacks[substack_no]->top();
        m_stacks[substack_no]->pop();
        if (m_stacks.size() > 1 && m_stacks[substack_no]->size() == 0) {
            auto it = m_stacks.begin() + substack_no;
            m_stacks.erase(it);
        }
        return rslt;
    }

    int count_substacks() const { return m_stacks.size(); };

private:
    int m_max_height;
    vec_stack_t m_stacks;
};

int main(int argc, char **argv) {

    MultiStack ms(5);
    assert(ms.count_substacks() == 1);

    for (int i = 0; i < 21; i++) {
        ms.push(i);
        cout << "push: " << i << " height: " << ms.count_substacks() << endl;
        assert(ms.count_substacks() == (1 + (i / 5)) );
        assert(ms.top() == i);
    }

    int s3_v = 14;
    int stack_index = 2;
    assert(ms.top(stack_index) == s3_v);

    for (int i = 1; i < 5; i++) {
        int v = ms.pop(stack_index);
        assert(v == s3_v);
        assert(ms.count_substacks() == 5);
        s3_v--;
    }

    int v = ms.pop(stack_index);
    assert(v == s3_v);
    assert(ms.count_substacks() == 4);

    return 0;
}