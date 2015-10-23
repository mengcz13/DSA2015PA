#include <stdio.h>
#define IOBUFFER 1<<20

struct TreeNode{
    int a;
    int b;
    int c;
    //int d;
};

int main(){
    setvbuf(stdin, new char[IOBUFFER], _IOFBF, IOBUFFER);
    setvbuf(stdout, new char[IOBUFFER], _IOFBF, IOBUFFER);
    TreeNode* tna=new TreeNode[64000000];
    int n=0;
    scanf("%d",&n);
    getchar();
    char* str=new char[65];
    for (int i=0;i<n;++i)
        gets(str);
	return 0;
}
