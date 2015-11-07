#include <stdio.h>

//#define DEBUG
#define MAX (100000)

struct Target {
    int left;
    int right;
    int anti;
};

struct Segment {
    int left;
    int right;
    int targetnum;//恰好覆盖当前区间的靶子的数量
    long long antisum;
    Segment* lc;
    Segment* rc;

    Segment():left(0), right(0), targetnum(0), antisum(0), lc(NULL), rc(NULL){}
};

void swappoint(Target*& t1, Target*& t2) {
    Target* temp = t1;
    t1 = t2;
    t2 = temp;
}

void quicksort(Target** tarray, int start, int end) {
    if (end - start <= 1)
        return;
    int pivot = tarray[start]->anti;
    int p = start + 1;
    for (int i = start + 1; i < end; ++i) {
        if (tarray[i]->anti < pivot) {
            swappoint(tarray[p++], tarray[i]);
        }
    }
    p--;
    swappoint(tarray[start], tarray[p]);
    quicksort(tarray, start, p);
    quicksort(tarray, p+1, end);
}

class HistorySegmentTree {

private:
    Segment* root;
    int rootnum;
    int root_tail;
    Segment* node;
    int store_size;
    int node_tail;

    int shootpoint;//射击点位：1~shootpoint

public:
    HistorySegmentTree(int rootn, int sizen, int sp);
    ~HistorySegmentTree();
    Segment* allocate();
    void insert(Target* target);
    long long get_antisum(int position, int k);//二分查找确定position位置上覆盖了k个靶子时的antisum(覆盖数是单调不减的)

protected:
    void copy_his_lc(Segment* curr_seg, Segment* hist_seg);
    void copy_his_rc(Segment* curr_seg, Segment* hist_seg);
    void insert_segment(Segment* curr_seg, int left, int right, int anti, Segment* hist_seg);
    int get_targetsum(int version, int position);
    long long get_antisum_in_his(int version, int position);
};

HistorySegmentTree::HistorySegmentTree(int rootn, int sizen, int sp) {
    rootnum = rootn;
    store_size = sizen;
    shootpoint = sp;
    root = new Segment[rootnum];
    node = new Segment[store_size];
    root_tail = 1;
    node_tail = 0;
    root[0].left = 0;
    root[0].right = shootpoint;
}

HistorySegmentTree::~HistorySegmentTree() {
    delete []root;
    delete []node;
}

Segment* HistorySegmentTree::allocate() {
    if (node_tail < store_size)
        return (&node[node_tail++]);
    else
        return (new Segment());
}

void HistorySegmentTree::insert(Target* target) {
    int root_last = root_tail - 1;
    root[root_tail].left = root[root_last].left;
    root[root_tail].right = root[root_last].right;
    root[root_tail].targetnum = root[root_last].targetnum;
    root[root_tail].antisum = root[root_last].antisum;
    insert_segment(&root[root_tail++], target->left, target->right, target->anti, &root[root_last]);
}

long long HistorySegmentTree::get_antisum(int position, int k) {
    int start = 1, end = root_tail;
    while (end - start > 0) {
        int mid = (start + end) >> 1;
        int midvalue = get_targetsum(mid, position);
        if (k < midvalue)
            end = mid;
        else
            start = mid + 1;
    }
    start--;
    return get_antisum_in_his(start, position);
}

void HistorySegmentTree::copy_his_lc(Segment* curr_seg, Segment* hist_seg) {
    curr_seg->lc = allocate();
    curr_seg->lc->left = curr_seg->left;
    curr_seg->lc->right = ((curr_seg->left + curr_seg->right) >> 1);
    if (hist_seg != NULL && hist_seg->lc != NULL) {
        curr_seg->lc->targetnum = hist_seg->lc->targetnum;
        curr_seg->lc->antisum = hist_seg->lc->antisum;
    }
    else {
        curr_seg->lc->targetnum = curr_seg->targetnum;
        curr_seg->lc->antisum = curr_seg->antisum;
    }
}

void HistorySegmentTree::copy_his_rc(Segment* curr_seg, Segment* hist_seg) {
    curr_seg->rc = allocate();
    curr_seg->rc->left = ((curr_seg->left + curr_seg->right) >> 1) + 1;
    curr_seg->rc->right = curr_seg->right;
    if (hist_seg != NULL && hist_seg->rc != NULL) {
        curr_seg->rc->targetnum = hist_seg->rc->targetnum;
        curr_seg->rc->antisum = hist_seg->rc->antisum;
    }
    else {
        curr_seg->rc->targetnum = curr_seg->targetnum;
        curr_seg->rc->antisum = curr_seg->antisum;
    }
}

void HistorySegmentTree::insert_segment(Segment* curr_seg, int left, int right, int anti, Segment* hist_seg) {
    if (left > right)
        return;
    if (curr_seg->left == left && curr_seg->right == right) {
        //树节点与待插入区间恰好重合，根据上一历史版本确定能否直接返回。上一版本若此点为空或者无孩子即可返回。
        if (hist_seg == NULL || hist_seg->lc == NULL && hist_seg->rc == NULL) {
            ++(curr_seg->targetnum);
            curr_seg->antisum += anti;

            #ifdef DEBUG
            printf("left:%d\tright:%d\ttargetnum:%d\tantisum:%lld\tsize:%d\n", left, right, curr_seg->targetnum, curr_seg->antisum, node_tail);
            #endif

            return;
        }
        else {
            copy_his_lc(curr_seg, hist_seg);
            copy_his_rc(curr_seg, hist_seg);
            insert_segment(curr_seg->lc, curr_seg->lc->left, curr_seg->lc->right, anti, hist_seg->lc);
            insert_segment(curr_seg->rc, curr_seg->rc->left, curr_seg->rc->right, anti, hist_seg->rc);
            return;
        }
    }
    else {
        int seg_mid = ((curr_seg->left + curr_seg->right) >> 1);
        if (right <= seg_mid) {
            //向左深入，右分支与历史版本相同
            if (hist_seg == NULL) {
                curr_seg->rc = NULL;
            }
            else
                curr_seg->rc = hist_seg->rc;
            //curr_seg->lc = ...
            copy_his_lc(curr_seg, hist_seg);
            insert_segment(curr_seg->lc, left, right, anti, (hist_seg == NULL)?NULL:(hist_seg->lc));
        }
        else if (left > seg_mid) {
            //向右深入，左分支与历史版本相同
            if (hist_seg == NULL) {
                curr_seg->lc = NULL;
            }
            else
                curr_seg->lc = hist_seg->lc;
            //curr_seg->rc = ...
            copy_his_rc(curr_seg, hist_seg);
            insert_segment(curr_seg->rc, left, right, anti, (hist_seg == NULL)?NULL:(hist_seg->rc));
        }
        else {
            //向左右分别深入
            copy_his_lc(curr_seg, hist_seg);
            copy_his_rc(curr_seg, hist_seg);
            insert_segment(curr_seg->lc, left, seg_mid, anti, (hist_seg == NULL)?NULL:(hist_seg->lc));
            insert_segment(curr_seg->rc, seg_mid + 1, right, anti, (hist_seg == NULL)?NULL:(hist_seg->rc));
        }
    }
}

int HistorySegmentTree::get_targetsum(int version, int position) {
    Segment* p = &root[version];
    while (1) {
        int mid = (p->left + p->right) >> 1;
        if (position > mid){
            if (p->rc != NULL)
                p = p->rc;
            else
                break;
        }
        else {
            if (p->lc != NULL)
                p = p->lc;
            else
                break;
        }
    }
    return (p->targetnum);
}

long long HistorySegmentTree::get_antisum_in_his(int version, int position) {
    Segment* p = &root[version];
    while (1) {
        int mid = (p->left + p->right) >> 1;
        if (position > mid){
            if (p->rc != NULL)
                p = p->rc;
            else
                break;
        }
        else {
            if (p->lc != NULL)
                p = p->lc;
            else
                break;
        }
    }
    return (p->antisum);
}

int main() {
    #ifdef DEBUG
    freopen("input.txt","r",stdin);
    freopen("output.txt","w",stdout);
    #endif

    int m = 0, n = 0;
    scanf("%d%d", &m, &n);
    Target** tarray = new Target*[m];
    Target* tdata = new Target[m];
    for (int i = 0; i < m; ++i) {
        int l = 0, r = 0, w = 0;
        scanf("%d%d%d", &l, &r, &w);
        tdata[i].left = l;
        tdata[i].right = r;
        tdata[i].anti = w;
        tarray[i] = &tdata[i];
    }
    quicksort(tarray, 0, m);

    HistorySegmentTree* hst = new HistorySegmentTree(m + 1, 100 * m, n + 16);
    for (int i = 0; i < m; ++i) {
        hst->insert(tarray[i]);
    }

    long long pre = 1;
    for (int i = 0; i < n; ++i) {
        int x = 0, a = 0, b = 0, c = 0;
        scanf("%d%d%d%d", &x, &a, &b, &c);
        int k = (a * pre + b) % c + 1;
        pre = hst->get_antisum(x, k);
        printf("%lld\n", pre);
    }

    delete hst;
    delete []tarray;
    delete []tdata;
    return 0;
}
