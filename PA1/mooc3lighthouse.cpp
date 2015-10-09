#include <stdio.h>

#define MAX 4000000
//#define DEBUG

void swap(int &a, int &b){
    int t=a;
    a=b;
    b=t;
}

void qsort(int *array, int sta, int end, int* byarray){
    if (end-sta<=1)
        return;
    int pivot=array[sta];
    int p=sta+1;
    for (int i=sta+1;i<end;++i){
        if (array[i]<pivot){
            swap(array[i],array[p]);
            swap(byarray[i],byarray[p]);
            p++;
        }
    }
    p--;
    swap(array[sta],array[p]);
    swap(byarray[sta],byarray[p]);
    qsort(array,sta,p,byarray);
    qsort(array,p+1,end,byarray);
}

//本题实质上求各点按照横坐标顺序排列后非逆序对的个数
//利用归并排序求逆序对个数, 每次归并均可求出横跨左右子序列的逆序对个数: 当归并时复制右子序列时, 表明左子序列剩余的各个元素与被复制元素均构成逆序对
long long mergesort(int *array, int sta, int end, int* buffer){
    #ifdef DEBUG
    printf("%d %d\n",sta,end);
    #endif
    if (end-sta<=1)
        return 0;
    else{
        int mid=(sta+end)>>1;
        long long p1=mergesort(array,sta,mid,buffer);
        long long p2=mergesort(array,mid,end,buffer);
        for (int i=sta;i<mid;++i)
            buffer[i]=array[i];
        int i=sta, j=mid, k=sta;
        long long inter=0;
        while(i<mid&&j<end){
            if (buffer[i]>array[j]){
                array[k]=array[j];
                j++;
                inter+=(long long)(mid-i);
            }
            else{
                array[k]=buffer[i];
                i++;
            }
            k++;
        }
        if (i==mid){
            
        }
        else{
            for (;k<end;k++){
                array[k]=buffer[i];
                i++;
            }
        }
        return (p1+p2+inter);
    }
}

int main(){
    setvbuf(stdin, new char[1 << 20], _IOFBF, 1 << 20);
    #ifdef DEBUG
    freopen("input.txt","r",stdin);
    freopen("output.txt","w",stdout);
    #endif
    int *buffer=new int[MAX];
    int *array=new int[MAX];
    int *byarray=new int[MAX];
    int n=0;
    scanf("%d", &n);
    for (int i=0;i<n;++i)
        scanf("%d%d",&array[i],&byarray[i]);
    qsort(array,0,n,byarray);
    long long total=n;
    total=total*(total-1)/2;
    printf("%lld",total-mergesort(byarray,0,n,buffer));
    delete []buffer;
    delete []array;
    delete []byarray;
    return 0;
}