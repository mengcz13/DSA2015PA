#include <stdio.h>

#define MAXNUM 10000000+5

//测试开关，与低效算法比对用
//#define DEBUG

class VecQueue{
private:
    int* vec;
    int first;
    int last;
    int maxsize;
public:
    VecQueue(int n);
    ~VecQueue();
    bool if_empty(){return (first==last);}
    bool if_full(){return ((last+1)%maxsize == first);}
    int enqueue(int x);
    int dequeue();//警告：返回值表示是否成功，不返回元素!!!
    int dequeue_from_bottom();
    int& top(){return vec[first];}
    int bottom(){return vec[(last-1)%maxsize];}
    int& operator[](int rank){return vec[(first+rank)%maxsize];}
    int size(){return (last-first)%maxsize;}

    //低效算法，对比测试用
    #ifdef DEBUG
    int max(){
        if (this->if_empty())
            return 0;
        int m=vec[first];
        int i=first;
        while (i!=last){
            if (vec[i]>m)
                m=vec[i];
            i=(i+1)%maxsize;
        }
        return m;
    }
    #endif

};

VecQueue::VecQueue(int n){
    vec=new int[n+1];
    first=0;//队列首元素
    last=0;//新入队元素待插入位置
    maxsize=n+1;
}

VecQueue::~VecQueue(){
    delete []vec;
}

int VecQueue::enqueue(int x){
    if (!this->if_full()){
        vec[last]=x;
        last=(last+1)%maxsize;
        return 0;
    }
    else
        return -1;
}

int VecQueue::dequeue(){
    if (!this->if_empty()){
        first=(first+1)%maxsize;
        return 0;
    }
    else
        return -1;
}

int VecQueue::dequeue_from_bottom(){
    if (!this->if_empty()){
        last=(last-1)%maxsize;
        return 0;
    }
    else
        return -1;
}

int main(){

    setvbuf(stdin, new char[1 << 20], _IOFBF, 1 << 20);
    setvbuf(stdout, new char[1 << 20], _IOFBF, 1 << 20);

    #ifdef DEBUG
    freopen("input.txt","r",stdin);
    freopen("output.txt","w",stdout);
    #endif

    int n=0;
    long long daynum=0, stocksum=0;
    scanf("%d",&n);

    #ifdef DEBUG
    VecQueue* current_stock=new VecQueue(n<<1);//当前市场上股票队列，对比测试用
    #endif

    VecQueue* densed_max=new VecQueue(n<<1);//最大值队列，已压缩
    VecQueue* densed_max_num=new VecQueue(n<<1);//最大值队列，存储压缩后每个值的个数
    int lognum=(n<<1);
    int stock_in_hand=0;//当前时间段持有股票数
    for (int i=0;i<lognum;++i){
        int xstocknum=0;
        long long daybreak=0;
        scanf("%lld",&daybreak);
        daynum+=daybreak;
        stocksum+=daybreak*stock_in_hand;

        char c=getchar();
        if (c==' '){//上市
            scanf("%d",&xstocknum);

            #ifdef DEBUG
            current_stock->enqueue(xstocknum);
            #endif

            //更新最大值
            int newsum=1;
            while (!densed_max->if_empty()){
                if (densed_max->bottom()<=xstocknum){
                    newsum+=densed_max_num->bottom();
                    densed_max->dequeue_from_bottom();
                    densed_max_num->dequeue_from_bottom();
                }
                else
                    break;
            }
            densed_max->enqueue(xstocknum);
            densed_max_num->enqueue(newsum);
        }
        else{//退市

            #ifdef DEBUG
            current_stock->dequeue();
            #endif

            int temp=densed_max_num->top();
            densed_max_num->top()=temp-1;
            if (densed_max_num->top()==0){
                densed_max->dequeue();
                densed_max_num->dequeue();
            }
        }
        //股票在过程中也可能为空TAT没读懂题……
        if (!densed_max->if_empty())
            stock_in_hand=densed_max->top();
        else
            stock_in_hand=0;

        #ifdef DEBUG
        printf("to be tested: %d \t correct: %d\t diff: %d\n",stock_in_hand,current_stock->max(),stock_in_hand-current_stock->max());
        #endif
    }
    double average=static_cast<double>(stocksum)/static_cast<double>(daynum);
    printf("%.2lf",average);

    #ifdef DEBUG
    delete current_stock;
    #endif

    delete densed_max;
    delete densed_max_num;
    return 0;
}