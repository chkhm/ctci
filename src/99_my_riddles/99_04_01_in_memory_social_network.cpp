

// You are in charge of designing a small, in - memory social network, with the basic functionality of adding friendship
// between two people via an AddFriendship function, and a GetSuggestedFriends function for a particular user in the
// network.The criteria is to pick someone with whom the given user has the most number of friends in common.
//
// Start by discussing the most suitable data structure, and implement all the objectsand functions.
//
// Domain discussion
// -----------------
// 
// Subjects in the domain:
// - person
// - personstore
// 
// Relationships
// - friendship
//
// Functions:
// - addPerson(person) // member of personstore
// - getPerson(id) // member of personstore
// - addFriendship(personA, personB) // member of relationshipTable
// - getSuggestedFriends(person) // member of relationshipTable
//
// Design Discussion
// -----------------
//
// static view:
//
// class BoostRestServer
//     [associated]
//         |
//         |
//         >
// class Personstore   --->
//     [aggregates] (0,n)
//         |
//         | 
//         |
// class Person
//         |
//         |
//  pairs<Person, [Person]>
//         |
//   [aggregates] (0,n)
// class RelationShipTable
//

#include <cassert>
#include <iostream>
#include <string>
#include <set>
#include <map>
#include <vector>
#include <memory>
#include <algorithm>
#include <functional>

//#include <boost/lexical_cast.hpp>
//#include <boost/random/mersenne_twister.hpp>
//#include <boost/random/random_device.hpp>
//#include <boost/random/uniform_int_distribution.hpp>

//#include <boost/asio.hpp>


using namespace std;

using p_id_t = string;
using set_person_t = set<p_id_t>;

class Person {
public:
	Person(p_id_t id, string lastname, string firstname, string email, set_person_t friends)
		: m_id(id), m_lastname(lastname), m_firstname(firstname), m_email(email), m_friends(friends)
	{}
	Person(p_id_t id, string lastname, string firstname, string email)
		: m_id(id), m_lastname(lastname), m_firstname(firstname), m_email(email)
	{}

	p_id_t id() const { return m_id; }
	string lastname() const { return m_lastname; }
	string firstname() const { return m_firstname; }
	string email() const { return m_email; }
	set_person_t friends() const { return m_friends; }
	bool isFriend(const p_id_t &p) const { return m_friends.contains( p ); }
	bool isFriend(const Person &p) const { return m_friends.contains( p.id() ); }

	void addFriend(const Person &p) { m_friends.insert(p.id()); }
	int countOverlappingFriends(const Person &p) const {
		int rslt = 0;
		for (auto f : m_friends) {
			if (p.isFriend(f)) { rslt++; }
		}
		return rslt;
	}

private:
	string m_id;
	string m_lastname;
	string m_firstname;
	string m_email;
	set_person_t m_friends;
};

using person_pointer_t = shared_ptr<Person>;
using dist_pair_t = pair<int, p_id_t>;

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

class PersonCollection {
public:
	PersonCollection() {};
	person_pointer_t addPerson(const Person &p) { 
		shared_ptr<Person> ptr_p = make_shared<Person>(p);
		m_persons[p.id()] = ptr_p; 
		return m_persons.at(p.id()); 
	}
	void erasePerson(const p_id_t &id);

	// finds the top 3 people of overlapping friends but not being friends already
	// approach:
	//  - put all persons into a vector if they are not this person or already in the person's friend list
	//  - turn the vector into a heap
	//  - copy the top three into the result list
	vector<dist_pair_t> suggestFriendsFor(const p_id_t &id) const {
		Person &srcPerson = *m_persons.at(id);
		vector<dist_pair_t> rslt;
		vector<dist_pair_t> heap;
		
		for (auto it = m_persons.begin(); it != m_persons.end(); it++) {
			if (it->first != id && !srcPerson.isFriend(it->first)) {
				int overlapping_friends_count = srcPerson.countOverlappingFriends(*(it->second));
				heap.push_back( { overlapping_friends_count, it->first } );
			}
		}
		// cout << "Before make_heap: " << heap << endl;
		make_heap(heap.begin(), heap.end());
		// cout << " After make_heap: " << heap << endl;

		for (int i = 0; i < 3; i++) { 
			auto f = heap.front();
			// cout << "front: " << f.first << " " << f.second << endl;
			rslt.push_back(f);
			pop_heap(heap.begin(), heap.end());
			// cout << " After  pop_heap: " << heap << endl;
			heap.pop_back();
		}
		return rslt;
	}

private:
	map<p_id_t, person_pointer_t> m_persons;
};

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
