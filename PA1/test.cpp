#include <stdio.h>

int main(){
    setvbuf(stdin, new char[1<<20], _IOFBF, 1<<20);
    int n=0;
    scanf("%d",&n);
    char* buf=new char[(16*64*n)>>1];
    fread(buf,1,(16*64*n)>>1,stdin);
    fread(buf,1,(16*64*n)>>1,stdin);
	return 0;
}
