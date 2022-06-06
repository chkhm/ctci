
//
// write a function that takes an array of values as input and chooses the operators that aggregate the values
// so that they maximize the outcome
// 
// Input: array of floating point numbers
// Output: operators yielding the highest result + result of calculation
//
// Conditions:
//  The following are all possible operands: *, +, -, and () parenthesis 
// 
//
// Ex:
// input is{ 3,4,5,1 } -- > output: 72  3 * 4 * (5 + 1)
// input is{ 1,1,1,5 } -- > output: 15  (1 + 1 + 1) * 5
// 
// Follow up: How would you deal with negative numbers
//

#include <cassert>
#include <iostream>
#include <vector>
#include <string>
#include <utility>

using namespace std;

template<typename T>
ostream& operator<<(ostream& os, const vector<T>& vec) {
	os << "{ ";
	for (auto entry : vec) {
		os << entry << "  ";
	}
	os << " }";
	return os;
}



enum operand_t {
	mult = '*',
	add = '+',
	brac_open = '(',
	brac_close = ')'
};

pair<string, double> find_max_operands(const vector<double>& values) {

	return { "", 0.0 };
}

int main(int argc, char** argv) {
	vector<double> input1 = { 3, 4, 5, 1 };
	vector<double> input2 = { 1, 1, 1, 5 };  // 2 * 6 vs 3 * 5
	vector<double> input3 = { 3, 1, 2 }; // 3 * (1 + 2) = 9
	vector<double> input4 = { 1, 1, 1, 1, 1, 2 }; //  (1 + 1) * (1 + 1) * (1 + 2) = 12
	pair<string, double> rslt1 = { "3*4*(5+1)", 72 };
	pair<string, double> rslt2 = { "(1+1+1)*5", 15 };

	auto answer1 = find_max_operands(input1);
	cout << "input: " << input1 << " --> " << answer1.first << " = " << answer1.second << endl;
	assert(answer1 == rslt1);

	auto answer2 = find_max_operands(input2);
	cout << "input: " << input2 << " --> " << answer2.first << " = " << answer2.second << endl;
	assert(answer2 == rslt2);

	return 0;
}


