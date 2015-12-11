#include <stdio.h>
#include <string.h>
#include "temperature.h"

#define LC(x) (x << 1)
#define RC(x) ((x << 1) | 1)
#define PA(x) (x >> 1)
#define NEGINF (1 << 31)
#define INF (~(1 << 31))

typedef enum {X, Y} IDS;

struct ObStation {
    int x;
    int y;
    long long tem;
};

void quicksort(ObStation** array, int start, int end, IDS ids) {
    if (end - start <= 1)
        return;
    int pivot = start;
    int p = start + 1;
    if (ids == X) {
        for (int j = start + 1; j < end; ++j) {
            if (array[j]->x < array[pivot]->x) {
                ObStation* temp = array[j];
                array[j] = array[p];
                array[p] = temp;
                ++p;
            }
        }
    }
    else {
        for (int j = start + 1; j < end; ++j) {
            if (array[j]->y < array[pivot]->y) {
                ObStation* temp = array[j];
                array[j] = array[p];
                array[p] = temp;
                ++p;
            }
        }
    }
    --p;
    ObStation* temp = array[p];
    array[p] = array[pivot];
    array[pivot] = temp;
    quicksort(array, start, p, ids);
    quicksort(array, p + 1, end, ids);
}

int binary_search(ObStation** array, int start, int end, int target, IDS ids) {
    if (ids == X) {
        while (start < end) {
            int mid = (start + end) >> 1;
            if (target < array[mid]->x)
                end = mid;
            else
                start = mid + 1;
        }
        return start - 1;
    }
    else {
        while (start < end) {
            int mid = (start + end) >> 1;
            if (target < array[mid]->y)
                end = mid;
            else
                start = mid + 1;
        }
        return start - 1;
    }
}

struct TreeNode {
    int start;
    int end;
    int left;
    int right;
    TreeNode() {
        left = -1;
    }
};

struct Director { //存储yarranged_pool中每个点的游标，归并过程中确定。确定后在递归查询时可根据游标直接确定被查询的y在左右孩子中秩的范围。
    int ll; //下界y1在左孩子中的下界
    int lh; //上界y2在左孩子中的上界
    int rl; //下界y1在右孩子中的下界
    int rh; //上界y2在右孩子中的上界
};

//范围树,根据离散化后的X把点分开.每个节点中根据y排序,使用前缀和数组存储temp的值(变为1维区间查询)
class RangeTree {
private:
    ObStation* data;//数据区
    int datatail;
    TreeNode* xtree;//数组存储根据X分出的"线段树"
    int xtreetail;
    ObStation** xarranged;//根据X排序的结果
    int* x2discrete_table;//X的rank与离散后的X值的对照表,便于在xarranged中二分查找后直接确定其离散值
    int* discrete;//X的离散值与X的首rank的对照表,discrete[i]表示离散值为i的点在xarrange中最小的rank
    int discretetail;
    ObStation** yarranged_pool;//存储所有按y排序后的各节点数组的内存池
    Director* yarranged_pool_director;//游标存储
    int yptail;
    long long* prefix_ysum;//与内存池对应,存储所有节点的前缀和数组
    int pointnum;
public:
    RangeTree(int n);//初始化
    ~RangeTree();
    void readdata(int ix, int iy, int itemp);//插入station到数据区
    void discreteX();//对X离散化
    void build();//建树
    void buildnode(int l, int r, int rank);//构建表示X区间[l,r]的节点
    long long outside_query(int x1, int x2, int y1, int y2, int& psum);//对外接口,输入未离散化的X和Y
    long long query(int x1, int x2, int y1, int y2, int rank, int& psum);//对内接口,X已经转换为离散值,同时需要确定y的离散范围
};

RangeTree::RangeTree(int n) {
    data = new ObStation[n];
    datatail = 0;
    xtree = new TreeNode[n << 2];
    xtreetail = 0;
    xarranged = new ObStation*[n];
    x2discrete_table = new int[n];
    discrete = new int[n];
    discretetail = 0;
    yarranged_pool = new ObStation*[n << 5];
    yarranged_pool_director = new Director[n << 5];
    yptail = 0;
    prefix_ysum = new long long[n << 5];
    pointnum = n;
}

RangeTree::~RangeTree() {
    delete []data;
    delete []xtree;
    delete []xarranged;
    delete []x2discrete_table;
    delete []discrete;
    delete []yarranged_pool;
    delete []yarranged_pool_director;
    delete []prefix_ysum;
}

void RangeTree::readdata(int ix, int iy, int itemp) {
    data[datatail].x = ix;
    data[datatail].y = iy;
    data[datatail].tem = itemp;
    ++datatail;
}

void RangeTree::discreteX() {
    for (int i = 0; i < datatail; ++i) {
        xarranged[i] = &data[i];
    }
    quicksort(xarranged, 0, datatail, X);
    int lastx = xarranged[0]->x;
    discrete[discretetail++] = 0;
    x2discrete_table[0] = 0;
    for (int i = 0; i < datatail; ++i) {
        if (xarranged[i]->x != lastx) {
            discrete[discretetail++] = i;
            lastx = xarranged[i]->x;
        }
        x2discrete_table[i] = discretetail - 1;
    }
    discrete[discretetail] = datatail;
}

void RangeTree::build() {
    buildnode(0, discretetail - 1, 1);//构建所有节点代表的区间
    for (int i = xtreetail; i >= 1; --i) {//利用线段树性质,数组倒序遍历即为自下而上的层次遍历
        //节点未必连续分布,反例[0,9]
        if (xtree[i].left == -1)
            continue;
        else {
            if (xtree[i].left == xtree[i].right) {//对于叶子节点,快排Y
                int xstart = discrete[xtree[i].left];
                int xend = discrete[xtree[i].right + 1];
                xtree[i].start = yptail;
                for (int j = xstart; j < xend; ++j) {
                    yarranged_pool[yptail++] = xarranged[j];
                }
                xtree[i].end = yptail;
                quicksort(yarranged_pool, xtree[i].start, xtree[i].end, Y);
            }
            else {//对于非叶子节点,由于左右孩子已经有序,故直接归并
                int plc = xtree[LC(i)].start;
                int bdlc = xtree[LC(i)].end;
                int prc = xtree[RC(i)].start;
                int bdrc = xtree[RC(i)].end;
                xtree[i].start = yptail;
                while (plc < bdlc && prc < bdrc) {
                    if (yarranged_pool[plc]->y > yarranged_pool[prc]->y) {
                        yarranged_pool[yptail] = yarranged_pool[prc];
                        ++yptail;
                        ++prc;
                    }
                    else {
                        yarranged_pool[yptail] = yarranged_pool[plc];
                        ++yptail;
                        ++plc;
                    }
                }
                if (plc == bdlc) {
                    for (int j = prc; j < bdrc; ++j) {
                        yarranged_pool[yptail] = yarranged_pool[j];
                        ++yptail;
                    }
                }
                else {
                    for (int j = plc; j < bdlc; ++j) {
                        yarranged_pool[yptail] = yarranged_pool[j];
                        ++yptail;
                    }
                }
                xtree[i].end = yptail;
                int llp = xtree[LC(i)].start, lhp = xtree[LC(i)].start, rlp = xtree[RC(i)].start, rhp = xtree[RC(i)].start;
                int disp = xtree[i].start;
                while (disp < xtree[i].end) { //归并完成后确定游标
                    int curr = yarranged_pool[disp]->y;
                    while (llp < bdlc && yarranged_pool[llp]->y < curr) {
                        ++llp;
                    }
                    yarranged_pool_director[disp].ll = llp;
                    while (rlp < bdrc && yarranged_pool[rlp]->y < curr) {
                        ++rlp;
                    }
                    yarranged_pool_director[disp].rl = rlp;
                    while (lhp < bdlc && yarranged_pool[lhp]->y <= curr) {
                        ++lhp;
                    }
                    while (rhp < bdrc && yarranged_pool[rhp]->y <= curr) {
                        ++rhp;
                    }
                    while (disp < xtree[i].end && yarranged_pool[disp]->y == curr) {
                        ++disp;
                    }
                    yarranged_pool_director[disp - 1].lh = lhp - 1;
                    yarranged_pool_director[disp - 1].rh = rhp - 1;
                }
            }
            //前缀和
            // prefix_ysum[xtree[i].start] = yarranged_pool[xtree[i].start]->tem;
            // for (int j = xtree[i].start + 1; j < xtree[i].end; ++j) {
            //     prefix_ysum[j] = prefix_ysum[j - 1] + yarranged_pool[j]->tem;
            // }
        }
    }
}

void RangeTree::buildnode(int l, int r, int rank) {
    if (rank > xtreetail)
        xtreetail = rank;
    xtree[rank].left = l;
    xtree[rank].right = r;
    if (l < r) {
        buildnode(l, (l + r) >> 1, LC(rank));
        buildnode(((l + r) >> 1) + 1, r, RC(rank));
    }
    else
        return;
}

long long RangeTree::query(int x1, int x2, int y1, int y2, int rank, int& psum) {
    if (y1 > y2) {
        psum = 0;
        return 0;
    }
    if (xtree[rank].left == x1 && xtree[rank].right == x2) {//匹配时根据y查找前缀和
        psum = y2 - y1 + 1;
        // if (y1 == xtree[rank].start)
        //     return prefix_ysum[y2];
        // else
        //     return (prefix_ysum[y2] - prefix_ysum[y1 - 1]);
        long long temsum = 0;
        for (int ri = y1; ri <= y2; ++ri) {
            temsum += yarranged_pool[ri]->tem;
        }
        return temsum;
    }
    else {
        int mid = (xtree[rank].left + xtree[rank].right) >> 1;
        if (x2 <= mid) {
            return query(x1, x2, yarranged_pool_director[y1].ll, yarranged_pool_director[y2].lh, LC(rank), psum);
        }
        else if (x1 > mid) {
            return query(x1, x2, yarranged_pool_director[y1].rl, yarranged_pool_director[y2].rh, RC(rank), psum);
        }
        else {
            int leftpsum = 0, rightpsum = 0;
            long long leftsum = query(x1, mid, yarranged_pool_director[y1].ll, yarranged_pool_director[y2].lh, LC(rank), leftpsum);
            long long rightsum = query(mid + 1, x2, yarranged_pool_director[y1].rl, yarranged_pool_director[y2].rh, RC(rank), rightpsum);
            psum = leftpsum + rightpsum;
            return (leftsum + rightsum);
        }
    }
}

long long RangeTree::outside_query(int x1, int x2, int y1, int y2, int& psum) {
    //找到X的离散值
    int dx1 = 0, dx2 = 0;
    if (x1 == NEGINF)
        dx1 = 0;
    else
        dx1 = binary_search(xarranged, 0, pointnum, x1 - 1, X) + 1;
    dx2 = binary_search(xarranged, 0, pointnum, x2, X);
    if (dx1 > dx2)
        return 0;
    else {
        dx1 = x2discrete_table[dx1];
        dx2 = x2discrete_table[dx2];
    }
    int dy1 = 0, dy2 = 0;
    if (y1 == NEGINF)
        dy1 = xtree[1].start;
    else
        dy1 = binary_search(yarranged_pool, xtree[1].start, xtree[1].end, y1 - 1, Y) + 1;
    dy2 = binary_search(yarranged_pool, xtree[1].start, xtree[1].end, y2, Y);
    if (dy1 > dy2)
        return 0;
    long long sum = query(dx1, dx2, dy1, dy2, 1, psum);
    return sum;
}

int main() {
    setvbuf(stdin, new char[1 << 20], _IOFBF, 1 << 20);
    setvbuf(stdout, new char[1 << 20], _IOFBF, 1 << 20);
    int n = GetNumOfStation();
    RangeTree rt(n);
    for (int i = 0; i < n; ++i) {
        int ix = 0, iy = 0, itemp = 0;
        GetStationInfo(i, &ix, &iy, &itemp);
        rt.readdata(ix, iy, itemp);
    }
    rt.discreteX();
    rt.build();
    int x1 = 0, x2 = 0, y1 = 0, y2 = 0;
    while (int judge = GetQuery(&x1, &y1, &x2, &y2)) {
        int psum = 0;
        long long sum = rt.outside_query(x1, x2, y1, y2, psum);
        if (psum == 0)
            Response(0);
        else
            Response(sum / psum);
    }
    return 0;
}
