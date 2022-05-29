
/**
 * @brief detect if two lists intersect.
 * 
 *                        
 *                        
 *                        
 * List1:  A -> B -> C -> D -> E
 *                   ^
 *                   | 
 * List2:  F -> G -> H
 * 
 * 
 * Solution 1: 
 *  - iterate over List 1 and throw each node into a hashmap
 *  - iterate over List 2 and check if node is in hashmap
 * 
 * Solution 2:
 *  - iterate over both lists at the same time (it1, it2):
 *    and build stack of visited nodes for each of them until both nodes are equal or both iterators reached end:
 *     S1:  A B C D E
 *     S2:  F G H C D E
 * 
 *  - if both iterators reached end then no intersection
 *  - else:
 *  -   take nodes from both stacks until nodes are not equal anymore
 *      last equal node is the intersection point
 */

#include <cassert>
#include <iostream>
#include <vector>
#include <stack>

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

Node *find_tail(Node *n) {
    if (n == nullptr) {
        return nullptr;
    }
    while (n->m_next != nullptr) {
        n = n->m_next;
    }
    return n;
}

void connect(Node* connection_point, Node *list) {
    Node *tail = find_tail(list);
    tail->m_next = connection_point;
}

// idea: we put every node on a stack until either tail is reached or both are equal
Node *detectIntersection(Node *head1, Node *head2) {
    Node *intersection = nullptr;
    Node *n1 = head1;
    Node *n2 = head2;
    stack<Node*> s1;
    stack<Node*> s2;
    while (n1 != n2) {
        if (n1 != nullptr) {
            s1.push(n1);
            n1= n1->m_next;
        }
        if (n2 != nullptr) {
            s2.push(n2);
            n2 = n2->m_next;
        }
    }
    while (s1.size() > 0 && s2.size() > 0 && s1.top() == s2.top()) {
        intersection = s1.top();
        s1.pop();
        s2.pop();
    }
    return intersection;
}

ostream &operator<<(ostream &o, Node *list) {
    for (Node *n = list; n != nullptr; n = n->m_next) {
        o << n->m_v << "  ";
    }
    return o;
}

void print_rslt(Node *intersection) {
        if (intersection) {
        cout << "Intersection: " << intersection->m_v << endl;
    } else {
        cout << "Intersection: <null>" << endl;
    }
}

int main(int argc, char **argv) {
    Node *head1 = make_list({ 1, 2, 3, 4, 5});    
    Node *head2 = make_list({6, 7, 8});
    Node *node3 = head1->m_next->m_next;

    cout << "Test 1:" << endl;    
    cout << "List 1: " << head1 << endl;
    cout << "List 2: " << head2 << endl;
    
    Node *intersection = detectIntersection(head1, head2);
    print_rslt(intersection);
    assert(intersection == nullptr);    
    
    connect(node3, head2);
    cout << "Test 2:" << endl;    
    cout << "List 1: " << head1 << endl;
    cout << "List 2: " << head2 << endl;

    intersection = detectIntersection(head1, head2);
    print_rslt(intersection);
    assert(intersection == node3);
    return 0;
}