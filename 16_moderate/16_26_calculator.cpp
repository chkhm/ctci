
//
// Process expression consisting of numbers +, -, *, /
// Example: 2*3+5/6*3+15 = 23.5
//
// Ommiting throwing exceptions with bad input.
//

#include <cassert>
#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <algorithm>

using namespace std;

vector<string> split(const string &s) {
    vector<string> rslt;
    string token = "";
    for (int i = 0; i < s.size(); i++) {
        if ((s[i] >= '0' && s[i] <= '9') || s[i] == '.') {
            token += s[i];
        } else {
            rslt.push_back(token);
            token = s[i];
            rslt.push_back(token);
            token = "";
        }
    }
    if (token != "") {
        rslt.push_back(token);
    }
    return rslt;
}

// Example: 2*3+5/6*3+15 = 23.5
// vals: 6 17.5
// ops : +
double process(const string &s) {
    stack<double> values;
    stack<char> ops;
    double rslt = 0.0;
    vector<string> tokens = split(s);
    copy(tokens.begin(), tokens.end(), ostream_iterator<string>(cout, " ")); cout << endl;

    if (tokens.size() == 0) {
        return rslt;
    }

    cout << tokens[0] << " ";
    double val1 = stod(tokens[0]);
    values.push(val1);
    int i = 0;
    while (i < tokens.size()-1) {
        i++;
        char op = tokens[i][0];
        i++;
        double val2 = stod(tokens[i]);
        char next_op = ' ';
        if (i+1 < tokens.size()) {
            next_op = tokens[i+1][0];
        }
        double val3 = 0.0;
        switch (op) {
            case '+': 
                if (next_op != '*' && next_op != '/') {
                    val1 = values.top(); values.pop();
                    val3 = val1 + val2;
                    cout << val1 << op << val2 << '=' << val3 << " next_op: " << next_op << endl;
                    values.push(val3);
                } else {
                    cout << "pushed: " << val2 << " next_op: " << next_op << endl;
                    values.push(val2);
                    ops.push(op);
                }
            break;
            case '-': 
                if (next_op != '*' && next_op != '/') {
                    val1 = values.top(); values.pop();
                    val3 = val1 - val2;
                    cout << val1 << op << val2 << '=' << val3 << endl;
                    values.push(val3);
                } else {
                    values.push(val2);
                    ops.push(op);
                }
            break;
            case '*': 
                val1 = values.top(); values.pop();
                val3 = val1 * val2;
                cout << val1 << op << val2 << '=' << val3 << endl;
                values.push(val3);
            break;
            case '/': 
                val1 = values.top(); values.pop();
                val3 = val1 / val2;
                cout << val1 << op << val2 << '=' << val3 << endl;
                values.push(val3);
            break;
        }
        // cout << "i: " << i << " size: " << tokens.size() << endl;
    }
    while (ops.size() > 0) {
        char op = ops.top(); ops.pop();
        double val2 = values.top(); values.pop();
        double val1 = values.top(); values.pop();
        double val3 = 0.0;
        switch (op) {
            case '+':
                val3 = val1 + val2;
                values.push(val3);
                break;
            case '-':
                val3 = val1 - val2;
                values.push(val3);
                break;
        }
    }
    rslt = values.top(); values.pop();
    return rslt;
}


int main(int argc, char **argv) {
    string expression = "2*3+5/6*3+15";

    double rslt = process(expression);
    cout << "rslt: " << rslt << endl;
    assert(rslt == 23.5);

    return 0;
}