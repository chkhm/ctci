
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

using vec_double_t = vector<double>;


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
	op_br_open = '(',
	op_br_close = ')'
};

ostream& operator<<(ostream& os, operand_t op) {
	switch (op) {
	case op_mult: os << '*'; break;
	case op_add: os << '+'; break;
	}
	return os;
}


using vec_op_t = vector<operand_t>;

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
//     if next number is end of vector: use "a" and return
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


enum value_type_t {
	is_zero,
	is_lte_one,
	is_lg_one
};

using seq_data_t = pair<value_type_t, int>;


class Term {
public:
	Term() {};
	virtual ~Term() {}
	virtual double value() const =0;
};

class TermValue : public Term {
public:
	TermValue(double v) : m_value(v) {}
	virtual ~TermValue() {}
	virtual double value() const { return m_value; }
private:
	double m_value;
};

class TermAdd : public Term {
public:
	TermAdd(Term* a, Term* b) : m_a(a), m_b(b) {};

	virtual double value() const { return m_a->value() + m_b->value(); }

private:
	Term* m_a;
	Term *m_b;
};

class TermMult : public Term {
	TermMult(Term* a, Term* b) : m_a(a), m_b(b) {};
	virtual double value() const { return m_a->value() * m_b->value(); }

private:
	Term* m_a;
	Term* m_b;
};


class MaxFinder {
public:
	MaxFinder(const vec_double_t& v)
		: m_input(v)
	{}

	int len() {
		return m_input.size();
	}

	double val(int idx) {
		return m_input[idx];
	}

	value_type_t value_type(double v) {
		if (v == 0.0) {
			return is_zero;
		}
		else if (v <= 1.0) {
			return is_lte_one;
		}
		else {
			return is_lg_one;
		}
	}

	seq_data_t get_next_sequence(int start_idx) {
		int i = start_idx;
		value_type_t seq_value_type = value_type(m_input[start_idx]);
		while (i < len() && value_type(val(i)) == seq_value_type) {
			i++;
		}
		return { seq_value_type, i };
	}

	void add_sequence(seq_data_t seq_data) {

	}

	pair<string, double> find_max_operands() {
		int i = 0;
		seq_data_t seq_data = {is_zero, 0};
		while (i < len()) {
			int start_idx = i;
			seq_data = get_next_sequence(i);
			add_sequence(seq_data);
			i = seq_data.second;
		}

		return { "", 0.0 };
	}

private:
	const vec_double_t& m_input;
	Term* m_rslt;
};














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


