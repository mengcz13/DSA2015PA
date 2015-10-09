#include <stdio.h>
#include <math.h>

#define MAX 100000+5

//#define DEBUG
//二分查找
int binary_search(int array[], int len, int tar){
	int head=0, tail=len;
	while ((tail-head)>0){
		int m=(head+tail)/2;
		if (tar<array[m])
			tail=m;
		else
			head=m+1;
	}
	head--;
	return head;
}
//快排
void sort_h_ill(int start, int end, int* raw_h, char* raw_ill){
	if (end-start<=1)
		return;
	int pivot=raw_h[start];
	int p=start+1;
	for (int i=start+1;i<end;++i){
		if (raw_h[i]>pivot){
			if (i!=p){
				int th=raw_h[p];raw_h[p]=raw_h[i];raw_h[i]=th;
				char tc=raw_ill[p];raw_ill[p]=raw_ill[i];raw_ill[i]=tc;
			}
			p++;
		}
	}
	p--;
	int th=raw_h[p];raw_h[p]=raw_h[start];raw_h[start]=th;
	char tc=raw_ill[p];raw_ill[p]=raw_ill[start];raw_ill[start]=tc;
	sort_h_ill(start,p,raw_h,raw_ill);
	sort_h_ill(p+1,end,raw_h,raw_ill);
}

int main(){
	setvbuf(stdin, new char[1 << 20], _IOFBF, 1 << 20);
	setvbuf(stdout, new char[1 << 20], _IOFBF, 1 << 20);
	int* raw_h=new int[MAX];//原始高度数据
	char* raw_ill=new char[MAX];//原始是否患病符号
	int* height=new int[MAX];//存储唯一化后的高度序列
	int* rightnum=new int[MAX];//存储某高度以上健康数量
	int* wrongnum=new int[MAX];//存储某高度以上染病数量，以上3个数组配合使用
	int n=0, maxh=0;
	scanf("%d%d",&n,&maxh);
	int right=0, wrong=0;
	char c=0;
	for (int i=0;i<n;++i){
		scanf("%d %c",&raw_h[i],&raw_ill[i]);
	}
	sort_h_ill(0,n,raw_h,raw_ill);//将输入高度从高到低排序
	//对输入数据根据高度归类，计算每种高度以上健康和患病个体数量
	int currenth=raw_h[0], point=0, currentright=0, currentwrong=0;
	for (int i=0;i<n;++i){
		if (raw_h[i]!=currenth){
			height[point]=currenth;
			rightnum[point]=currentright;
			wrongnum[point]=currentwrong;
			point++;
			currenth=raw_h[i];
		}
		if (raw_ill[i]=='+')
			currentright++;
		else
			currentwrong++;
	}
	height[point]=currenth;
	rightnum[point]=currentright;
	wrongnum[point]=currentwrong;
	point++;

#ifdef DEBUG
	for (int i=0;i<point;++i){
		printf("%d %d %d\n",height[i],rightnum[i],wrongnum[i]);
	}
#endif

	int totalright=rightnum[point-1];
	int totalwrong=wrongnum[point-1];
	int m=0;
	double phit=0, pfalse=0;
	int minright=0, maxwrong=0;
	int rL=0, rH=0, hL=0, hH=0;
	scanf("%d",&m);
	for (int i=0;i<m;++i){
		scanf("%lf%lf",&phit,&pfalse);
		minright=(int)ceil(phit*totalright);
		maxwrong=(int)floor(pfalse*totalwrong);
#ifdef DEBUG
		printf("minright:%d, maxwrong:%d\n",minright,maxwrong);
#endif
		//deciding hL,注意下界应为查找结果+1
		rL=binary_search(wrongnum,point,maxwrong);
		if (rL==-1)
			hL=height[0]+1;
		else if (rL==point-1){
			hL=0;
		}
		else
			hL=height[rL+1]+1;
		//deciding hH,直接返回查找结果
		rH=binary_search(rightnum,point,minright);
		if (rH==-1){
			if (minright==0)
				hH=maxh;
			else{
				hH=height[0];
			}
		}
		else{
			if (rightnum[rH]<minright){
				rH++;
			}
			else{
				while (rH>=0&&rightnum[rH]==minright){
					rH--;
				}
				rH++;
			}
			if (rH==0){
				if (minright==0)
					hH=maxh;
				else
					hH=height[rH];
			}
			else
				hH=height[rH];
		}
#ifdef DEBUG
		printf("%d %d\n",hL,hH);
#else
		if (hL<=hH)
			printf("%d %d\n",hL,hH);
		else
			printf("-1\n");
#endif
	}
	delete []raw_h;//原始高度数据
	delete []raw_ill;//原始是否患病符号
	delete []height;//存储唯一化后的高度序列
	delete []rightnum;//存储某高度以上健康数量
	delete []wrongnum;//存储某高度以上染病数量，以上3个数组配合使用
	return 0;
}































