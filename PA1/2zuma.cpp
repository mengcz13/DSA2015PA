#include <stdio.h>

#define BL 400000000
//输出缓冲区
char sta[BL+5];
int top=0;

void emit_all(){
	if (top>0){
		sta[top]='\0';
		printf("%s",sta);
		top=0;
	}
}

struct Node{
	char color;
	Node* succ;
	Node* prec;
};

class NodeList{
private:
	Node* head;
	Node* tail;
	
public:
	NodeList();
	~NodeList();
	Node* visit(int n);
	void insertBefore(Node* tar, Node* pos);
	void deleteNode(Node* node);
	void print_all();
	Node* get_head(){return head;}
	Node* get_tail(){return tail;}
	bool if_init;
};

NodeList::NodeList(){
	head=new Node;
	tail=new Node;
	head->color='$';
	head->prec=NULL;
	head->succ=tail;
	tail->color='$';
	tail->prec=head;
	tail->succ=NULL;
	if_init=true;
}

NodeList::~NodeList(){
	head->succ=NULL;
	tail->prec=NULL;
	delete head;
	delete tail;
}

Node* NodeList::visit(int n){
	if (n==0)
		return head->succ;
	Node* tar=head;
	int r=-1;
	while (tar->succ!=NULL){
		tar=tar->succ;
		r++;
		if (n==r)
			break;
	}
	return tar;
}

void NodeList::insertBefore(Node* tar, Node* pos){
	tar->succ=pos;
	tar->prec=pos->prec;
	tar->prec->succ=tar;
	tar->succ->prec=tar;
	if (if_init)
		return;
	//Clear 3-lasting nodes
	Node* left;
	Node* right;
	if (tar->color==tar->prec->color){
		if (tar->prec->color==tar->prec->prec->color){
			left=tar->prec->prec->prec;
			right=tar->succ;
		}
		else if (tar->prec->color==tar->succ->color){
			left=tar->prec->prec;
			right=tar->succ->succ;
		}
		else
			return;
	}
	else if (tar->color==tar->succ->color){
		if (tar->succ->color==tar->succ->succ->color){
			left=tar->prec;
			right=tar->succ->succ->succ;
		}
		else
			return;
	}
	else
		return;
	for (int i=0;i<3;++i){
		deleteNode(left->succ);
	}
	//检查插入处是否可以继续消除，若可以则批量删除
	while (left->color==right->color){
		if (left==head)
			return;
		else{
			int same=2;
			while (left->prec->color==left->color){
				same++;
				left=left->prec;
			}
			while (right->succ->color==right->color){
				same++;
				right=right->succ;
			}
			if (same>2){
				left=left->prec;
				right=right->succ;
				left->succ=right;
				right->prec=left;
			}
			else
				return;
		}
	}
}

void NodeList::deleteNode(Node* node){
	node->prec->succ=node->succ;
	node->succ->prec=node->prec;
	node->prec=NULL;
	node->succ=NULL;
}

void NodeList::print_all(){
	Node* t=head;
	if (head->succ==tail){
		sta[top]='-';
		top++;
		sta[top]='\n';
		top++;
		if (top>BL){
			sta[top]='\0';
			printf("%s",sta);
			top=0;
		}
		return;
	}
	while (t->succ!=tail){
		t=t->succ;
		sta[top]=t->color;
		top++;
		if (top>BL){
			sta[top]='\0';
			printf("%s",sta);
			top=0;
		}
	}
	sta[top]='\n';
	top++;
	if (top>BL){
		sta[top]='\0';
		printf("%s",sta);
		top=0;
	}
}

int main(){
	Node* nodearray=new Node[200000];
	NodeList nodelist;
	int top=0;
	char c=getchar();
	while (c!='\n'){
		nodearray[top].color=c;
		nodearray[top].prec=NULL;
		nodearray[top].succ=NULL;
		nodelist.insertBefore(&nodearray[top],nodelist.get_tail());
		c=getchar();
		top++;
	}
	nodelist.if_init=false;
	int n=0, r=0;
	scanf("%d",&n);
	for (int i=0;i<n;++i){
		scanf("%d",&r);
		getchar();
		c=getchar();
		nodearray[top].color=c;
		nodearray[top].prec=NULL;
		nodearray[top].succ=NULL;
		nodelist.insertBefore(&nodearray[top],nodelist.visit(r));
		top++;
		nodelist.print_all();
		getchar();
	}
	emit_all();
	delete []nodearray;
	return 0;
}





















