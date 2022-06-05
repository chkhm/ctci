

//
// A person who knows the rumor will tell the rumor to every other person he/ she meets, 
// Initially, only person 1 knows the story. Given a list of meetings between people in a form of
// (person_1_id, person_2_id, timestamp) construct a list of the persons who will know the story
// at the very end.
//
// Example 1: input: {meetings = [(1, 2, 100), (3, 4, 200), (1, 3, 300), (2,5, 400)], first_person = 1}
// The events could be out of order.
// Person 2 will learn the story at the moment 100, person 3 — at the moment 300,
// person 5 — in the moment 400. Person 4 will never learn the story. So the answer is [1, 2, 3, 5].
//
// Example 2: input: {meetings = [(1, 2, 100), (2, 3, 100), (4, 5, 100)], first_person = 2}
// the first parameter is array of the Persons meet at particular timestamp, second parameter is 
// the PersonId who knows the story first.
// Output: [1, 2, 3]
//
// Solution ideas:
//
// Simple O(n log n) solution: 
//   sort by time and then parse from left to right: for each conversation partner check if 
//   already in result list
//

#include <cassert>
#include <iostream>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <utility>

using namespace std;

using timestamp_t=int;

struct Meeting {
    int person_id1;
    int person_id2;
    timestamp_t timestamp;
};

bool operator<(const Meeting &m1, const Meeting &m2) {
    return m1.timestamp < m2.timestamp;
}

using vec_meeting_t = vector<Meeting>;
using set_pid_t = unordered_set<int>;

ostream &operator<<(ostream& os, const Meeting &m) {
    os << "{ " << m.person_id1 << ", " << m.person_id2 << ", " << m.timestamp << " }";
    return os;
}

template<typename T>
ostream &operator<<(ostream& os, const vector<T> &vec) {
    os << "{ ";
    for (auto entry : vec) {
        os << entry << "  ";
    }
    os << " }";
    return os;
}

template<typename T>
ostream &operator<<(ostream& os, const unordered_set<T> &set) {
    os << "{ ";
    for (auto entry : set) {
        os << entry << "  ";
    }
    os << " }";
    return os;
}

bool set_has_one_of_them(set_pid_t &set, int p1, int p2) {
    if (set.find(p1) != set.end() || set.find(p2) != set.end()) {
        return true;
    }
    return false;
}

struct Event {
    vector<int> m_persons;
    timestamp_t timestamp;

    set_pid_t
};

//
// strategy:
//   put first_person into rslt set
//   build vector of meeting_sets happening at same timestamp
//   build a vector of events for each timestamp: 
//       iterate over meetings:
//          check for both persons if they are already in a event
//          if not create a new event with the two
//          if not melt all events into one where one of the two are participating already
//          add both to the melted single event
//          for each event: add persons to rslt if one of them is in the rslt set
//    
set_pid_t who_knows_it_sorted(const vec_meeting_t &v, int first_person) {
    set_pid_t rslt;
    rslt.insert(first_person);
    // event = meetings at the same time with overlapping persons
    unordered_map<timestamp_t, vec_meeting_t> grouped_meetings = group_by_timestamp(v); 
    vector<timestamp_t> timestamps = all_keys(grouped_meetings);
    sort(timestamps.begin(), timestamps.end());
    for (timestamp_t t : timestamps) {
        // meetings with same timestamp
        vector<Event> events = split_into_meetings(grouped_meetings[t]);
        
    }


    return rslt;
}

int main(int argc, char **argv) {
    vec_meeting_t input_vec1 { {1, 2, 100}, {3, 4, 200}, {1, 3, 300}, {2, 5, 400} };
    vec_meeting_t input_vec2 { {1, 2, 100}, {2, 3, 100}, {4, 5, 100} };
    vec_meeting_t input_vec3 { {1, 2, 100}, {4, 5, 100}, {2, 3, 100}, {2, 4, 100} };

    cout << input_vec1 << endl;
    set_pid_t rslt1 = who_knows_it_sorted(input_vec1, 1);
    cout << rslt1 << endl << endl;

    cout << input_vec2 << endl;
    set_pid_t rslt2 = who_knows_it_sorted(input_vec2, 2);
    cout << rslt2 << endl;

    return 0;
}
