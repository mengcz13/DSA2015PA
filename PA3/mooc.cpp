#include <stdio.h>

#define LC(x) (x << 1)
#define RC(x) ((x << 1) | 1)
#define PA(x) (x >> 1)
#define GPA(x) (x >> 2)
#define ROOT 1
#define INF (~(1 << 31))
#define MINF (1 << 31)

//#define DEBUG

void swap(int &a, int &b) {
    int t = b;
    b = a;
    a = t;
}

//最大最小堆,由小顶堆改造而来.0,2,4,6,...存储以当前节点为根的树的最小值;1,3,5,7,...存最大值.画出其哈斯图,类似纺锤形.(详见M.D.Atkinson等人的论文"Min-Max Heaps and Generalized Priority Queues")
//插入方法同二叉堆,上滤有调整.首先确定过滤方向(朝向小顶还是大顶),之后隔层上滤即可.
//删除方法同二叉堆,但比较时应比较当前节点儿子和孙子节点中的最大/小值.与儿子交换则可直接向下迭代,与孙子交换后则需判断调整后孙子与儿子是否仍需要调整.利用数学归纳法易证每次操作后有且仅有当前节点为根的子树可能不满足堆序性,因此最后子树只有1个节点时整个树的堆序性得到满足,算法结束.
class MinMaxHeap {
private:
    int* data;
    int tail;
    int maxsize;
public:
    MinMaxHeap(int n);
    ~MinMaxHeap();
    void insert(int score);
    int get_min();
    int get_max();
    int pnum() {return (tail - 1);}

    void up(int r);
    void remove(int r);
    void up_as_min(int r);
    void up_as_max(int r);
    int get_layer_num(int r);
    int get_min_gchild(int r);
    int get_max_gchild(int r);
    int get_min_child(int r);
    int get_max_child(int r);
};

MinMaxHeap::MinMaxHeap(int n) {
    data = new int[n];
    maxsize = n;
    tail = 1;
}

MinMaxHeap::~MinMaxHeap() {
    delete []data;
}

void MinMaxHeap::insert(int score) {
    data[tail++] = score;
    up(tail - 1);
}

void MinMaxHeap::up(int rank) {
    int ln = get_layer_num(rank);
    if (ln == 0)
        return;
    if (ln & 1) { //ln is max layer
        if (data[rank] < data[PA(rank)]) {
            swap(data[rank], data[PA(rank)]);
            up_as_min(PA(rank));
        }
        else {
            up_as_max(rank);
        }
    }
    else { //ln is min layer
        if (data[rank] > data[PA(rank)]) {
            swap(data[rank], data[PA(rank)]);
            up_as_max(PA(rank));
        }
        else {
            up_as_min(rank);
        }
    }
    return;
}

int MinMaxHeap::get_min() {
    int min = data[ROOT];
    remove(ROOT);
    return min;
}

int MinMaxHeap::get_max() {
    int max = 0;
    int maxr = get_max_child(ROOT);
    if (maxr == -1) {
        maxr = ROOT;
        max = data[ROOT];
    }
    else {
        max = data[maxr];
    }
    remove(maxr);
    return max;
}

void MinMaxHeap::remove(int r) {
    swap(data[r], data[--tail]);
    int ln = get_layer_num(r);
    while(1) {
        if (ln & 1) { // max layer
            int mgc = get_max_gchild(r);
            int mc = get_max_child(r);
            if (mc == -1)
                return;
            else if (mc > 0 && mgc == -1) {
                if (data[r] < data[mc]) {
                    swap(data[r], data[mc]);
                    r = mc;
                    ++ln;
                }
                else
                    return;
            }
            else {
                int maxr = (data[mgc] > data[mc]) ? mgc : mc;
                if (maxr == mgc) {
                    if (data[r] < data[mgc]) {
                        swap(data[r], data[mgc]);
                        if (data[mgc] < data[PA(mgc)])
                            swap(data[mgc], data[PA(mgc)]);
                        r = mgc;
                        ln += 2;
                    }
                    else
                        return;
                }
                else {
                    if (data[r] < data[mc]) {
                        swap(data[r], data[mc]);
                        r = mc;
                        ++ln;
                    }
                    else
                        return;
                }
            }
        }
        else { // min layer
            int mgc = get_min_gchild(r);
            int mc = get_min_child(r);
            if (mc == -1)
                return;
            else if (mc > 0 && mgc == -1) {
                if (data[r] > data[mc]) {
                    swap(data[r], data[mc]);
                    r = mc;
                    ++ln;
                }
                else
                    return;
            }
            else {
                int minr = (data[mgc] < data[mc]) ? mgc : mc;
                if (minr == mgc) {
                    if (data[r] > data[mgc]) {
                        swap(data[r], data[mgc]);
                        if (data[mgc] > data[PA(mgc)])
                            swap(data[mgc], data[PA(mgc)]);
                        r = mgc;
                        ln += 2;
                    }
                    else
                        return;
                }
                else {
                    if (data[r] > data[mc]) {
                        swap(data[r], data[mc]);
                        r = mc;
                        ++ln;
                    }
                    else
                        return;
                }
            }
        }
    }
}

void MinMaxHeap::up_as_min(int r) {
    while (GPA(r)) {
        if (data[r] < data[GPA(r)]) {
            swap(data[r], data[GPA(r)]);
            r = GPA(r);
        }
        else {
            break;
        }
    }
}

void MinMaxHeap::up_as_max(int r) {
    while (GPA(r)) {
        if (data[r] > data[GPA(r)]) {
            swap(data[r], data[GPA(r)]);
            r = GPA(r);
        }
        else {
            break;
        }
    }
}

int MinMaxHeap::get_layer_num(int r) {
    int ln = 0;
    while (r != 1) {
        r = (r >> 1);
        ++ln;
    }
    return ln;
}

int MinMaxHeap::get_min_gchild(int r) {
    int gclist[4];
    gclist[0] = LC(LC(r));
    gclist[1] = LC(RC(r));
    gclist[2] = RC(LC(r));
    gclist[3] = RC(RC(r));
    int rank = -1, min = INF;
    for (int i = 0; i < 4; ++i) {
        if (gclist[i] < tail && data[gclist[i]] < min) {
            rank = gclist[i];
            min = data[gclist[i]];
        }
    }
    return rank;
}

int MinMaxHeap::get_max_gchild(int r) {
    int gclist[4];
    gclist[0] = LC(LC(r));
    gclist[1] = LC(RC(r));
    gclist[2] = RC(LC(r));
    gclist[3] = RC(RC(r));
    int rank = -1, max = MINF;
    for (int i = 0; i < 4; ++i) {
        if (gclist[i] < tail && data[gclist[i]] > max) {
            rank = gclist[i];
            max = data[gclist[i]];
        }
    }
    return rank;
}

int MinMaxHeap::get_min_child(int r) {
    int gclist[2];
    gclist[0] = LC(r);
    gclist[1] = RC(r);
    int rank = -1, min = INF;
    for (int i = 0; i < 2; ++i) {
        if (gclist[i] < tail && data[gclist[i]] < min) {
            rank = gclist[i];
            min = data[gclist[i]];
        }
    }
    return rank;
}

int MinMaxHeap::get_max_child(int r) {
    int gclist[2];
    gclist[0] = LC(r);
    gclist[1] = RC(r);
    int rank = -1, max = MINF;
    for (int i = 0; i < 2; ++i) {
        if (gclist[i] < tail && data[gclist[i]] > max) {
            rank = gclist[i];
            max = data[gclist[i]];
        }
    }
    return rank;
}

int main() {
    setvbuf(stdin, new char[1 << 20], _IOFBF, 1 << 20);
    setvbuf(stdout, new char[1 << 20], _IOFBF, 1 << 20);
    #ifdef DEBUG
    freopen("input.txt","r",stdin);
    #endif
    int n = 0;
    scanf("%d", &n);
    MinMaxHeap mmh(n << 1);
    getchar();
    for (int i = 0; i < n; ++i) {
        char c = getchar();
        if (c == 'I') {
            int pts = 0;
            scanf("%d", &pts);
            mmh.insert(pts);
        }
        else if (c == 'H') {
            if (mmh.pnum()) {
                printf("%d\n", mmh.get_max());
            }
            else {
                printf("0\n");
            }
        }
        else if (c == 'L') {
            if (mmh.pnum()) {
                printf("%d\n", mmh.get_min());
            }
            else {
                printf("0\n");
            }
        }
        getchar();
    }
    return 0;
}
