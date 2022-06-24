

// You are in charge of designing a small, in - memory social network, with the basic functionality of adding friendship
// between two people via an AddFriendship function, and a GetSuggestedFriends function for a particular user in the
// network.The criteria is to pick someone with whom the given user has the most number of friends in common.
//
// Start by discussing the most suitable data structure, and implement all the objectsand functions.

#include <iostream>
#include <string>
#include <set>
#include <map>
#include <vector>
#include <memory>
#include <algorithm>
#include <functional>

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
	//  - maintain a "top 3" list: list_of_three
	//  - go over all persons
	//     o count overlapping friends
	//     o keep the top 3 in the list_of_three
	set_person_t suggestFriendsFor(const p_id_t &id) const {
		Person &srcPerson = *m_persons.at(id);
		set_person_t rslt;

		vector<dist_pair_t> heap(m_persons.size());
		auto it = m_persons.begin();
		for (int i = 0; i < m_persons.size(); i++) {
			if (it->first != id && it->second) {
				int overlapping_friends_count = srcPerson.countOverlappingFriends(*(it->second));
				heap[i]= {overlapping_friends_count, it->first};
				it++;
			}
		}
		make_heap(heap.begin(), heap.end());
		for (int i = 0; i < 3; i++) { 
			auto f = heap.front();
			cout << "front: " << f.first << " " << f.second << endl;
			rslt.insert(f.second);
			pop_heap(heap.begin(), heap.end());
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

	PersonCollection pc;
	for (auto p : input_persons) {
		pc.addPerson(p);
	}
	cout << "social network suggestion for q:" << endl;
	auto rslt = pc.suggestFriendsFor("q");
	for (auto x : rslt) {
		cout << x << " ";
	}
	cout << endl;

	return 0;
}

