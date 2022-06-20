
//
// 
// @author: Christoph Kuhmuench (christoph.kuhmuench@email.de) 2022
// 
//
// (1) Problem Description
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
// (2) Examples:
// input is{ 3,4,5,1 } -- > output: 72  3 * 4 * (5 + 1)
// input is{ 1,1,1,5 } -- > output: 15  (1 + 1 + 1) * 5
// 
// (3) Solution idea:
//   - maintain a result list of expressions - an exression represent a set of numbers connected by '+' and surrounded by '(' ')'
//   - group the input into sequences where a sequence either contains only numbers less or equal than 2 or alternatively only numbers greater or equal than 2
//   - if the sequence is of type greater than 2:  for each value in the sequence add a single value expression to the result list
//   - if the sequence is of type less or equal 2: build an expression and keep adding the values from the sequence until the sequence is greater or equal to 2: e.g.: {1, 1, 0, 1, 1, 1} --> (1+1+0), (1+1), (1)
//                                                 the last entry might be smaller than 2, that's ok for now.
// 
//   - iterate over the list of expressions: If you find one smaller or equal 2, merge it with either the left or right neighbour, depending on which is smaller
// 
// 
// (4) Follow up: How would you deal with negative numbers:
//    - a simple solution could be to turn all negative numbers positive like this:  input: { -2,3 } -- > output: 6  -(-2) * 3 = 6
//    - if this is considered cheating: if there is an even number lower than -1: keep multiplying them (minus times minus is positive); the negative larger or equal -1 are subtracted
//                                      else:                                     keep multiplying them except for the largest (closest to -1) which is subtracted; the negative larger or equal -1 are subtracted
//
// (5) Discussion of solution
// urgh, that was difficult.
// also, I'm not sure if it works with doubles, e.g. something like { 2.1, 0.1, 0.1, 2.1, 2.3 } -- > (2.1 + 0.1) * (0.1 + 2.1) * (2.3) = 11.132
//                                                                  { 2.1, 0.3, 0.3, 1.8, 0.5 } -- > (2.1 + 0.3) * (0.3 + 1.8 + 0.5)   = 6.24
//                                                                                                        2.4                2.6 = 6.24  // diff(2.4, 2.6) = 0.2 or var(2.4, 2.6) = 0.01
//                                                                                                   (2.1 + 0.3 + 0.3) * (1.8 + 0.5)   = 6.21
//                                                                                                        2.7                2.3 = 6.21  // diff(2.7, 2.3) = 0.4 or var(2.7, 2.3) = 0.04
//                                                                  so, we probably want to minimize variance for each expression
// probably need to split that sequence one at a time, always preferring the lower side
// 06/19/2022 Implemented the above discussed splitting
//
// (6) New idea for floating point numbers
// we need an algorithm that does a step-wise optimization like this: 
//    { 2.1, 0.3, 0.3, 1.8, 0.5 } --> (2.1) * (0.3 + 0.3 + 1.8) (0.5) --> (2.1) * (0.3 + 0.3 + 1.8 + 0.5) --> (2.1 + 0.3) * (0.3 + 1.8 + 0.5)
//         0. Input                        1. step: rough grouping               2. step: expr. <= 1.0           3. step: minimize variance
// 
// 
// we could implement this with a forward + backward sweep:
//   - Forward sweep: steps 1 and 2 are done as described above in section (3). 
//   - Backward sweep: if an expression is >= 2.0 when removing some numbers from its front side, remove them and balance them between this and the previous expression
//                     the balancing is done by always adding them to the lowest of the two expressions
// 06/19/2022 Implemented the above discussed splitting
// 
// to be honest: not sure if this will always be sufficient. To make sure, the forward/ backward sweeps could be repeated until nothing changes anymore. This will eventually stabilize.
//
//
// 06/19/2022 Final remarks:
// I believe this solution works now under all circumstances. Code has some room for cleanup :-)
// But since this is just a coding exercise I leave it as is.
// 
//



#include <cassert>
#include <cmath>

#include <iostream>
#include <vector>
#include <list>
#include <string>
#include <utility>
#include <algorithm>

using namespace std;

using v_t = double;
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
	is_lt_two,
	is_gte_two
};

using seq_data_t = pair<value_type_t, int>;

class Expr {
public:
	Expr(const vec_t& vec, int start_idx, int end_idx) : m_vec(vec), m_start_idx(start_idx), m_end_idx(end_idx) {}
	~Expr() {}

	void move_end_to(int new_end_idx) {
		m_end_idx = new_end_idx;
		assert(m_end_idx >= m_start_idx);
		assert(m_end_idx <= m_vec.size());
	}

	void move_start_to(int new_start_idx) {
		m_start_idx = new_start_idx;
		assert(m_start_idx <= m_end_idx);
		assert(m_start_idx >= 0);
	}

	void offset_right_end(int n) {
		m_end_idx += n;
		assert(m_end_idx >= m_start_idx);
		assert(m_end_idx <= m_vec.size());
	}

	void offset_left_end(int n) {
		m_start_idx += n;
		assert(m_start_idx <= m_end_idx);
		assert(m_start_idx >= 0);
	}

	v_t operator[](int idx) { return m_vec[m_start_idx + idx]; }
	int start_idx() const { return m_start_idx; }
	int end_idx() const { return m_end_idx;  }
	int len() const { return m_end_idx - m_start_idx; }
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
			char buf[1024];
			sprintf(buf, "%.1f", m_vec[i]);
			rslt += buf;   // std::to_string(m_vec[i]);
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
		if (v < 2) {
			return is_lt_two;
		}
		else {
			return is_gte_two;
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
			if (current_val >= 2) {
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

	void add_all_gte_two_sequence(int start_idx, int end_idx) {
		for (int i = start_idx; i < end_idx; i++) {
			m_rslt.push_back(Expr(m_input, i, i + 1));
		}
	}

	void add_all_lt_two_sequence(int start_idx, int end_idx) {
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
		seq_data_t seq_data = {is_lt_two, 0};
		// Step 1: build the expression list
		while (i < len()) {
			int start_idx = i;
			seq_data = get_next_sequence(i);
			if (seq_data.first == is_gte_two) {
				add_all_gte_two_sequence(start_idx, seq_data.second);
			}
			else {
				add_all_lt_two_sequence(start_idx, seq_data.second);
			}
			i = seq_data.second;
		}

		// Step 2: sweep forward: if expression < 2 then distribute it over its neighbours
		for (auto it = m_rslt.begin(); it != m_rslt.end(); it++) {
			if (it->sum() < 2) {
				// distribute the elements over the left and right neighbor
				while (it->len() > 0) {
					v_t pred_sum = (it != m_rslt.begin()) ? prev(it)->sum() : 99999;
					v_t next_sum = (next(it) != m_rslt.end()) ? next(it)->sum() : 99999;
					if (pred_sum <= next_sum) {
						prev(it)->move_end_to(it->start_idx() + 1);
						it->offset_left_end(1);
					}
					else {
						next(it)->move_start_to(it->end_idx()  - 1);
						it->offset_right_end(-1);
					}
				}
				auto tmp_it = prev(it);
				m_rslt.erase(it);
				it = tmp_it;
			}
		}

		// Step3: sweep backwards: if expression > 2 even when removing elements from front then move to previous expression until difference between them is minimized
		for (auto rit = m_rslt.rbegin(); rit != m_rslt.rend(); rit++) {
			v_t next_sum = (next(rit) != m_rslt.rend()) ? next(rit)->sum() : 99999;
			bool done = false;
			int move_counter = 0;
			// keep removing first element as long as we can and it makes sense
			while (!done && rit->len() > 1 && rit->sum() > next_sum) {
				rit->offset_left_end(1);
				if (rit->sum() >= 2) {
					next(rit)->offset_right_end(1);
					++move_counter;
				}
				else {
					rit->offset_left_end(-1);
					done = true;
				}
				next_sum = (next(rit) != m_rslt.rend()) ? next(rit)->sum() : 99999;
			}

			// finally minimize the delta between rit and prev, but only if something has been moved
			if (move_counter > 0 && next_sum != 99999) {
				double delta1 = abs(next_sum - rit->sum());
				rit->offset_left_end(-1);
				next(rit)->offset_right_end(-1);
				double delta2 = abs(next(rit)->sum() - rit->sum());
				if (delta1 < delta2) {
					rit->offset_left_end(1);
					next(rit)->offset_right_end(1);
				}
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


bool operator==(const pair<string, v_t>& p1, const pair<string, v_t>& p2) {
	if (p1.first != p2.first) {
		return false;
	}
	double delta = abs(p1.second - p2.second);
	double mx = max(p1.second, p2.second);
	double tolerance = mx * 0.001; // we allow for 0.1% tolerance because of floating point incorrectness
	if (delta < tolerance) {
		return true;
	}
	return false;
}

int main(int argc, char** argv) {
	vec_t input1 = { 3, 4, 5, 1 };
	vec_t input2 = { 1, 1, 1, 5 };  // 2 * 6 vs 3 * 5
	vec_t input3 = { 3, 1, 2 }; // 3 * (1 + 2) = 9
	vec_t input4 = { 1, 1, 1, 1, 1, 2 }; //  (1 + 1) * (1 + 1 + 1) * 2 = 12
	vec_t input5 = { 4, 1, 1, 1, 2, 1, 5 }; //  4 * (1 + 1 + 1) * (2 + 1) * 5 = 180
	vec_t input6 = { 4, 1, 1, 1, 2, 1, 1, 5 }; //  4 * (1 + 1 + 1) * 2 * (1 + 1) * 5 = 240
	vec_t input7 = { 2.1, 0.1, 0.1, 2.1, 2.3 }; // (2.1 + 0.1) * (0.1 + 2.1) * (2.3)
	vec_t input8 = { 2.1, 0.3, 0.3, 1.8, 0.5 }; // (2.1 + 0.3) * (0.3 + 1.8 + 0.5)

	pair<string, v_t> rslt1  = { "(3.0)*(4.0)*(5.0+1.0)", 72 };
	pair<string, v_t> rslt2  = { "(1.0+1.0+1.0)*(5.0)", 15 };
	pair<string, v_t> rslt3  = { "(3.0)*(1.0+2.0)", 9 };
	pair<string, v_t> rslt4  = { "(1.0+1.0)*(1.0+1.0+1.0)*(2.0)", 12 };
	pair<string, v_t> rslt4a = { "(1.0+1.0+1.0)*(1.0+1.0)*(2.0)", 12 };
	pair<string, v_t> rslt5  = { "(4.0)*(1.0+1.0+1.0)*(2.0+1.0)*(5.0)", 180 };
	pair<string, v_t> rslt6  = { "(4.0)*(1.0+1.0+1.0)*(2.0)*(1.0+1.0)*(5.0)", 240 };
	pair<string, v_t> rslt7  = { "(2.1+0.1)*(0.1+2.1)*(2.3)", 11.132 };
	pair<string, v_t> rslt8  = { "(2.1+0.3)*(0.3+1.8+0.5)", 6.24 };

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

	cout << "input: " << input4 << " expected: " << rslt4.first << " = " << rslt4.second << " or " << rslt4a.first << " = " << rslt4a.second << endl;
	MaxFinder f4(input4);
	auto answer4 = f4.find_max_operators();
	cout << "input: " << input4 << " --> " << answer4.first << " = " << answer4.second << endl;
	assert(answer4 == rslt4 || answer4 == rslt4a);

	MaxFinder f5(input5);
	auto answer5 = f5.find_max_operators();
	cout << "input: " << input5 << " --> " << answer5.first << " = " << answer5.second << endl;
	assert(answer5 == rslt5);

	MaxFinder f6(input6);
	auto answer6 = f6.find_max_operators();
	cout << "input: " << input6 << " --> " << answer6.first << " = " << answer6.second << endl;
	assert(answer6 == rslt6);

	cout << "input: " << input7 << " expected: " << rslt7.first << " = " << rslt7.second << endl;
	MaxFinder f7(input7);
	auto answer7 = f7.find_max_operators();
	cout << "input: " << input7 << " --> " << answer7.first << " = " << answer7.second << endl;
	assert(answer7 == rslt7);

	// will fail at the moment, needs the sweeping procedure
	cout << "input: " << input8 << " expected: " << rslt8.first << " = " << rslt8.second << endl;
	MaxFinder f8(input8);
	auto answer8 = f8.find_max_operators();
	cout << "input: " << input8 << " --> " << answer8.first << " = " << answer8.second << endl;
	assert(answer8 == rslt8);

	return 0;
}


