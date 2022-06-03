
//
// implement three stacks in one array
//
// idea:
//  - stack 1 grows upwards from bottom
//  - stack 2 grows downwards from top
//  - stack 3 grows bi-directional (odd upwards, even downwards) from the middle
//

#include <cassert>
#include <iostream>

using namespace std;

enum grow_type_t {
    grow_upwards,
    grow_downwards,
    grow_alternating
};

enum stack_id_t {
    stack_bottom,
    stack_top,
    stack_middle
};

class StackTriple {
    struct TrippleInfo {
        TrippleInfo(int array_size)
            : m_array(new int[array_size]), m_array_size(array_size) 
            {}

        ~TrippleInfo() {
            if (m_array != nullptr) {
                delete [] m_array;
                m_array = nullptr;
            }
            m_array_size = 0;
        }

        int* m_array;
        int  m_array_size;
        int  m_heights[3] { 0};
        int* m_starts[3] { 0 };
    };

    class Stack {
    public:
        Stack(grow_type_t grow_type, TrippleInfo &tripple_info);
        ~Stack();
        int height() const { return m_tripple_info.m_heights[m_grow_type]; }
        void push(int v);
        int top();
        int pop();
    private:
        int *top_pos();
        int *start() { return m_tripple_info.m_starts[m_grow_type]; }
        void increment_height() { m_tripple_info.m_heights[m_grow_type]++; } 
        void decrement_height() { m_tripple_info.m_heights[m_grow_type]--; }

    private:
        grow_type_t m_grow_type;
        TrippleInfo &m_tripple_info;
    };

public:
    StackTriple(int array_size);
    ~StackTriple();
    Stack &stack(stack_id_t stack_id);

private:
    TrippleInfo m_tripple_info;
    Stack m_stacks[3];    
};

StackTriple::Stack::Stack(grow_type_t grow_type, StackTriple::TrippleInfo &tripple_info)
    : m_grow_type(grow_type), m_tripple_info(tripple_info)
{
    tripple_info.m_heights[grow_type] = 0;
    switch(grow_type) {
        case grow_upwards:
            tripple_info.m_starts[grow_type] = tripple_info.m_array;
            break;
        case grow_downwards:
            tripple_info.m_starts[grow_type] = tripple_info.m_array + tripple_info.m_array_size;
            break;
        case grow_alternating:
            tripple_info.m_starts[grow_type] = tripple_info.m_array + (tripple_info.m_array_size/2);
            break;
    }
}

StackTriple::Stack::~Stack() {
    m_tripple_info.m_starts[m_grow_type] = nullptr;
    m_tripple_info.m_heights[m_grow_type] = 0;
}

int *StackTriple::Stack::top_pos() {
    int *stack_top = nullptr;
    if (height() == 0) {
        return nullptr;
    }
    switch (m_grow_type) {
        case grow_upwards:
            stack_top = start() + height();
            break;
        case grow_downwards:
            stack_top = start() - height();
            break;
        case grow_alternating:
            if (height() % 2 == 0) {
                stack_top = start() + ((height()+1)/2);
            } else {
                stack_top = start() - ((height()+1)/2);
            }
            break;
    }
    return stack_top;
}

void StackTriple::Stack::push(int v) {
    increment_height();
    int *stack_top = top_pos();
    *stack_top = v;
}

int StackTriple::Stack::pop() {
    if (height() <= 0) {
        return -9999;
    }
    int *stack_top = top_pos();
    int rslt = *stack_top;
    decrement_height();
    return rslt;
}

int StackTriple::Stack::top() {
    if (height() == 0) {
        return -9999;
    }
    int *stack_top = top_pos();
    int rslt = *stack_top;
    return rslt;
}



StackTriple::StackTriple(int array_size)
    : m_tripple_info(array_size),
      m_stacks{  {grow_upwards, m_tripple_info}, 
                 {grow_downwards, m_tripple_info}, 
                 { grow_alternating, m_tripple_info} }
{ }

StackTriple::~StackTriple()
{ }

StackTriple::Stack &StackTriple::stack(stack_id_t stack_id) {
    switch (stack_id) {
        case stack_bottom: return m_stacks[0];
        case stack_top: return m_stacks[1];
        case stack_middle: return m_stacks[2];
    }
}

int main(int argc, char **argv) {
    StackTriple st(100);

    for (int i = 1; i < 6; i++) {
        st.stack(stack_bottom).push(i);
        st.stack(stack_top).push(10*i);
        st.stack(stack_middle).push(100*i);

        int x1 = st.stack(stack_bottom).top();
        int x2 = st.stack(stack_top).top();
        int x3 = st.stack(stack_middle).top();
        cout << "[bot] x: " << x1 << " [top] x: " << x2 << " [mid] x: " << x3 << endl;
        assert(x1 == i);
        assert(x2 == 10*i);
        assert(x3 == 100*i);
    }
    cout << "Popping:" << endl;
    for (int i = 5; i > 0; i--) {
        assert(st.stack(stack_bottom).height() == i);
        assert(st.stack(stack_top).height() == i);
        assert(st.stack(stack_middle).height() == i);

        int x1 = st.stack(stack_bottom).pop();
        int x2 = st.stack(stack_top).pop();
        int x3 = st.stack(stack_middle).pop();
        cout << "[bot] x: " << x1 << " [top] x: " << x2 << " [mid] x: " << x3 << endl;

        assert(x1 == i);
        assert(x2 == 10*i);
        assert(x3 == 100*i);
    }
    assert(st.stack(stack_bottom).height() == 0);
    assert(st.stack(stack_top).height() == 0);
    assert(st.stack(stack_middle).height() == 0);

    return 0;
}