

// You are in charge of designing a small, in - memory social network, with the basic functionality of adding friendship
// between two people via an AddFriendship function, and a GetSuggestedFriends function for a particular user in the
// network.The criteria is to pick someone with whom the given user has the most number of friends in common.
//
// Start by discussing the most suitable data structure, and implement all the objectsand functions.

#include <iostream>
#include <string>
#include <set>
#include <map>
#include <memory>

using namespace std;

using id_t = string;
using set_person_t = set<id_t>;

class Person {
public:
	Person(id_t id, string lastname, string firstname, string email)
		: m_id(id), m_lastname(lastname), m_firstname(firstname), m_email(email)
	{}

	id_t id() const { return m_id; }
	string lastname() const { return m_lastname; }
	string firstname() const { return m_firstname; }
	string email() const {return m_email; }
	void addFriend(const Person &p) { m_friends.insert(); }
	int countOverlappingFriends(const Person &p);
	bool isFriend(const Person &p) { return m_friends.contains( p.id() ); }

private:
	string m_id;
	string m_lastname;
	string m_firstname;
	string m_email;
	set_person_t m_friends;
};

using person_pointer_t = shared_ptr<Person>;

class PersonCollection {
public:
	PersonCollection() {};
	person_pointer_t addPerson(const Person &p);
	void erasePerson(const id_t &id);

	// finds the top 3 people of overlapping friends but not being friends already
	// approach:
	//  - maintain a "top 3" list: list_of_three
	//  - go over all persons
	//     o count overlapping friends
	//     o keep the top 3 in the list_of_three
	set_person_t suggestFriendsFor(const id_t &id) const {
		Person &srcPerson = *(m_persons[id]);
		set_person_t rslt;

		for (auto p : m_persons) {
			if (p.first != id) {
				// we only check if not yet friend
				if (!srcPerson.isFriend(p.second->id()) {
					int overlapping_friends_count = srcPerson.countOverlappingFriends(p.second);
					// to be cont.
				}
			}
		}

		return rslt;
	}

private:
	map<id_t, person_pointer_t> m_persons;
};

int main(int argc, char** argv) {

	cout << "social network" << endl;

	return 0;
}

