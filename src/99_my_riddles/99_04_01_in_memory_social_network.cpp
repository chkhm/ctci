
#include <regex>
#include <sstream>
#include <stack>

#include "99_04_01_in_memory_social_network.h"

ostream &operator<<(ostream &o, const set_person_t &sp) {
	o << "[ ";
	bool first_round = true;
	for (auto p : sp) {
		if (first_round) {
			first_round = false;
		} else {
			o << ", ";
		}
		o << '"' << p << '"';
	}
	o << " ]";
}


ostream& operator<<(ostream& o, const Person& p) {
	o << "{ ";
	o << "id: \"" << p.id();
	o << "\", lastname: \"" << p.lastname();
	o << "\", firstname: \"" << p.firstname();
	o << "\", email: \"" << p.email();
	o << "\", friends: " << p.friends();
	o << " }";
	return o;
}

const char *RegexPersonPattern = 
R"V0G0N(\s*{\s*id\s*:\s*\"(\S*)"\s*,\s*)V0G0N"
R"V0G0N(\s*{\s*lastname\s*:\s*\"(\S*)"\s*,\s*)V0G0N"
R"V0G0N(\s*{\s*firstname\s*:\s*\"(\S*)"\s*,\s*)V0G0N"
R"V0G0N(\s*{\s*email\s*:\s*\"(\S*)"\s*,\s*)V0G0N"
R"V0G0N(\s*{\s*friends\s*:\s*\"(\S*)"\s*,\s*)V0G0N"
R"V0G0N(\s*}\s*)V0G0N";

char opposite_of(char c) {
	if (c == '[') return ']';
	if (c == ']') return '[';
	if (c == '{') return '}';
	if (c == '}') return '{';
	return c;
}

string read_json_block(istream& i) {
	ostringstream sb;
	stack<char> block_delims;
    char c;
	i >> c;
	sb << c;
	if (c != '{') throw runtime_error("missing opening '{'");
	block_delims.push(c);
	bool string_mode = false;
	while (!i.eof() && !block_delims.empty()) {
		i.get(c);
		sb.put(c);
		if (c == '"') string_mode != string_mode;
		if (!string_mode) {
			switch (c) {
				case '{':
				case '[': 
					block_delims.push(c); break;
				case '}':
				case ']':
					char bd = block_delims.top();
					if (bd != opposite_of(c)) throw runtime_error("delimeters don't match.");
					block_delims.pop();
					break;
				default: break;
			}
		}
	}
	return sb.str();
}

//
// strategy: read single char until count of opening { and closing } are matching.
//
istream& operator>>(istream& i, Person& p) {
	string clock = read_json_block(i);
	
    return i;
}


bool operator<(dist_pair_t &p1, dist_pair_t &p2) {
	return p1.first < p2.first;
}

bool operator>(dist_pair_t &p1, dist_pair_t &p2) {
	return p1.first > p2.first;
}

ostream& operator<<(ostream& o, const vector <dist_pair_t> &v) {
	for (auto p : v) {
		o << "{ " << p.first << ", " << p.second << " } ";
	}
	return o;
}

ostream &operator<<(ostream &o, const PersonCollection &pc) {
    o << "{ persons: [ ";
    auto pids = pc.persons();
    auto first_round = true;
    for (auto pid : pids) {
        if (first_round) {
            first_round = false;
        } else {
            o << ", ";
        }
        o << pc.person(pid);
    }
    o << " ] }";
}

/*
int main(int argc, char** argv) {
	vector<Person> input_persons {
		{ "q", "Kuhmuench", "Christoph", "christoph.kuhmuench@email.de", {"a", "b"}},
		{ "a", "Alastname", "Afirstname", "a@gmail.com", {"q", "c"}},
		{ "b", "Blastname", "Bfirstname", "b@gmail.com", {"c"}},
		{ "c", "Clastname", "Cfirstname", "c@gmail.com", {"a", "b"}},
		{ "d", "Dlastname", "Dfirstname", "d@gmail.com", {"a"}},
		{ "e", "Dlastname", "Efirstname", "e@gmail.com", {"b"}},
		{ "f", "Dlastname", "Ffirstname", "f@gmail.com", {"q", "a", "b"}},
		{ "g", "Dlastname", "Gfirstname", "g@gmail.com", {"f"}},
	};

	vector<dist_pair_t> expected_rslt = { {2, "f"}, {2, "c"}, {1, "e"} };

	PersonCollection pc;
	for (auto p : input_persons) {
		pc.addPerson(p);
	}
	auto rslt = pc.suggestFriendsFor("q");
	cout << "social network suggestion for q:" << endl;
	cout << rslt << endl;
	assert(rslt == expected_rslt);

	//string str = "42";
	//int x = boost::lexical_cast<int>(str);
	//cout << "x: " << x << endl;
	//
	//// boost::random::mt19937 rng;
	//boost::random::random_device rng;
	//boost::random::uniform_int_distribution<> ten(0, 10);
	//int randomNumber = ten(rng);
	//cout << "RandonNum: " << randomNumber << endl;
	//
	//boost::asio::io_service io_svc;
	//boost::asio::io_service::work worker(io_svc);
	//
	//io_svc.run();
	//
	//std::cout << "We will not see this line in console window :(" << std::endl;

	return 0;
}
*/
