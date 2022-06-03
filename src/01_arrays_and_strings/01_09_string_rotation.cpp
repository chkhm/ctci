
//
//  waterbottlewaterbottle  <- erbottlewat
//

#include <iostream>
#include <string>
#include <cassert>

using namespace std;

bool isRotation(const string &s1, const string &s2) {

    string s3 = s1 + s1;

    size_t rslt = s3.find(s2);

    return rslt != string::npos;
}

int main(int argc, char **argv) {

    string s1 = "waterbottle";
    string s2 = "erbottlewat";

    assert(isRotation(s1, s2));

    return 0;
} 