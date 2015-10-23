#include <cstdio>

#define MAX ((1<<24)+(1<<22)+(1<<21)+192000)
#define MAXSTR (64+1)
#define IOBUFFER 1<<10

//#define DEBUG

struct TreeNode{
    char credit;
    char depth;
    TreeNode* parent;
    TreeNode* lc;
    TreeNode* rc;

    TreeNode():credit(-1),depth(0),parent(NULL),lc(NULL),rc(NULL){}

    void insert_child(TreeNode* child, int flag){
        child->parent=this;
        if (flag)
            rc=child;
        else
            lc=child;
        child->depth=1-this->depth;
    }
};

class GameTree{
private:
    TreeNode* buffer;
    int tail;
    TreeNode* root;
    int maxsize;

public:
    GameTree(int size);
    ~GameTree();
    void insert_string(char*);//插入新串
    void update(TreeNode*);//从插入点向上更新胜负值
    TreeNode* allocate();//从buffer分配一单位节点空间
    char if_adam_win(){return root->credit;}
};

GameTree::GameTree(int size){
    if (size<MAX){
        buffer=new TreeNode[size];
        maxsize=size;
    }
    else{
        buffer=new TreeNode[MAX];
        maxsize=MAX;
    }
    tail=0;
    root=allocate();
}

GameTree::~GameTree(){
    root=NULL;
    delete []buffer;
}

void GameTree::insert_string(char* seq){
    int p=0;
    TreeNode* par=root;
    while (seq[p]>47){
        if ((~seq[p])&1){
            if (par->rc!=NULL)
                par=par->rc;
            else{
                TreeNode* ch=allocate();
                par->insert_child(ch,1);
                par=ch;
            }
        }
        else{
            if (par->lc!=NULL)
                par=par->lc;
            else{
                TreeNode* ch=allocate();
                par->insert_child(ch,0);
                par=ch;
            }
        }
        p++;
    }
    if (par->credit==-1){
        if (par->depth & 1)
            par->credit=1;
        else
            par->credit=0;
        update(par);
    }
}

void GameTree::update(TreeNode* latest){
    while (latest!=root){
        char to_be_changed=0;
        if (latest->depth & 1){
            char lcredit=(latest->parent->lc==NULL)?0:latest->parent->lc->credit;
            char rcredit=(latest->parent->rc==NULL)?0:latest->parent->rc->credit;
            to_be_changed=(lcredit|rcredit);
        }
        else{
            char lcredit=(latest->parent->lc==NULL)?1:latest->parent->lc->credit;
            char rcredit=(latest->parent->rc==NULL)?1:latest->parent->rc->credit;
            to_be_changed=(lcredit&rcredit);
        }

        //若当前父节点之前已更新过且本次无需修改则退出
        if (to_be_changed==latest->parent->credit)
            break;
        latest->parent->credit=to_be_changed;
        latest=latest->parent;
    }
}

TreeNode* GameTree::allocate(){
    if (tail<maxsize)
        return (&buffer[tail++]);
    else
        return (new TreeNode());
}

int main(){
    setvbuf(stdin, new char[IOBUFFER], _IOFBF, IOBUFFER);
    setvbuf(stdout, new char[IOBUFFER], _IOFBF, IOBUFFER);

    #ifdef DEBUG
    freopen("input.txt","r",stdin);
    freopen("output.txt","w",stdout);
    #endif
    
    int n=0;
    scanf("%d",&n);
    GameTree* gametree=new GameTree((40000000));
    getchar();
    char* str=new char[MAXSTR];
    char* test;
    int r=1;
    char lastone=-1;

    test=gets(str);
    gametree->insert_string(str);
    if (gametree->if_adam_win())
        printf("Adam 1 ");
    else
        printf("Eve 1 ");
    lastone=gametree->if_adam_win();
    r++;

    while ((test=gets(str))!=NULL){
        gametree->insert_string(str);
        char current=gametree->if_adam_win();
        if (current!=lastone){
            if (current){
                printf("%d\nAdam %d ",r-1,r);
            }
            else{
                printf("%d\nEve %d ",r-1,r);
            }
            lastone=current;
        }
        r++;
    }
    printf("%d",n);

    #ifdef DEBUG
    TreeNode tnd;
    printf("\n%u",sizeof(NULL));
    #endif

    delete gametree;
    delete str;

    return 0;
}