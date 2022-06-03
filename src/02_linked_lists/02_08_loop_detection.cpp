
//
// detect where the loop begins
//
// 1 -> 2 -> 3 -> 4 -> 5 -> 6 -> 7 -> 8 -> 4
// A
//                     B     
//  n = 5
//
// 1
//
// 1 -> 2 -> 3 -> 4 -> 5 -> 6 -> 7 -> 8 -> 5
//                     A
//        B
// n  =  7
//
// Logic:
//   slow runner covered  n nodes when both meet
//   fast runner covered 2n nodes when both meet
//   when slow runner enters the loop:
//     - slow runner has covered x1 nodes
//     - fast runner is ( (2x1 - x1) % x2 ) = (x1 % x2) = X1 nodes into the loop
//     - fast runner is x2 - X1 "behind" slow runner
//   fast runner catches up with a rate of 1 to slow runner
//    
//   when both runners meet:
//     - slow runner has covered x1 + x3 steps
//     - x3 = X1
//     ==> n = x1 + X1 = x1 + (x1 % x2)  ==> 5 = x1 % 4 ==> x1 = 1 or 5 or 9 or ...

#include <iostream>
#include <vector>

using namespace std;

class Node {
public:
    Node() : m_v(0), m_next(nullptr) { }
    Node(int v) : m_v(v), m_next(nullptr) {}

    void insert(Node *n);
    void erase();

    int m_v;
    Node *m_next;
};

Node *make_list(vector<int> values) {
    if (values.size() == 0) {
        return nullptr;
    }
    Node *head = new Node(values[0]);
    Node *tail = head;
    for (int i =1; i < values.size(); i++) {
        tail->m_next = new Node(values[i]);
        tail = tail->m_next;
    }
    return head;
}


Node *find_loop_start(Node *head) {
    Node *slow_runner = head;
    Node *fast_runner = head;
    fast_runner = fast_runner->m_next;
    int counter1 = 0;
    while (slow_runner != fast_runner) {
        slow_runner = slow_runner->m_next;
        fast_runner = fast_runner->m_next->m_next;
        counter1++;
    }
    int counter2 = 0;
    slow_runner = head;
    fast_runner = fast_runner->m_next;
    while (slow_runner != fast_runner) {
        slow_runner = slow_runner->m_next;
        fast_runner = fast_runner->m_next;
        counter1++;
    }
    return slow_runner;
}

int main(int argc, char **argv) {
    Node *head1 = make_list({ 1, 2, 3, 4, 5, 6, 7, 8});    
    Node *ringStart = head1->m_next->m_next->m_next; // node 4
    head1->m_next->m_next->m_next->m_next->m_next->m_next->m_next->m_next = ringStart; // node 8 -> 4

    Node *rslt = find_loop_start(head1);
    cout << "head:      " << head1->m_v << endl;
    cout << "ringstart: " << ringStart->m_v << endl;
    cout << "result:    " << rslt->m_v << endl;

    return 0;
}