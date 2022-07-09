
#include <cassert>

#include "99_04_01_in_memory_social_network.h"
#include "99_04_02_boost_http_server.h"

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

	stringstream ss;

	cout << endl;
	cout << "Test boost json:" << endl;
	ss << *pc.person("q") << endl;
	cout << "pc[\"q\"]: " << ss.str() << endl << endl;

	Person x;
	ss >> x;

	cout << "person x: " << x << endl;


	auto rslt = pc.suggestFriendsFor("q");
	cout << "social network suggestion for q:" << endl;
	cout << rslt << endl;
	assert(rslt == expected_rslt);

	const char *http_server_argv[5] = { argv[0], "0.0.0.0", "8080", ".", "1" };
	int http_server_argc = 5;
	int server_start_rslt = run_asynch_http_server(http_server_argc, http_server_argv);

	return 0;
}

