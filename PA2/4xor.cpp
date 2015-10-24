#include <stdio.h>
#define MAX (30*250000)
#define MAXD 30
#define IOBUFFER (1<<20)
//#define DEBUG

struct TreeNode {
    TreeNode* lc;
    TreeNode* rc;
    TreeNode():lc(NULL),rc(NULL){}
};

class BinaryTree {
private:
    TreeNode* buffer;
    TreeNode* root;
    int maxres;
    int tail;
    int maxsize;
public:
    BinaryTree(int);
    ~BinaryTree();
    void insert_num(int);//将当前数字加入编码树
    void greedy(int);//贪心法更新结果
    int maxr(){return maxres;}
    TreeNode* allocate();
};

BinaryTree::BinaryTree(int n) {
    if (n<MAX)
        buffer=new TreeNode[n];
    else
        buffer=new TreeNode[MAX];
    maxsize=(n<MAX)?n:MAX;
    tail=0;
    maxres=0;
    root=allocate();
}

BinaryTree::~BinaryTree() {
    root=NULL;
    delete []buffer;
}

TreeNode* BinaryTree::allocate() {
    if (tail<maxsize)
        return (&buffer[tail++]);
    else
        return (new TreeNode());
}

void BinaryTree::insert_num(int x) {
    TreeNode* p=root;
    for (int i=MAXD-1;i>=0;i--) {
        int digit=((x>>i)&1);
        if (digit) {
            if (p->rc==NULL)
                p->rc=allocate();
            p=p->rc;
        }
        else {
            if (p->lc==NULL)
                p->lc=allocate();
            p=p->lc;
        }
    }
}

void BinaryTree::greedy(int x) {
    int res=0;
    TreeNode* p=root;
    for (int i=MAXD-1;i>=0;i--) {
        int digit=((x>>i)&1);
        if (digit) {
            if (p->lc!=NULL) {
                p=p->lc;
                res+=(1<<i);
            }
            else if (p->rc!=NULL) {
                p=p->rc;
            }
            else {
                res+=(x&((1<<(i+1))-1));
                break;
            }
        }
        else {
            if (p->rc!=NULL) {
                p=p->rc;
                res+=(1<<i);
            }
            else if (p->lc!=NULL) {
                p=p->lc;
            }
            else {
                res+=(x&((1<<(i+1))-1));
                break;
            }
        }
    }
    if (res>maxres)
        maxres=res;
}

int main() {
    setvbuf(stdin, new char[IOBUFFER], _IOFBF, IOBUFFER);
    setvbuf(stdout, new char[IOBUFFER], _IOFBF, IOBUFFER);
    #ifdef DEBUG
    freopen("input.txt","r",stdin);
    freopen("output.txt","w",stdout);
    #endif
    int n=0, x=0;
    scanf("%d",&n);
    BinaryTree* bt=new BinaryTree(MAXD*n);
    for (int i=0;i<n;++i) {
        scanf("%d",&x);
        bt->insert_num(x);
        bt->greedy(x);
    }
    printf("%d",bt->maxr());
    delete bt;
    return 0;
}