#include <stdio.h>

struct Segment {
    int left;
    int right;
    int targetnum;//恰好覆盖当前区间的靶子的数量
    long long antisum;
    Segment* lc;
    Segment* rc;

    Segment():left(0), right(0), targetnum(0), antisum(0), lc(NULL), rc(NULL){}
};

int main() {
    Segment* seg = new Segment[10000000];
    delete []seg;
    return 0;
}