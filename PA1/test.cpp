#include <stdio.h>

struct TreeNode{
    bool cd;
    //char credit;
    //char depth;
    TreeNode* parent;
    TreeNode* lc;
    TreeNode* rc;
};

int main(){
    TreeNode tn;
    printf("%u",sizeof(tn));
    int n=0;scanf("%d",&n);
	return 0;
}
