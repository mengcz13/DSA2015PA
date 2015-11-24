#include <stdio.h>
#define MAX 500000
long long mergesort(int *array, int sta, int end, int* buffer){
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
    setvbuf(stdout, new char[1 << 20], _IOFBF, 1 << 20);
    int *buffer=new int[MAX];
    int *array=new int[MAX];
    int n=0;
    scanf("%d", &n);
    while (n) {
        int tail = 0;
        for (int i = 0; i < n; ++i) {
            scanf("%d", &array[tail++]);
        }
        printf("%lld\n", mergesort(array, 0, n, buffer));
        scanf("%d", &n);
    }
    delete []buffer;
    delete []array;
    return 0;
}