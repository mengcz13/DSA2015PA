#include <stdio.h>
#include <string.h>
#define HASHD 33
#define HASHSIZE 262741 //size of hashtable

struct Node { //哈希表单元，存储字符串头指针，字符串的出现序号，采用独立链方法解决冲突
    char* str;
    int rank;
    Node* next;
};

class HashTable {
private:
    Node* hashtable; //哈希表
    char* strpool;
    int strtail;
    Node* nodepool;
    int nodetail;

    int wheellen;
    int strnum;
    int hashsize;
    int hashd;

    int seq;

    int* DM;
    char* temp;
public:
    HashTable(int n, int m, int hs, int hd) {
        wheellen = n;
        strnum = m;
        hashsize = hs;
        hashd = hd;
        seq = 1;
        strtail = 0;
        nodetail = 0;

        hashtable = new Node[hashsize];
        strpool = new char[(strnum + 1) * (wheellen + 1)];
        nodepool = new Node[hashsize << 1];
        temp = new char[(wheellen + 1) << 1];
        DM = new int[wheellen];
        DM[0] = 1;
        for (int i = 1; i < wheellen; ++i) {
            DM[i] = (DM[i - 1] * hashd) % hashsize;
        }
        memset(hashtable, 0, hashsize * sizeof(Node));
        memset(nodepool, 0, (hashsize << 1) * sizeof(Node));
        memset(strpool, 0, ((strnum + 1) * (wheellen + 1)) * sizeof(char));
        memset(temp, 0, ((wheellen + 1) << 1) * sizeof(char));
    }

    ~HashTable() {
        delete []hashtable;
        delete []strpool;
        delete []DM;
        delete []temp;
    }

    int char2digit(char c) { //定义字符的值为其ASCII码值
        return (int)c;
    }

    int hashcode(char* str) { //多项式哈希函数
        int hc = 0;
        for (int i = 0; i < wheellen; ++i) {
            hc = (hc * hashd + char2digit(str[i])) % hashsize;
        }
        return hc;
    }

    bool bfcmp(char* a, char* b) { //暴力比较，O(len)时间，由于仅在冲突时比较且串长度相等，故采用暴力比较不会过分影响效率
        for (int i = 0; i < wheellen; ++i) {
            if (a[i] != b[i])
                return false;
        }
        return true;
    }

    Node* find_hashtable(char* str) { //找到当前字符串在哈希表中的位置，如果没有则返回NULL
        for (int i = 0; i < wheellen; ++i) {
            temp[i] = str[i];
            temp[i + wheellen] = temp[i];
        }
        char* ptstr = &temp[0];
        int hc = hashcode(ptstr);
        for (int i = 0; i < wheellen; ++i) {
            if (hashtable[hc].str != NULL) {
                Node* p = &hashtable[hc];
                while (p != NULL) {
                    if (bfcmp(p->str, ptstr)) { //这里的策略比较保守，如果哈希值匹配仍然要比较后确认，一定程度上影响了效率
                        return p;
                    }
                    p = p->next;
                }
            }
            //计算循环移位后的哈希值，O(1)时间
            int left = hc - char2digit(temp[i]) * DM[wheellen - 1];
            while (left < 0)
                left += hashsize;
            hc = (left * hashd + char2digit(temp[i + wheellen])) % hashsize;
            ptstr = &temp[i + 1];
        }
        return NULL;
    }

    int readstring(char* str) { //查找字符串，若匹配则返回出现序号，否则添加进哈希表
        Node* p = find_hashtable(str);
        if (p != NULL) {
            ++seq;
            return (p->rank);
        }
        else {
            int hc = hashcode(str);
            if (hashtable[hc].str == NULL) {
                hashtable[hc].str = &strpool[strtail * (wheellen + 1)];
                ++strtail;
                hashtable[hc].rank = seq;
                hashtable[hc].next = NULL;
                for (int i = 0; i < wheellen; ++i) {
                    hashtable[hc].str[i] = str[i];
                }
                hashtable[hc].str[wheellen] = 0;
            }
            else {
                Node* hot = &hashtable[hc];
                while (hot->next != NULL) {
                    hot = hot->next;
                }
                hot->next = &nodepool[nodetail++];
                hot->next->str = &strpool[strtail * (wheellen + 1)];
                ++strtail;
                hot->next->rank = seq;
                hot->next->next = NULL;
                for (int i = 0; i < wheellen; ++i) {
                    hot->next->str[i] = str[i];
                }
                hot->next->str[wheellen] = 0;
            }
            ++seq;
            return 0;
        }
    }
};

int main() {
    setvbuf(stdin, new char[1 << 20], _IOFBF, 1 << 20);
    setvbuf(stdout, new char[1 << 20], _IOFBF, 1 << 20);
    int n = 0, m = 0;
    scanf("%d%d", &n, &m);
    getchar();
    HashTable ht(n, m, HASHSIZE, HASHD);
    char* tempstr = new char[n << 1];
    memset(tempstr, 0, sizeof(char) * (n << 1));
    for (int i = 0; i < m; ++i) {
        scanf("%s", tempstr);
        printf("%d\n", ht.readstring(tempstr));
    }
    delete []tempstr;
    return 0;
}