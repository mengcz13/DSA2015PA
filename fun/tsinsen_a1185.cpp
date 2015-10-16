#include <stdio.h>
//#define DEBUG

// BFS+贪心
// 首先从上到下第一行各点均来一遍BFS，判断是否为无解情况
// 若有解则从上到下分别进行BFS（注意每次进行之前初始化），确定每个点覆盖的区间范围，然后进行贪心
// 注意：有解情况下第一行BFS之后或者覆盖最后一行0个点，或者覆盖最后一行的某个区间（反证法）

#define MAX 500

struct Node{
    int x;
    int y;
    int height;
    int if_watered;
    int if_source;
    Node(){}
};

struct NQpoint{
    Node* node;
    NQpoint* succ;
    NQpoint(){}
};

class NodeQueue{
public:
    NodeQueue();
    ~NodeQueue();
    void enqueue(Node*);
    void dequeue();
    Node* top(){return head->succ->node;}
    bool empty(){return (head->succ==tail);}
private:
    NQpoint* head;
    NQpoint* tail;
    NQpoint* last;
};

NodeQueue::NodeQueue(){
    head=new NQpoint;
    tail=new NQpoint;
    head->succ=tail;
    last=head;
}

NodeQueue::~NodeQueue(){
    while (head->succ!=tail)
        dequeue();
    delete head;
    delete tail;
}

void NodeQueue::enqueue(Node* n){
    NQpoint* np=new NQpoint;
    np->node=n;
    last->succ=np;
    np->succ=tail;
    last=np;
}

void NodeQueue::dequeue(){
    NQpoint* first=head->succ;
    if (last==first)
        last=head;
    head->succ=first->succ;
    delete first;
}

bool valid(int x, int y, int maxx, int maxy){
    return (x>=0&&x<maxx&&y>=0&&y<maxy);
}

void select(int ch, int x, int y, int maxx, int maxy, Node** array, NodeQueue* nodelist){
    if (valid(x,y,maxx,maxy)&&(array[x][y].height<ch)){
        if (!array[x][y].if_watered){
            nodelist->enqueue(&array[x][y]);
            array[x][y].if_watered=1;
        }
        else{
            if (array[x][y].if_source)
                array[x][y].if_source=0;
        }
    }
}

void bfs(Node **array, int sx, int sy, int maxx, int maxy){
    NodeQueue nodelist;
    nodelist.enqueue(&array[sx][sy]);
    array[sx][sy].if_source=1;
    array[sx][sy].if_watered=1;
    while (!nodelist.empty()){
        Node* temp=nodelist.top();
        select(temp->height, temp->x-1, temp->y, maxx, maxy, array, &nodelist);
        select(temp->height, temp->x+1, temp->y, maxx, maxy, array, &nodelist);
        select(temp->height, temp->x, temp->y-1, maxx, maxy, array, &nodelist);
        select(temp->height, temp->x, temp->y+1, maxx, maxy, array, &nodelist);
        nodelist.dequeue();
    }
}

int main(){
    #ifdef DEBUG
    freopen("input.txt","r",stdin);
    freopen("output.txt","w",stdout);
    #endif
    int n=0, m=0;
    scanf("%d%d",&n,&m);
    Node** array=new Node*[n];
    for (int i=0;i<n;++i)
        array[i]=new Node[m];
    for (int i=0;i<n;++i)
        for (int j=0;j<m;++j){
            int th=0;
            scanf("%d",&th);
            array[i][j].x=i;
            array[i][j].y=j;
            array[i][j].height=th;
            array[i][j].if_watered=0;
            array[i][j].if_source=0;
        }
    for (int j=0;j<m;++j){
        if (!array[0][j].if_watered){
            bfs(array,0,j,n,m);
        }
    }
    int dryarea=0, factory=0;
    for (int j=0;j<m;++j)
        if (!array[n-1][j].if_watered)
            dryarea++;
    if (dryarea)
        printf("%d\n%d",0,dryarea);
    else{
        int *left=new int[m];
        int *right=new int[m];
        //Greedy
        for (int k=0;k<m;++k){
            for (int i=0;i<n;++i)
                for (int j=0;j<m;++j){
                    array[i][j].if_watered=0;
                    array[i][j].if_source=0;
                }
            bfs(array,0,k,n,m);
            left[k]=-1;
            right[k]=-1;
            if (array[n-1][0].if_watered)
                left[k]=0;
            if (array[n-1][m-1].if_watered)
                right[k]=m-1;
            for (int l=0;l<m-1;++l){
                int diff=array[n-1][l].if_watered-array[n-1][l+1].if_watered;
                if (diff==-1)
                    left[k]=l+1;
                else if (diff==1)
                    right[k]=l;
            }
        }

        #ifdef DEBUG
        for (int i=0;i<m;++i)
            printf("%d ",left[i]);
        puts("\n");
        for (int i=0;i<m;++i)
            printf("%d ",right[i]);
        #endif

        int dp=0;
        int en=0;
        while (en<m){
            int mostright=0, sel=0;
            for (int k=0;k<m;++k){
                if (left[k]>=0 && left[k]<=en && right[k]>=en){
                    if (right[k]>mostright){
                        mostright=right[k];
                        sel=k;
                    }
                }
            }
            left[sel]=-1;
            en=mostright+1;
            dp++;
        }
        printf("%d\n%d",1,dp);
    }
    return 0;
}