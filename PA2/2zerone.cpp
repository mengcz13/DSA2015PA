#include <cstdio>

#define MAX 64000000
#define MAXSTR (64+1)
#define IOBUFFER 1<<20

//#define DEBUG

//为了节省空间采用位运算+手工模拟地址...0~25位存储地址，29位存储胜负，30位存储深度奇偶性，31位存储是否初始化（不过好像没用到）
//由于总结点不超过64*10^6<2^26所以这样是可以覆盖所有节点的
#define ADDR(x) (x & ((1<<26)-1))
#define CREDIT(x) ((x>>29) & 1)
#define DEPTH(x) ((x>>30) & 1)
#define INIT(x) ((x>>31) & 1)
struct TreeNode{
    int parent_addr;
    int lc_addr;
    int rc_addr;
    TreeNode():parent_addr(0),lc_addr(0),rc_addr(0){}

    int parent(){return ADDR(parent_addr);}
    int lc(){return ADDR(lc_addr);}
    int rc(){return ADDR(rc_addr);}
    int credit(){return CREDIT(parent_addr);}
    int depth(){return DEPTH(parent_addr);}
    int if_init(){return INIT(parent_addr);}
    void set_depth(int dep){
        if (dep)
            parent_addr|=(1<<30);
        else
            parent_addr&=(~(1<<30));
    }
    void set_credit(int credit){
        if (credit)
            parent_addr|=(1<<29);
        else
            parent_addr&=(~(1<<29));
    }
    void set_parent(int paddr){
        parent_addr|=paddr;
    }
    void set_rc(int rcaddr){
        rc_addr=rcaddr;
    }
    void set_lc(int lcaddr){
        lc_addr=lcaddr;
    }
};

class GameTree{
private:
    TreeNode* buffer;
    int tail;
    int maxsize;

public:
    GameTree(int size);
    ~GameTree();
    void insert_string(char*);//插入新串
    void update(int);//从插入点向上更新胜负值
    int allocate();//从buffer分配一单位节点空间
    int if_adam_win(){return buffer[0].credit();}
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
    tail=1;
}

GameTree::~GameTree(){
    delete []buffer;
}

void GameTree::insert_string(char* seq){
    int p=0;
    int par=0;
    while (seq[p]>47){
        int prc=buffer[par].rc();
        int plc=buffer[par].lc();
        if ((~seq[p])&1){
            if (prc!=0)
                par=prc;
            else{
                int ch=allocate();
                buffer[par].set_rc(ch);
                buffer[ch].set_parent(par);
                buffer[ch].set_depth(1-buffer[par].depth());
                par=ch;
            }
        }
        else{
            if (plc!=0)
                par=plc;
            else{
                int ch=allocate();
                //par->insert_child(ch,0);
                buffer[par].set_lc(ch);
                buffer[ch].set_parent(par);
                buffer[ch].set_depth(1-buffer[par].depth());
                par=ch;
            }
        }
        p++;
    }
    if (buffer[par].lc()==0 && buffer[par].rc()==0){
        if (buffer[par].depth())
            buffer[par].set_credit(1);
        else
            buffer[par].set_credit(0);
        update(par);
    }
}

//自底向上更新胜负情况
void GameTree::update(int latest){
    while (latest!=0){
        int to_be_changed=0;
        if (buffer[latest].depth()){
            int lcredit=(buffer[buffer[latest].parent()].lc()==0)?0:buffer[buffer[buffer[latest].parent()].lc()].credit();
            int rcredit=(buffer[buffer[latest].parent()].rc()==0)?0:buffer[buffer[buffer[latest].parent()].rc()].credit();
            to_be_changed=(lcredit|rcredit);
        }
        else{
            int lcredit=(buffer[buffer[latest].parent()].lc()==0)?1:buffer[buffer[buffer[latest].parent()].lc()].credit();
            int rcredit=(buffer[buffer[latest].parent()].rc()==0)?1:buffer[buffer[buffer[latest].parent()].rc()].credit();
            to_be_changed=(lcredit&rcredit);
        }
        buffer[buffer[latest].parent()].set_credit(to_be_changed);
        latest=buffer[latest].parent();
    }
}

int GameTree::allocate(){
    if (tail<maxsize)
        return (tail++);
    else
        return (-1);
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
    GameTree* gametree=new GameTree((64*n));
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

    delete gametree;
    delete str;

    return 0;
}