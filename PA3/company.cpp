#include <stdio.h>

void swap(long long& a, long long& b) {
    long long t = a;
    a = b;
    b = t;
}

void quicksort(long long* array, int sta, int end) {
    if (end - sta <= 1)
        return;
    int pivot = sta;
    int p = sta + 1;
    for (int j = sta + 1; j < end; ++j) {
        if (array[j] < array[pivot]) {
            swap(array[j], array[p]);
            ++p;
        }
    }
    --p;
    swap(array[pivot], array[p]);
    quicksort(array, sta, p);
    quicksort(array, p + 1, end);
}

int uniquify(long long* array, int sta, int end) {
    quicksort(array, sta, end);
    int p = sta + 1;
    for (int j = sta + 1; j < end; ++j) {
        if (array[j] != array[p - 1]) {
            swap(array[p], array[j]);
            ++p;
        }
    }
    return p;
}

int binary_search(long long* array, int sta, int end, long long target) {
    while (end - sta > 0) {
        int mid = (sta + end) >> 1;
        if (target < array[mid])
            end = mid;
        else
            sta = mid + 1;
    }
    return (sta - 1);
}

//Bitmap类，可实现O（1）时间内的初始化。由于输入数据员工编号范围过大，故已进行离散化处理。
class Bitmap {
private:
    int* code;//散列表存储员工code
    int* rank;//散列表存储员工编号在栈中的位置，例如rank[i]表示的是编号为i的员工在stack中的秩
    int* stack;//员工编号栈，存储员工的编号，与rank一起起到校验作用。若rank[i]中的值不在stack有效范围，则必然员工i从未登录；若在有效范围且stack[rank[i]]==i，则必然i在线；若在有效范围且stack[rank[i]]==-i，则员工i必然曾经登录但已下线。遵循这一原则设计login, logout和valid检测。
    int stacktail;
    int max_map_size;
    int activenum;

public:
    Bitmap(int size);
    ~Bitmap();
    void login(int a, int c);
    void logout(int a);
    void close();
    int number();
    int query(int a);

    int valid(int a);
};

Bitmap::Bitmap(int size) {
    code = new int[size];
    rank = new int[size];
    stack = new int[size];
    stacktail = 0;
    max_map_size = size;
    activenum = 0;
}

Bitmap::~Bitmap() {
    delete []code;
    delete []rank;
    delete []stack;
}

void Bitmap::login(int a, int c) {
    int validtype = valid(a);
    if (validtype == 0) { // never
        code[a] = c;
        stack[stacktail++] = a;
        rank[a] = stacktail - 1;
        ++activenum;
    }
    else if (validtype == 1) { // now
        code[a] = c;
    }
    else { // used but not now
        code[a] = c;
        stack[rank[a]] = a;
        ++activenum;
    }
}

void Bitmap::logout(int a) {
    int validtype = valid(a);
    if (validtype == 1) {
        stack[rank[a]] = -a;
        --activenum;
    }
}

void Bitmap::close() {
    stacktail = 0;
    activenum = 0;
}

int Bitmap::number() {
    return activenum;
}

int Bitmap::query(int a) {
    int validtype = valid(a);
    if (validtype == 1) {
        return code[a];
    }
    else
        return -1;
}

int Bitmap::valid(int a) {
    //0: NEVER BEEN INSERTED; 1: VALID; 2: HAVE BEEN INSERTED BUT INVALID NOW
    if (rank[a] >= 0 && rank[a] < stacktail && stack[rank[a]] == a)
        return 1;
    else if (rank[a] >= 0 && rank[a] < stacktail && stack[rank[a]] == -a)
        return 2;
    else
        return 0;
}

struct Order {
    char type;
    long long a;
    int c;
};

int main() {
    setvbuf(stdin, new char[1 << 20], _IOFBF, 1 << 20);
    setvbuf(stdout, new char[1 << 20], _IOFBF, 1 << 20);
    long long n = 0, anssum = 0;
    int m = 0;
    scanf("%lld%d", &n, &m);
    Bitmap bitmap(m);

    //离散化处理long long范围的员工编号（不超过500000）
    Order* orderarray = new Order[m];
    long long* nolist = new long long[m];
    int nolisttail = 0;
    for (int i = 0; i < m; ++i) {
        getchar();
        scanf("%c", &orderarray[i].type);
        if (orderarray[i].type == 'I') {
            scanf("%lld%d", &orderarray[i].a, &orderarray[i].c);
            nolist[nolisttail++] = orderarray[i].a;
        }
        else if (orderarray[i].type == 'O' || orderarray[i].type == 'Q') {
            scanf("%lld", &orderarray[i].a);
            nolist[nolisttail++] = orderarray[i].a;
        }
    }
    int stlen = uniquify(nolist, 0, nolisttail);

    for (int i = 0; i < m; ++i) {
        if (orderarray[i].type == 'I') {
            int disa = binary_search(nolist, 0, stlen, orderarray[i].a);
            bitmap.login(disa, orderarray[i].c);
        }
        else if (orderarray[i].type == 'O') {
            int disa = binary_search(nolist, 0, stlen, orderarray[i].a);
            bitmap.logout(disa);
        }
        else if (orderarray[i].type == 'C') {
            bitmap.close();
        }
        else if (orderarray[i].type == 'N') {
            anssum += (long long)(bitmap.number());
        }
        else if (orderarray[i].type == 'Q'){
            int disa = binary_search(nolist, 0, stlen, orderarray[i].a);
            anssum += (long long)(bitmap.query(disa));
        }
    }
    printf("%lld", anssum);
    delete []orderarray;
    delete []nolist;
    return 0;
}
