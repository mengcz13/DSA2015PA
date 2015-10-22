#include <cstdio>

#define MAX ((1<<24)+(1<<22)+(1<<21))
#define MAXSTR (64+1)

#define DEBUG

struct TreeNode{
    char credit;
    char depth;
    TreeNode* parent;
    TreeNode* lc;
    TreeNode* rc;

    TreeNode():credit(0),depth(0),parent(NULL),lc(NULL),rc(NULL){}

    void insert_child(TreeNode* child, int flag){
        child->parent=this;
        if (flag)
            rc=child;
        else
            lc=child;
        child->depth=this->depth+1;
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
    while (seq[p]>='0'){
        if ((~seq[p])&1){
            if (par->rc!=NULL)
                par=par->rc;
            else{
                TreeNode* ch=allocate();
                par->insert_child(ch,(~seq[p])&1);
                par=ch;
            }
        }
        else{
            if (par->lc!=NULL)
                par=par->lc;
            else{
                TreeNode* ch=allocate();
                par->insert_child(ch,(~seq[p])&1);
                par=ch;
            }
        }
        p++;
    }
    if (par->lc==NULL && par->rc==NULL){
        if (par->depth & 1)
            par->credit=1;
        update(par);
    }
}

void GameTree::update(TreeNode* latest){
    TreeNode* temp=latest;
    while (temp!=root){
        char to_be_changed=0;
        if (temp->depth & 1){
            char lcredit=(temp->parent->lc==NULL)?0:temp->parent->lc->credit;
            char rcredit=(temp->parent->rc==NULL)?0:temp->parent->rc->credit;
            to_be_changed=(lcredit|rcredit);
        }
        else{
            char lcredit=(temp->parent->lc==NULL)?1:temp->parent->lc->credit;
            char rcredit=(temp->parent->rc==NULL)?1:temp->parent->rc->credit;
            to_be_changed=(lcredit&rcredit);
        }

        //这里的优化似乎有错……似乎某节点不需修改后仍需向上更新
        // if (to_be_changed==temp->parent->credit)
        //     break;
        // else
        temp->parent->credit=to_be_changed;
        temp=temp->parent;
    }
}

TreeNode* GameTree::allocate(){
    if (tail<maxsize-1)
        return (&buffer[tail++]);
    else
        return (new TreeNode());
}

int main(){
    setvbuf(stdin, new char[1 << 20], _IOFBF, 1 << 20);
    setvbuf(stdout, new char[1 << 20], _IOFBF, 1 << 20);
    #ifdef DEBUG
    freopen("input.txt","r",stdin);
    freopen("output.txt","w",stdout);
    #endif
    
    int n=0;
    scanf("%d",&n);
    GameTree* gametree=new GameTree((n*64)<<1);
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
        }
        lastone=current;
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