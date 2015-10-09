#include <stdio.h>

#define MAXN 5000+10
#define J 8 //最大压位数为J

int POWER[9]={1,10,100,1000,10000,100000,1000000,10000000,100000000};
//字符串转换为整数同时压位
int str2n(char s[], int ds, long long num[]){
	int digit=0;
	int k=0;
	for (int i=ds-1;i>=0;i--){
		digit+=POWER[(ds-1-i)%J]*(s[i]-'0');
		if ((ds-1-i)%J==J-1||i==0){
			num[k]=digit;
			digit=0;
			k++;
		}
	}
	return k;
}
//高精度乘法
int highmul(long long n1[],int l1, long long n2[], int l2, long long res[]){
	for (int i=0;i<=l1+l2;++i){
		res[i]=0;
	}
	for (int i=0;i<l2;++i){
		for (int j=0;j<l1;++j){
			res[i+j]+=n2[i]*n1[j];
		}
	}
	for (int i=0;i<l1+l2-1;++i){
		res[i+1]+=res[i]/(POWER[J-1]*10);
		res[i]=res[i]%(POWER[J-1]*10);
	}
	int s=0;
	for (s=l1+l2;s>=0;s--){
		if (res[s]!=0)
			break;
	}
	return s+1;
}

int main(){
	setvbuf(stdin, new char[1 << 20], _IOFBF, 1 << 20);
	setvbuf(stdout, new char[1 << 20], _IOFBF, 1 << 20);
	int n=0;
	char a[MAXN];
	char b[MAXN];
	char ct;
	long long n1[MAXN/J+2];
	long long n2[MAXN/J+2];
	long long res[2*(MAXN/J)+10];
	scanf("%d",&n);
	getchar();
	for (int i=0;i<n;++i){
		int sl1=0, sl2=0;
		ct=getchar();
		while(ct!=' '){
			a[sl1]=ct;
			sl1++;
			ct=getchar();
		}
		ct=getchar();
		while(ct!='\n'){
			b[sl2]=ct;
			sl2++;
			ct=getchar();
		}
		int l1=str2n(a,sl1,n1);
		int l2=str2n(b,sl2,n2);
		int tl=highmul(n1,l1,n2,l2,res);
		printf("%lld",res[tl-1]);
		for (int i=tl-2;i>=0;i--){
			int zer=0;
			for (zer=J-1;zer>0;zer--){
				if (res[i]/POWER[zer]!=0)
					break;
			}
			zer=(J-1)-zer;
			for (int z=0;z<zer;++z)
				printf("%d",0);
			printf("%lld",res[i]);
		}
		printf("\n");
	}
	return 0;
}
