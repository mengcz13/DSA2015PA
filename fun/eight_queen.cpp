#define N 8
#include <iostream>
#include <stack>
#include <vector>
using namespace std;

struct Point{
    int x;
    int y;
    Point(int xx, int yy) {
        x = xx;
        y = yy;
    }
};

bool if_coexist(const Point& p1, const Point& p2) {
    if (p1.x == p2.x || p1.y == p2.y || (p1.x - p2.x) == (p1.y - p2.y) || (p1.x - p2.x) == -(p1.y - p2.y))
        return false;
    else
        return true;
}

bool if_able(const vector<Point>& sta, const Point& p) {
    for (int i = 0; i < sta.size(); ++i) {
        if (!if_coexist(p, sta[i]))
            return false;
    }
    return true;
}

int main() {
    int solu = 0;
    vector<Point> chess_stack;
    Point p(0, 0);
    chess_stack.push_back(p);
    while (!chess_stack.empty()) {
        // cout << "(" << chess_stack.back().x << "," << chess_stack.back().y << ")" << endl;
        int next_column = 0;
        bool if_hasnext = false;
        if (chess_stack.back().x + 1 == N)
            if_hasnext = false;
        else {
            for (next_column = 0; next_column < N; ++next_column) {
                Point tp(chess_stack.back().x + 1, next_column);
                if (if_able(chess_stack, tp)) {
                    if_hasnext = true;
                    chess_stack.push_back(tp);
                    break;
                }
            }
        }
        if (!if_hasnext) {
            if (chess_stack.size() == N)
                ++solu;
            bool if_next = false;
            while (!if_next && !chess_stack.empty()) {
            if_next = false;
            int startx = chess_stack.back().x, starty = chess_stack.back().y;
            chess_stack.pop_back();
            for (int i = starty + 1; i < N; ++i) {
                Point tp(startx, i);
                if (if_able(chess_stack, tp)) {
                    chess_stack.push_back(tp);
                    if_next = true;
                    break;
                }
            }
            }
        }
    }
    cout << solu << endl;
    return 0;
}