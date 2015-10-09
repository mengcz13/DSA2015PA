#include <stdio.h>

#define MAX 200000+5
//计算叉积
inline long long cross(long long x0, long long y0, long long x, long long y){
	return (x0*y0-x0*y-x*y0);
}

int findb(int len, int x, int y, long long*xarray, long long*yarray){
	int start=0, end=len, mid=0;
	while (end-start>0){
		mid=(start+end)/2;
		if (0<cross(xarray[mid],yarray[mid],x,y))//叉积大于0说明线在点右侧
			end=mid;
		else
			start=mid+1;
	}
	start--;
	return start;
}
//快排，用于输入的x和y，构建从左到右的线段集合
void qsort_xy(long long* array, int start, int end){
	if (end-start<=1)
		return;
	long long pivot=array[start];
	int p=start+1;
	for (int i=start+1;i<end;++i){
		if (array[i]<pivot){
			if (i!=p){
				long long t=array[p];array[p]=array[i];array[i]=t;
			}
			p++;
		}
	}
	p--;
	long long t=array[p];array[p]=array[start];array[start]=t;
	qsort_xy(array,start,p);
	qsort_xy(array,p+1,end);
}

int main(){
	setvbuf(stdin, new char[1 << 20], _IOFBF, 1 << 20);
	setvbuf(stdout, new char[1 << 20], _IOFBF, 1 << 20);
	long long* xarray=new long long[MAX];
	long long* yarray=new long long[MAX];
	int n=0, m=0;
	scanf("%d",&n);
	for (int i=0;i<n;++i){
		scanf("%lld",&xarray[i]);
	}
	for (int i=0;i<n;++i){
		scanf("%lld",&yarray[i]);
	}
	qsort_xy(xarray,0,n);
	qsort_xy(yarray,0,n);
	scanf("%d",&m);
	long long x=0, y=0;
	for (int i=0;i<m;++i){
		scanf("%lld%lld",&x,&y);
		printf("%d\n",findb(n,x,y,xarray,yarray)+1);
	}
	delete []xarray;
	delete []yarray;
	return 0;
}

