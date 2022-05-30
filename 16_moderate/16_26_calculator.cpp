
//
// Process expression consisting of numbers +, -, *, /
// Example: 2*3+5/6*3+15 = 23.5
//
// Ommiting throwing exceptions with bad input.
// first idea:
//  split string into vector
//  sweep from left to right (val1 op val2) where val1 comes from stack
//  if op in "+-": 
//      if next op is in "*/":
//         push val1 and op to stacks
//      else
//         val3 = val op val2
//         push val3

// second idea:
//   split string into linked list
//   sweep from left to right twice:
//     first round: process only '*' and '/' and replace sub-term with result in linked list
//     second round: process '+' and '-' and replace sub-term with result in linked list
//     should leave only one element in list
//

#include <cassert>
#include <iostream>
#include <variant>
#include <string>
#include <vector>
#include <stack>
#include <list>
#include <algorithm>

using namespace std;

using token_t=variant<double,char>;
using vector_tokens_t = vector<token_t>;

ostream &operator<<(ostream &os, const token_t &t) {
    if (holds_alternative<double>(t)) {
        os << get<double>(t);
    } else {
        os << get<char>(t);
    }
    return os;
}

vector_tokens_t split(const string &s) {
    vector_tokens_t rslt;
    string token = "";
    for (int i = 0; i < s.size(); i++) {
        if ((s[i] >= '0' && s[i] <= '9') || s[i] == '.') {
            token += s[i];
        } else {
            rslt.push_back(stod(token));
            token = s[i];
            rslt.push_back(token[0]);
            token = "";
        }
    }
    if (token != "") {
        rslt.push_back(stod(token));
    }
    return rslt;
}

double process_expr(double val1, double val2, char op) {
    switch (op) {
        case '+': return val1 + val2; break;
        case '-': return val1 - val2; break;
        case '*': return val1 * val2; break;
        case '/': return val1 / val2; break;
    }
    return -99999.9;
}

// Example: 2*3+5/6*3+15 = 23.5
// vals: 6 17.5
// ops : +
double process(const string &s) {
    stack<double> values;
    stack<char> ops;
    double rslt = 0.0;
    vector_tokens_t tokens = split(s);

    for (auto token : tokens)  { cout << token << " "; } 
    cout << endl;

    if (tokens.size() == 0) {
        return rslt;
    }

    double val1 = get<double>(tokens[0]);
    values.push(val1);
    int i = 0;
    while (i < tokens.size()-1) {
        i++;
        char op = get<char>(tokens[i]);
        i++;
        double val2 = get<double>(tokens[i]);
        char next_op = ' ';
        if (i+1 < tokens.size()) {
            next_op = get<char>(tokens[i+1]);
        }
        double val3 = 0.0;
        switch (op) {
            case '+':
            case '-': 
                if (next_op != '*' && next_op != '/') {
                    val1 = values.top(); values.pop();
                    val3 = process_expr(val1, val2, op);
                    // cout << val1 << op << val2 << '=' << val3 << " next_op: " << next_op << endl;
                    values.push(val3);
                } else {
                    // cout << "pushed: " << val2 << " next_op: " << next_op << endl;
                    values.push(val2);
                    ops.push(op);
                }
            break;
            case '*': 
            case '/':
                val1 = values.top(); values.pop();
                val3 = process_expr(val1, val2, op);
                // cout << val1 << op << val2 << '=' << val3 << endl;
                values.push(val3);
            break;
        }
        // cout << "i: " << i << " size: " << tokens.size() << endl;
    }
    while (ops.size() > 0) {
        char op = ops.top(); ops.pop();
        double val2 = values.top(); values.pop();
        double val1 = values.top(); values.pop();
        double val3 = process_expr(val1, val2, op); // can only be '+' or '-'
        values.push(val3);
    }
    rslt = values.top(); values.pop();
    return rslt;
}

list<token_t> split_to_list(const string &s) {
    list<token_t> rslt;
    string token = "";
    for (int i = 0; i < s.size(); i++) {
        if ((s[i] >= '0' && s[i] <= '9') || s[i] == '.') {
            token += s[i];
        } else {
            rslt.push_back(stod(token));
            token = s[i];
            rslt.push_back(token[0]);
            token = "";
        }
    }
    if (token != "") {
        rslt.push_back(stod(token));
    }
    return rslt;
}

// could probably be cleaned up even more
double process_double_sweep(const string &s) {
    double result = 0.0;
    list<token_t> tokens = split_to_list(s);

    if (tokens.begin() == tokens.end()) {
        return 0.0;
    }
    auto it_val1 = tokens.begin();
    auto it_val2 = tokens.begin();
    auto it_op = next(it_val1);

    auto erase_and_replace = [&tokens, &it_val1, &it_val2, &it_op]() {
        cout << *it_val1 << " " << *it_op << " " << *it_val2 << endl; 
        double val1 = get<double>(*it_val1);
        double val2 = get<double>(*it_val2);
        char op = get<char>(*it_op);
        double val3 = process_expr(val1, val2, op);
        tokens.erase(it_val2);
        tokens.erase(it_op);
        *it_val1 = val3;        
    };

    while ( it_op != tokens.end() ) {
        char op = get<char>(*it_op);
        it_val2 = next(it_op);
        if (op == '*' || op == '/') {
            erase_and_replace();
        } else {
            it_val1 = it_val2;
        }
        it_op = next(it_val1);
    }
    it_val1 = tokens.begin();
    it_op = next(it_val1);

    cout << "loop 2" << endl;
    for (auto token : tokens)  { cout << token << " "; } 
    cout << endl;

    while (it_op != tokens.end()) {
        it_val2 = next(it_op);
        erase_and_replace();
        it_op = next(it_val1);
    }
    result = get<double>(*it_val1);
    return result;
}

int main(int argc, char **argv) {
    string expression = "2*3+5/6*3+15";

    // double rslt = process(expression);
    double rslt = process_double_sweep(expression);
    cout << "rslt: " << rslt << endl;
    assert(rslt == 23.5);

    string expression2 = "";
    rslt = process_double_sweep(expression2);
    cout << "rslt: " << rslt << endl;
    assert(rslt == 0.0);

    return 0;
}