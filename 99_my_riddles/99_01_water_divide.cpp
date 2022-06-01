
//
// I found this a particularly difficult riddle for a CI.
//
// I admit that I failed that one and did not get the job. I do find the question
// too difficult for a 1h session.
//
// The riddle:
// -----------
// Given a map (as a 2D matrix) where each value represents the altitude at the given (x,y) coordinate.
// calculate the water divide where the water flows either to the top/right side or the bottom/ left 
// side.
//
// For simplicity water only flows left/ right or up/ down, not diagnoally
// Water always flows down all sides that are same hight or lower
//
// example:
//
//
//    0  1 2 2 3 1 2 2 3     x x x x u u u u
//    1  2 4 2 2 3 5 3 1     x x x x x x u u
//    2  1 5 2 6 4 2 3 1     d x x x x v u u 
//    3  1 3 5 7 7 3 4 2 ==> d d x x x v u u
//    4  2 2 4 6 8 4 2 1     d d d d x u u u
//    5  1 3 2 5 6 7 7 1     d d d d d x x u
//    6  3 2 2 2 2 3 2 7     d d d d d x x x
//    7  1 3 2 1 1 3 2 2     d d d d d x x x
//
//  points marked with u flow up or right. points marked with d flow down or left.
//  points marked with v flow nowhere
//  points marked with x flow either u or d. (i.e. they are the divide points)
//

//
// Improvement ideas:
//   I think that breadth search rather than depth search would more efficient
//   The solution strategy is dynamic programming were you should make re-use of results from
//   previous "runs". This solution is not making any re-use.
//   The results from previous searches should be reused like this:
//     - result matrix collects results from previous runs
//     - stop if the search finds a field covered in a previous run and re-use that result
//
// Style: 
//  - I tried to make this very "functional" rather than object-oriented
//    so the functions have a lot of parameters
//    style could be improved by making them lambdas and capture the re-used variables
//
//  - too many ugly if statements with complicated logic
//    there must be a way to simplify this ugliness
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
    int height = v.size();
    int width = v[0].size();
    int row = p.first;
    int col = p.second;
    if (row < 0 && col < 0) { 
        cerr << "ERROR: row:" << row << " col: " << col << endl; 
        exit(-1); 
    }
    if (row >= height && col >= width) { 
        cerr << "ERROR: row:" << row << " col: " << col << endl; 
        exit(-1); 
    }
    if (row < 0) { return up_right; }
    if (row >= height) { return down_left; }
    if (col < 0) { return down_left; }
    if (col >= width) { return up_right; }
    return land;
}

bool is_visited_or_ocean(const vec2d_t &v, vec_bool2d_t &util_matrix, coord_t p) {
    point_t point_type = get_point_type(v, p);
    if (point_type == up_right || point_type == down_left) { 
        return true; 
    }
    if (util_matrix[p.first][p.second]) {
        return true;
    }
    return false;
}

list_coord_t get_possible_neighbours(const vec2d_t &v, vec_bool2d_t &util_matrix, coord_t start_point) {
    list_coord_t rslt;
    list_coord_t final_rslt;
    int height = v.size();
    int width = v[0].size();

    // we don't have neighbours for an ocean point
    if (start_point.first < 0 || start_point.second < 0 || 
        start_point.first >= height || start_point.second >= width) {
        return rslt;
    }

    vector<coord_t> potential_neighbours = { 
        {start_point.first-1, start_point.second},
        {start_point.first+1, start_point.second}, 
        {start_point.first, start_point.second-1}, 
        {start_point.first, start_point.second+1} 
    };

    for (auto n : potential_neighbours) {
        if (n.first < -1 || n.second < -1 || n.first > height || n.second > width) {
            // do nothing
        } else if ((n.first < 0 && n.second < 0 ) || (n.first >= height && n.second >= width)) {
            // do nothing
        } else if (n.first == -1 || n.second == -1 || n.first == height || n.second == width) {
            rslt.push_back(n);
        } else if (!util_matrix[n.first][n.second]) {
            if (v[start_point.first][start_point.second] >= v[n.first][n.second]) {
                rslt.push_back(n);
            }
        }
    }
    // the loop makes the neighbours that go into the ocean come first
    // this should improve the performance
    for (auto it= rslt.begin(); it != rslt.end(); it++) {
        if (it->first < 0 || it->second < 0 || it->first >= height || it->second >= width) {
            final_rslt.push_front(*it);
        } else {
            final_rslt.push_back(*it);
        }
    }
    return final_rslt;
}

static int global_count = 0;

bool recursive_find(const vec2d_t &v, vec_bool2d_t &util_matrix, point_t ocean, coord_t start_point) {
    global_count++;
    // if (global_count >= 200) { exit(0); }
    // cout << "recursive_find: " << start_point << " ocean: " << ocean;
    point_t p_type = get_point_type(v, start_point);
    // cout << " p_type: " << p_type << endl;
    if (p_type == ocean) { return true; }
    if (p_type == land) {
        util_matrix[start_point.first][start_point.second] = true; // mark as visited
    }
    list_coord_t neighbours = get_possible_neighbours(v, util_matrix, start_point);
    // cout << start_point << " neighbours: " << neighbours << endl;
    for (auto coord : neighbours) {
        bool rslt = recursive_find(v, util_matrix, ocean, coord);
        if (rslt) { return true; }
    }
    return false;
}

//
// This function searches for a path to one ocean
// It returns true if found and false otherwise
//
// idea:
//   Adapted Deijkstra:
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
bool find_path_to_ocean(const vec2d_t &v, point_t ocean, coord_t start_point) {
    int height = v.size();
    int width = v[0].size();
    vec_bool2d_t util_matrix(height, vec_bool_t(width, false));
    bool rslt = recursive_find(v, util_matrix, ocean, start_point);
    return rslt;
}

// 
// idea:
//  - iterate over all points and call mark_path_to_oceans
//
vec_point2d_t mark_water_divide(const vec2d_t &v) {
    int height = v.size();
    int width  = v[0].size();
    vec_point2d_t rslt(height, vec_point_t(width, unkown));
    for (int row = 0; row < height; row++) {
        for (int col = 0; col < width; col++) {
            //if (row ==1 && col == 2) {
            //    cout << endl << endl << endl << endl;
            //}
            coord_t c {row, col};
            bool path_to_up_ocean_exists = find_path_to_ocean(v, up_right, c );
            bool path_to_down_ocean_exists = find_path_to_ocean(v, down_left, c);
            if (path_to_up_ocean_exists && path_to_down_ocean_exists) {
                rslt[row][col] = divide;
            } else if (path_to_up_ocean_exists) {
                rslt[row][col] = up_right;
            } else if (path_to_down_ocean_exists) {
                rslt[row][col] = down_left;
            } else {
                rslt[row][col] = valley;
            }
            // if (row == 1 && col == 2) { return rslt; } 
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
}






