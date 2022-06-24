
//
// Riddle Name: Continental Divide or Water Divide
// -----------------------------------------------
// 
// The Continental Divide or Water Divide is defined as those places on an elevation map where water flows in either of two oceans.
// For the United States the oceans would be the Pacific and the Atlantic.
// 
// I found this a particularly difficult riddle for a CI to be solved on a whiteboard.
//
// I admit that I failed that one and did not get the job. I do find the question
// too difficult for a 1h session.
//
// The Riddle:
// -----------
// Given an elevation map (as a 2D matrix) where each value represents the elevation at the given (x,y) coordinate,
// calculate the water divide where the water flows either to the top/right side or the bottom/ left side.
//
// For simplicity water only flows left/ right or up/ down, not diagnoally
// Water always flows down all sides that are same hight or lower
//
// Example:
// --------
// 
//           0 1 2 3 4 5 6 7         0 1 2 3 4 5 6 7 
// 
//             Pacific
//    0   A  1 2 2 3 1 2 2 3  P      x x x x u u u u   0
//    1   t  2 4 2 2 3 5 3 1  a      x x x x x x u u   1
//    2   l  1 5 2 6 4 2 3 1  c      d x x x x v u u   2
//    3   a  1 3 5 7 7 3 4 2  i  ==> d d x x x v u u   3
//    4   n  2 2 4 6 8 4 2 1  f      d d d d x u u u   4
//    5   t  1 3 2 5 6 7 7 1  i      d d d d d x x u   5
//    6   i  3 2 2 2 2 3 2 7  c      d d d d d x x x   6
//    7   c  1 3 2 1 1 3 2 2         d d d d d x x x   7
//            Atlantic
// 
//  points marked with u flow up or right. points marked with d flow down or left.
//  points marked with v flow nowhere (valley)
//  points marked with x flow either u or d. (i.e. they are the divide points)

//
// Improvement ideas:
//   I think that breadth search rather than depth search would more efficient
//
// Style: 
//  - I tried to make this very "functional" rather than object-oriented
//    so the functions have a lot of parameters
//    style could be improved by making them lambdas and capture the re-used variables
//
//  - too many ugly if statements with complicated logic
//    there must be a way to simplify this ugliness
//  - maybe I should have avoided the negative coordinates (representing ocean)
//    in the end they are a reason for many of the if statements
//

#include <cassert>
#include <iostream>
#include <vector>
#include <list>
#include <algorithm>
#include <utility>

using namespace std;

enum point_t {
    land=-1,
    unkown=-1,
    valley=0,
    up_right=1,
    down_left=2,
    divide=3
};

using vec_t = vector<int>;
using vec2d_t=vector<vec_t>;
using vec_point_t = vector<point_t>;
using vec_point2d_t = vector<vec_point_t>;
using vec_bool_t = vector<bool>;
using vec_bool2d_t = vector<vec_bool_t>;
using coord_t= pair<int, int>;
using list_coord_t= list<coord_t>;

int height(const vec2d_t &v) {
    return v.size();
}
int width(const vec2d_t &v) {
    return v[0].size();
}
int row(const coord_t &p) {
    return p.first;
}
int col(const coord_t &p) {
    return p.second;
}


ostream &operator<<(ostream &os, const vec_t& v) {
    copy(v.begin(), v.end(), ostream_iterator<int>(os, " "));
    return os;
}

// could probably be templated
ostream &operator<<(ostream &os, const vec2d_t& v2d) {
    for (const auto &v : v2d) {
        os << v << endl;
    }
    return os;
}

ostream &operator<<(ostream &os, const vec_point_t& v) {
    for (auto p : v) {
        switch(p) {
            case unkown:     os << '?'; break;
            case valley:     os << 'v'; break;
            case up_right:   os << 'u'; break;
            case down_left:  os << 'd'; break;
            case divide:     os << 'x'; break;
            default:         os << '$'; break; // should never be reached.
        }
        os << ' ';
    }
    return os;
}

ostream &operator<<(ostream &os, const vec_point2d_t& v2d) {
    for (const auto &v : v2d) {
        os << v << endl;
    }
    return os;
}

ostream &operator<<(ostream &os, const coord_t &c) {
    os << '{' << c.first << ", " << c.second << '}';
    return os;
}

ostream &operator<<(ostream &os, const list_coord_t &c) {
    os << "{ ";
    for (auto it=c.begin(); it != c.end(); it++) {
        os << *it << " ";
    }
    os << " }";
    return os;
}

point_t get_point_type(const vec2d_t &v, coord_t p) {
    if (row(p) < 0 && col(p) < 0) { 
        cerr << "ERROR: row:" << row(p) << " col: " << col(p) << endl; 
        exit(-1); 
    }
    if (row(p) >= height(v) && col(p) >= width(v)) { 
        cerr << "ERROR: row:" << row(p) << " col: " << col(p) << endl; 
        exit(-1); 
    }
    if (row(p) < 0) { return up_right; }
    if (row(p) >= height(v)) { return down_left; }
    if (col(p) < 0) { return down_left; }
    if (col(p) >= width(v)) { return up_right; }
    return land;
}

list_coord_t get_possible_neighbours(const vec2d_t &v, vec_bool2d_t &util_matrix, coord_t start_point) {
    list_coord_t rslt;
    list_coord_t final_rslt;

    // we don't have neighbours for an ocean point
    if (row(start_point) < 0 || col(start_point) < 0 || 
        row(start_point) >= height(v) || col(start_point) >= width(v)) {
        return rslt;
    }

    vector<coord_t> potential_neighbours = { 
        { row(start_point)-1, col(start_point) },
        { row(start_point)+1, col(start_point) }, 
        { row(start_point), col(start_point)-1 }, 
        { row(start_point), col(start_point)+1 } 
    };

    for (auto n : potential_neighbours) {
        if (row(n) < -1 || col(n) < -1 || row(n) > height(v) || col(n) > width(v)) {
            // do nothing: we don't go deeper into the ocean
        } else if ((row(n) < 0 && col(n) < 0 ) || (row(n) >= height(v) && col(n) >= width(v))) {
            // do nothing: we don't go to (-1,-1) or (height+1, width+1)
        } else if (row(n) == -1 || col(n) == -1 || row(n) == height(v) || col(n) == width(v)) {
            rslt.push_back(n);
        } else if (!util_matrix[row(n)][col(n)]) {
            if (v[row(start_point)][col(start_point)] >= v[row(n)][col(n)]) {
                rslt.push_back(n);
            }
        }
    }

    // experimenting with re-ordering the neighbours to have oceans first
    // return rslt;

    // the loop makes the neighbours that go into the ocean come first
    // this should improve the performance
    /* */
    for (auto it= rslt.begin(); it != rslt.end(); it++) {
        if (row(*it) < 0 || col(*it) < 0 || row(*it) >= height(v) || col(*it) >= width(v)) {
            final_rslt.push_front(*it);
        } else {
            final_rslt.push_back(*it);
        }
    }
    return final_rslt;
    /* */
}

// this is a simple complexity checker to see if the number goes down with some tweaking of the algo
static int global_count = 0;
bool recursive_find(const vec2d_t &v, vec_point2d_t &r, vec_bool2d_t &util_matrix, point_t ocean, coord_t start_point) {
    global_count++;

    point_t p_type = get_point_type(v, start_point);
    if (p_type == ocean) { return true; }
    if (p_type == land) {
        // profit from history
        point_t r_pt = r[row(start_point)][col(start_point)];
        if (r_pt == divide || r_pt == ocean) {
            return true;
        }
        util_matrix[row(start_point)][col(start_point)] = true; // mark as visited
    }
    list_coord_t neighbours = get_possible_neighbours(v, util_matrix, start_point);
    for (auto coord : neighbours) {
        bool rslt = recursive_find(v, r, util_matrix, ocean, coord);
        if (rslt) { return true; }
    }
    return false;
}

//
// This function searches for a path to one ocean
// It returns true if found and false otherwise
//
// idea:
//     - create a util matrix marking off visited nodes
//     - recursive algo:
//         if (point is ocean) { return true }
//         if (point is visited node or wrong ocean) {return false }
//         mark point as visited in util matrix
//         coord_list = get_all_possible_neighbors(start_point)
//         for each point in coord_list:
//              rslt = recursive_call with point
//              if rslt is true: return true
//         return false
bool find_path_to_ocean(const vec2d_t &v, vec_point2d_t &r, point_t ocean, coord_t start_point) {
    vec_bool2d_t util_matrix(height(v), vec_bool_t(width(v), false));
    bool rslt = recursive_find(v, r, util_matrix, ocean, start_point);
    return rslt;
}

// 
// idea:
//  - iterate over all points and call find_path_to_oceans
//
vec_point2d_t mark_water_divide(const vec2d_t &v) {
    vec_point2d_t rslt(height(v), vec_point_t(width(v), unkown));
    for (int row = 0; row < height(v); row++) {
        for (int col = 0; col < width(v); col++) {
            coord_t c {row, col};
            bool path_to_up_ocean_exists = find_path_to_ocean(v, rslt, up_right, c );
            bool path_to_down_ocean_exists = find_path_to_ocean(v, rslt, down_left, c);
            if (path_to_up_ocean_exists && path_to_down_ocean_exists) {
                rslt[row][col] = divide;
            } else if (path_to_up_ocean_exists) {
                rslt[row][col] = up_right;
            } else if (path_to_down_ocean_exists) {
                rslt[row][col] = down_left;
            } else {
                rslt[row][col] = valley;
            }
        }
    }
    return rslt;
}

int main(int argc, char **argv) {
    vec2d_t v = {
      { 1, 2, 2, 3, 1, 2, 2, 3 },
      { 2, 4, 2, 2, 3, 5, 3, 1 },
      { 1, 5, 2, 6, 4, 2, 3, 1 },
      { 1, 3, 5, 7, 7, 3, 4, 2 },
      { 2, 2, 4, 6, 8, 4, 2, 1 },
      { 1, 3, 2, 5, 6, 7, 7, 1 },
      { 3, 2, 2, 2, 2, 3, 2, 7 },
      { 1, 3, 2, 1, 1, 3, 2, 2 }
    };

    vec_point2d_t r_expected = {
      { divide, divide, divide, divide, up_right, up_right, up_right, up_right }, 
      { divide, divide, divide, divide, divide, divide, up_right, up_right },
      { down_left, divide, divide, divide, divide, valley, up_right, up_right },
      { down_left, down_left, divide, divide, divide, valley, up_right, up_right },
      { down_left, down_left, down_left, down_left, divide, up_right, up_right, up_right },
      { down_left, down_left, down_left, down_left, down_left, divide, divide, up_right },
      { down_left, down_left, down_left, down_left, down_left, divide, divide, divide },
      { down_left, down_left, down_left, down_left, down_left, divide, divide, divide }
    };

    cout << v << endl;
    vec_point2d_t r = mark_water_divide(v);
    cout << r << endl;
    assert(r == r_expected);
    cout << "recursion counter: " << global_count << endl;

    return 0;
}






