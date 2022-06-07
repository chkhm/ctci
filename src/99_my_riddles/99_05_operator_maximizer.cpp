
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
	op_mult = '*',
	op_add = '+',
	op_brac_open = '(',
	op_brac_close = ')'
};

//
// Idea:
//   - multiplying is the default op (usually has the biggest impact)
//   - Always use '(a+0)' when running into a zero
//   - When running into a 1 we want to use it to increment the smaller number
//   - when we run into a sequence of ones ( 1, 1, 1, ...) we want to build as many groups of (1+1)*(1+1)*... as possible
//   - if the number of 1s in a sequence is odd, then the remaining 1 should be added to the side with the lower number (left side or right side)
//   - if a sequence of ones is interrupted by a zero then treat the zero as "non-existent", just create a (1+0+1) out of it
//  
//  Algo:
//   iterate through vector:
//     take current number (a) and next number (b)
//     if a > 1 and b > 1: use "a*"
//     if a > 1 and b = 1: 
//        get_sequence_length_of_ones
//        set b to the first number after the sequence
//        if sequence length is even: use "a*(1+1)*(1+0+1)..(1+1)*"
//        if sequence length is odd:
//           if a < b: use "(a+1)*(1+1)*(1+0+1)..(1+1)*"
//           else:     use "a*(1+1)*(1+0+1)..(1+1)*(1+b)" set b to (1+b)
//     if a = 1: 
//        get_sequence_length_of_ones
//        set b to the first number after the sequence
//        if sequence length is even: use "a*(1+1)*(1+0+1)..(1+1)*"
//        if sequence length is odd:
//           if a < b: use "(a+1)*(1+1)*(1+0+1)..(1+1)*"
//           else:     use "a*(1+1)*(1+0+1)..(1+1)*(1+b)" set b to (1+b)
//     set a to b
pair<string, double> find_max_operands(const vector<double>& values) {

	struct term_t {
		int m_start_idx;
		int m_end_idx;
		int m_value;
		string m_term;
	};

	int i = 0;
	while (i < values.size()) {

	}

	return { "", 0.0 };
}

int main(int argc, char** argv) {
	vector<double> input1 = { 3, 4, 5, 1 };
	vector<double> input2 = { 1, 1, 1, 5 };  // 2 * 6 vs 3 * 5
	vector<double> input3 = { 3, 1, 2 }; // 3 * (1 + 2) = 9
	vector<double> input4 = { 1, 1, 1, 1, 1, 2 }; //  (1 + 1) * (1 + 1) * (1 + 2) = 12
	vector<double> input5 = { 4, 1, 1, 1, 2, 1, 5 }; //  4 * (1 + 1) * (1 + 2 + 1) * 5 = 160
	vector<double> input6 = { 4, 1, 1, 1, 2, 1, 1, 5 }; //  4 * (1 + 1) * (1 + 2) * (1 + 1) * 5 = 240


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


