

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
// Example 3: input: {meetings = [ (1, 2, 100), (4, 5, 100), (2, 3, 100), (2, 4, 100) ], first_person =1}
// Output: [1, 2, 3, 4, 5]
//
// 
// solution works but should be optimized to do less copying (use ranges instead of copying)
//
//

#include <cassert>
#include <iostream>
#include <vector>
#include <list>
#include <unordered_set>
#include <unordered_map>
#include <utility>
#include <algorithm>

using namespace std;

using timestamp_t=int;
using set_pid_t = unordered_set<int>;


struct Meeting {
    set_pid_t m_persons;
    timestamp_t m_timestamp;
};

using list_meeting_t = list<Meeting>;


bool operator<(const Meeting &m1, const Meeting &m2) {
    return m1.m_timestamp < m2.m_timestamp;
}


ostream &operator<<(ostream& os, const Meeting &m) {
    os << "{ ";
    copy(m.m_persons.begin(), m.m_persons.end(), ostream_iterator<int>(os, ", "));
    os << ", " << m.m_timestamp << " }";
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
ostream& operator<<(ostream& os, const list<T>& vec) {
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

unordered_map<timestamp_t, list_meeting_t> group_by_timestamp(const list_meeting_t& v) {
    unordered_map<timestamp_t, list_meeting_t> rslt;
    for (auto m : v) {
        rslt[m.m_timestamp].push_back(m);
    }
    return rslt;
}

vector<timestamp_t> all_keys(unordered_map<timestamp_t, list_meeting_t> grouped_meetings) {
    vector < timestamp_t> rslt;
    auto key_of = [](auto pair) {return pair.first; };
    
    for (auto kv : grouped_meetings) {
        rslt.push_back(kv.first);
    }
    
    //transform(grouped_meetings.begin(), grouped_meetings.end(), rslt.begin(), key_of);
    return rslt;
}

bool are_intersecting(const set_pid_t& v1, const set_pid_t& v2) {
    bool rslt = false;
    for (auto p : v1) {
        if (v2.find(p) != v2.end()) {
            return true;
        }
    }
    return rslt;
}

set_pid_t union_of(const set_pid_t& s1, const set_pid_t& s2) {
    set_pid_t rslt;
    for (auto p : s1) {
        rslt.insert(p);
    }
    for (auto p : s2) {
        rslt.insert(p);
    }
    return rslt;
}


//
// potentialy expensive (n^2)
// search for ways to reduce
list_meeting_t merge_meetings(list_meeting_t& v) {
    list_meeting_t rslt;
    for (auto it1 = v.begin(); it1 != v.end(); it1++) {
        Meeting merged_meeting = *it1;
        for (auto it2 = next(it1); it2 != v.end(); it2++) {
            if (are_intersecting(merged_meeting.m_persons, it2->m_persons)) {
                merged_meeting.m_persons = union_of(merged_meeting.m_persons, it2->m_persons);
                auto tmp = it2;
                it2 = prev(it2);
                v.erase(tmp);
            }
        }
        rslt.push_back(merged_meeting);
    }
    return rslt;
}


// idea:
// add first person to result set
// group the meetings by timestamp
// for each group with same timestamp:
//     aggregated_meetings = merge meetings if they have at least one common person
//     for each aggregated meeting:
//          if at least one person of aggregated meeting is in result set then merge aggregated meeting persons to result set
set_pid_t who_knows_it_sorted(const list_meeting_t &v, int first_person) {
    set_pid_t rslt;
    rslt.insert(first_person);
    unordered_map<timestamp_t, list_meeting_t> grouped_meetings = group_by_timestamp(v); 
    vector<timestamp_t> timestamps = all_keys(grouped_meetings);
    sort(timestamps.begin(), timestamps.end());
    for (timestamp_t t : timestamps) {
        list_meeting_t merged_meetings = merge_meetings(grouped_meetings[t]);
        for (auto m : merged_meetings) {
            if (are_intersecting(rslt, m.m_persons)) {
                rslt = union_of(rslt, m.m_persons);
            }
        }
    }
    return rslt;
}

int main(int argc, char **argv) {
    list_meeting_t input_vec1{ {{1, 2}, 100}, {{3, 4}, 200}, {{1, 3}, 300}, {{2, 5}, 400} };
    list_meeting_t input_vec2{ {{1, 2}, 100}, {{2, 3}, 100}, {{4, 5}, 100} };
    list_meeting_t input_vec3{ {{1, 2}, 100}, {{4, 5}, 100}, {{2, 3}, 100}, {{2, 4}, 100} };

    set_pid_t expected_rslt1 = { 1, 2, 3, 5 };
    set_pid_t expected_rslt2 = { 1, 2, 3 };
    set_pid_t expected_rslt3 = { 1, 2, 3, 4, 5 };

    cout << input_vec1 << endl;
    set_pid_t rslt1 = who_knows_it_sorted(input_vec1, 1);
    cout << rslt1 << endl << endl;

    cout << input_vec2 << endl;
    set_pid_t rslt2 = who_knows_it_sorted(input_vec2, 2);
    cout << rslt2 << endl;

    cout << input_vec3 << endl;
    set_pid_t rslt3 = who_knows_it_sorted(input_vec3, 2);
    cout << rslt3 << endl;

    assert(rslt1 == expected_rslt1);
    assert(rslt2 == expected_rslt2);
    assert(rslt3 == expected_rslt3);

    return 0;
}
