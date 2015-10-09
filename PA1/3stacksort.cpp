#include <stdio.h>

#define MAX 5000000+5
//自己定义的栈类，包含输出
class IStack{
private:
	int* vec;
	int topr;
	int maxsize;
public:
	IStack(int n);
	~IStack();
	bool empty(){if (0==topr) return true; else return false;}
	int top(){if (!empty()) return vec[topr-1]; else return -1;}
	void push(int x);
	int pop();
	int get_max();//取得栈内元素最大值
};

IStack::IStack(int n){
	vec=new int[n];
	topr=0;
	maxsize=n;
}

IStack::~IStack(){
	delete []vec;
}

void IStack::push(int x){
	vec[topr]=x;
	topr++;
}

int IStack::pop(){
	int t=this->top();
	topr--;
	return t;
}

int IStack::get_max(){
	int m=vec[0];
	for (int i=0;i<topr;++i){
		if (vec[i]>m)
			m=vec[i];
	}
	return m;
}
//比较辅助栈栈顶元素和主栈内最大元素并决定插入哪个元素
int output_one(IStack* s0, IStack* s1){
	int m0=s0->get_max();
	int mt1=-1;
	if (!s1->empty())
		mt1=s1->top();
	if (mt1>m0){
		return s1->pop();
	}
	else{
		int t=s0->pop();
		while (t!=m0){
			s1->push(t);
			t=s0->pop();
		}
		return t;
	}
}

int main(){
	setvbuf(stdin, new char[1 << 20], _IOFBF, 1 << 20);
	setvbuf(stdout, new char[1 << 20], _IOFBF, 1 << 20);
	IStack* s0=new IStack(MAX);
	IStack* s1=new IStack(MAX);
	int* temps=new int[MAX];
	int n=0, tempr=0;
	scanf("%d",&n);
	for (tempr=0;tempr<n;++tempr){
		scanf("%d",&temps[tempr]);
	}
	for (int i=tempr-1;i>=0;--i){
		s0->push(temps[i]);
	}
	while (!s0->empty()){
		printf("%d ",output_one(s0,s1));
	}
	while (!s1->empty()){
		printf("%d ",s1->pop());
	}
	delete s0;
	delete s1;
	delete []temps;
	return 0;
}














