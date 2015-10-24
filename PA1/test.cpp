#include <stdio.h>
#define IOBUFFER 1<<20

struct TreeNode{
    TreeNode* lc;
    TreeNode* rc;
};

int main(){
    setvbuf(stdin, new char[IOBUFFER], _IOFBF, IOBUFFER);
    setvbuf(stdout, new char[IOBUFFER], _IOFBF, IOBUFFER);
    TreeNode* tna=new TreeNode[30*250000];
	return 0;
}
