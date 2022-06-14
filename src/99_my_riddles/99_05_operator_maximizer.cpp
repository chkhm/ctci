
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
// Solution idea:
//   - maintain a result list of expressions - an exression represent a set of number connected by '+' and surrounded by '(' ')'
//   - group the input into sequences where a sequence either contains only numbers less or equal to 1 or alternatively only numbers greater than 1
//   - if the sequence is of type greater than 1:  for each value in the sequence add a single value expression to the result list
//   - if the sequence is of type less or equal 1: build an expression and keep adding the values from the sequence until the sequence is greater 1: e.g.: {1, 1, 0, 1, 1, 1} --> (1+1+0), (1+1), (1)
//                                                 the last entry might be smaller or equal to 1, that's ok for now.
// 
//   - iterate over the list of expressions: If you find one smaller or equal one, merge it with either the left or right neighbour, depending on which is smaller
// 
// 
// Follow up: How would you deal with negative numbers:
//    - a simple solution could be to turn all negative numbers positive like this:  input: { -2,3 } -- > output: 6  -(-2) * 3 = 6
//    - if this is considered cheating: if there is an even number lower than -1: keep multiplying them (minus times minus is positive); the negative larger or equal -1 are subtracted
//                                      else:                                     keep multiplying them except for the largest (closest to -1) which is subtracted; the negative larger or equal -1 are subtracted
//
// urgh, that was difficult.
// also, I'm not sure if it works with doubles, e.g. something like { 1.1, 0.1, 0.1, 1.1, 1.3 } -- > (1.1 + 0.1) * (0.1 + 1.1) * (1.3)
//                                                                  { 1.1, 0.3, 0.3, 0.8, 0.5 } -- > (1.1 + 0.3) * (0.3 + 0.8 + 0.5)
//                                                                                                        1.4                1.6 = 2.24  // diff(1.4, 1.6) = 0.2 or var(1.4, 1.6) = 0.01
//                                                                                                   (1.1 + 0.3 + 0.3) * (0.8 + 0.5)
//                                                                                                        1.7                1.3 = 2.21  // diff(1.7, 1.3) = 0.4 or var(1.7, 1.3) = 0.04
//                                                                  so, we probably want to minimize variance for each expression
// probably need to split that sequence one at a time, always preferring the lower side
//
// we need an algorithm that does a step-wise optimization like this: 
//    (1.1) * (0.3 + 0.3 + 0.8) (0.5) --> (1.1) * (0.3 + 0.3 + 0.8 + 0.5) --> (1.1 + 0.3) * (0.3 + 0.8 + 0.5)
//


#include <cassert>
#include <iostream>
#include <vector>
#include <list>
#include <string>
#include <utility>
#include <algorithm>

using namespace std;

using v_t = int;
using vec_t = vector<v_t>;


template<typename T>
ostream& operator<<(ostream& os, const vector<T>& vec) {
	os << "{ ";
	for (auto entry : vec) {
		os << entry << "  ";
	}
	os << " }";
	return os;
}

enum operator_t {
	op_mult = '*',
	op_add = '+',
	op_br_open = '(',
	op_br_close = ')'
};

ostream& operator<<(ostream& os, operator_t op) {
	switch (op) {
	case op_mult: os << '*'; break;
	case op_add: os << '+'; break;
	}
	return os;
}

enum value_type_t {
	is_lte_one,
	is_gt_one
};

using seq_data_t = pair<value_type_t, int>;

class Expr {
public:
	Expr(const vec_t& vec, int start_idx, int end_idx) : m_vec(vec), m_start_idx(start_idx), m_end_idx(end_idx) {}
	~Expr() {}

	void move_end_to(int new_end_idx) {
		m_end_idx = new_end_idx;
		assert(m_end_idx > m_start_idx);
		assert(m_end_idx <= m_vec.size());
	}

	void move_start_to(int new_start_idx) {
		m_start_idx = new_start_idx;
		assert(m_start_idx < m_end_idx);
		assert(m_start_idx >= 0);
	}


	void offset_right_end(int n) {
		m_end_idx += n;
		assert(m_end_idx > m_start_idx);
		assert(m_end_idx <= m_vec.size());
	}

	void offset_left_end(int n) {
		m_start_idx += n;
		assert(m_start_idx < m_end_idx);
		assert(m_start_idx >= 0);
	}

	v_t operator[](int idx) { return m_vec[m_start_idx + idx]; }
	int start_idx() const { return m_start_idx; }
	int end_idx() const { return m_end_idx;  }
	v_t sum() {
		v_t total = 0;
		for (int i = m_start_idx; i < m_end_idx; i++) {
			total += m_vec[i];
		}
		return total;
	}

	string to_string() const {
		string rslt = "(";
		bool first = true;
		for (int i = m_start_idx; i < m_end_idx; i++) {
			if (first) {
				first = false;
			}
			else {
				rslt += '+';
			}
			rslt += std::to_string(m_vec[i]);
		}
		rslt += ')';
		return rslt;
	}

private:
	const vec_t& m_vec;
	int m_start_idx;
	int m_end_idx;
};

class MaxFinder {
public:
	MaxFinder(const vec_t& v)
		: m_input(v)
	{}

	int len() {
		return m_input.size();
	}

	double val(int idx) {
		return m_input[idx];
	}

	value_type_t value_type(v_t v) {
		if (v <= 1) {
			return is_lte_one;
		}
		else {
			return is_gt_one;
		}
	}

	seq_data_t get_next_sequence(int start_idx) {
		int i = start_idx;
		value_type_t seq_value_type = value_type(m_input[i]);
		i++;
		while (i < len() && value_type(val(i)) == seq_value_type) {
			i++;
		}
		return { seq_value_type, i };
	}

	list<Expr> construct_expressions(int start_idx, int end_idx) {
		list<Expr> rslt;
		v_t current_val = 0;
		int group_idx = start_idx;
		for (int i = start_idx; i < end_idx; i++) {
			current_val += m_input[i];
			if (current_val > 1) {
				rslt.push_back(Expr(m_input, group_idx, i + 1));
				current_val = 0;
				group_idx = i + 1;
			}
		}
		if (group_idx < end_idx) {
			rslt.push_back(Expr(m_input, group_idx, end_idx));
		}
		return rslt;
	}

	void add_all_gt_one_sequence(int start_idx, int end_idx) {
		for (int i = start_idx; i < end_idx; i++) {
			m_rslt.push_back(Expr(m_input, i, i + 1));
		}
	}

	void add_all_le_one_sequence(int start_idx, int end_idx) {
		v_t total_v2 = 0;
		list<Expr> l2 = construct_expressions(start_idx, end_idx);

		for (const auto &e : l2) {
			m_rslt.push_back(e);
		}
	}

	string get_result_string() {
		string rslt;

		bool first = true;
		for (const auto &e : m_rslt) {
			if (first) {
				first = false;
			}
			else {
				rslt += '*';
			}
			rslt += e.to_string();
		}
		return rslt;
	}

	v_t get_result_v() {
		if (m_rslt.size() == 0) {
			return 0;
		}
		auto it = m_rslt.begin();
		v_t rslt_v = it->sum();
		it++;
		while (it != m_rslt.end()) {
			rslt_v *= it->sum();
			it++;
		}
		return rslt_v;
	}

	pair<string, v_t> find_max_operators() {
		int i = 0;
		seq_data_t seq_data = {is_lte_one, 0};
		while (i < len()) {
			int start_idx = i;
			seq_data = get_next_sequence(i);
			if (seq_data.first == is_gt_one) {
				add_all_gt_one_sequence(start_idx, seq_data.second);
			}
			else {
				add_all_le_one_sequence(start_idx, seq_data.second);
			}
			i = seq_data.second;
		}

		for (auto it = m_rslt.begin(); it != m_rslt.end(); it++) {
			if (it->sum() <= 1) {
				v_t pred_sum = (it != m_rslt.begin()) ? prev(it)->sum() : 99999;
				v_t next_sum = (next(it) != m_rslt.end()) ? next(it)->sum() : 99999;
				auto tmp_it = prev(it);
				if (pred_sum <= next_sum) {
					prev(it)->move_end_to(it->end_idx());
				}
				else {
					next(it)->move_start_to(it->start_idx());
				}
				m_rslt.erase(it);
				it = tmp_it;
			}
		}

		string rslt_str = get_result_string();
		v_t rslt_v = get_result_v();

		return { rslt_str, rslt_v };
	}

private:
	const vec_t& m_input;
	list<Expr> m_rslt;
};


int main(int argc, char** argv) {
	vec_t input1 = { 3, 4, 5, 1 };
	vec_t input2 = { 1, 1, 1, 5 };  // 2 * 6 vs 3 * 5
	vec_t input3 = { 3, 1, 2 }; // 3 * (1 + 2) = 9
	vec_t input4 = { 1, 1, 1, 1, 1, 2 }; //  (1 + 1) * (1 + 1 + 1) * 2 = 12
	vec_t input5 = { 4, 1, 1, 1, 2, 1, 5 }; //  4 * (1 + 1 + 1) * (2 + 1) * 5 = 180
	vec_t input6 = { 4, 1, 1, 1, 2, 1, 1, 5 }; //  4 * (1 + 1 + 1) * 2 * (1 + 1) * 5 = 240


	pair<string, v_t> rslt1 = { "(3)*(4)*(5+1)", 72 };
	pair<string, v_t> rslt2 = { "(1+1+1)*(5)", 15 };
	pair<string, v_t> rslt3 = { "(3)*(1+2)", 9 };
	pair<string, v_t> rslt4 = { "(1+1)*(1+1+1)*(2)", 12 };
	pair<string, v_t> rslt5 = { "(4)*(1+1+1)*(2+1)*(5)", 180 };
	pair<string, v_t> rslt6 = { "(4)*(1+1+1)*(2)*(1+1)*(5)", 240 };

	MaxFinder f1(input1);
	auto answer1 = f1.find_max_operators();
	cout << "input: " << input1 << " --> " << answer1.first << " = " << answer1.second << endl;
	assert(answer1 == rslt1);

	MaxFinder f2(input2);
	auto answer2 = f2.find_max_operators();
	cout << "input: " << input2 << " --> " << answer2.first << " = " << answer2.second << endl;
	assert(answer2 == rslt2);

	MaxFinder f3(input3);
	auto answer3 = f3.find_max_operators();
	cout << "input: " << input3 << " --> " << answer3.first << " = " << answer3.second << endl;
	assert(answer3 == rslt3);

	MaxFinder f4(input4);
	auto answer4 = f4.find_max_operators();
	cout << "input: " << input4 << " --> " << answer4.first << " = " << answer4.second << endl;
	assert(answer4 == rslt4);

	MaxFinder f5(input5);
	auto answer5 = f5.find_max_operators();
	cout << "input: " << input5 << " --> " << answer5.first << " = " << answer5.second << endl;
	assert(answer5 == rslt5);

	MaxFinder f6(input6);
	auto answer6 = f6.find_max_operators();
	cout << "input: " << input6 << " --> " << answer6.first << " = " << answer6.second << endl;
	assert(answer6 == rslt6);

	return 0;
}


