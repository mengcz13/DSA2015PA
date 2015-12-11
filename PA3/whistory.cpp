#include <stdio.h>
#include <string.h>
#define HASHD 33
#define HASHSIZE 262741 //size of hashtable

//#define DEBUG

struct Node {
    char* str;
    int rank;
    Node* next;
};

class HashTable {
private:
    Node* hashtable;
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
        #ifdef DEBUG
        for (int i = 0; i < wheellen; ++i)
            printf("DM[%d]: %d\n", i, DM[i]);
        #endif
    }

    ~HashTable() {
        delete []hashtable;
        delete []strpool;
        delete []DM;
        delete []temp;
    }

    int char2digit(char c) {
        return (int)c;
    }

    int hashcode(char* str) {
        int hc = 0;
        for (int i = 0; i < wheellen; ++i) {
            hc = (hc * hashd + char2digit(str[i])) % hashsize;
        }
        return hc;
    }

    int get_min_hashcode(char* str, int &startrank) {
        int hc = hashcode(str);
        int minhc = hc;
        startrank = 0;
        for (int i = 0; i < wheellen; ++i) {
            #ifdef DEBUG
            printf("%d\n", hc);
            #endif
            if (hc < minhc) {
                minhc = hc;
                startrank = i;
            }
            int left = hc - char2digit(str[i]) * DM[wheellen - 1];
            while (left < 0)
                left += hashsize;
            hc = (left * hashd + char2digit(str[(i + wheellen) % wheellen])) % hashsize;
        }
        return minhc;
    }

    bool bfcmp(char* a, int starta, char* b, int startb) {
        int pa = starta, pb = startb;
        for (int i = 0; i < wheellen; ++i) {
            if (a[pa] != b[pb])
                return false;
            pa = (pa + 1) % wheellen;
            pb = (pb + 1) % wheellen;
        }
        return true;
    }

    Node* find_hashtable(char* str, int &minhc, int &startrank, Node* &hot) {
        startrank = 0;
        minhc = get_min_hashcode(str, startrank);
        if (hashtable[minhc].str != NULL) {
            Node* p = &hashtable[minhc];
            while (p != NULL) {
                if (bfcmp(p->str, 0, str, startrank)) {
                    return p;
                }
                hot = p;
                p = p->next;
            }
        }
        return NULL;
    }

    int readstring(char* str) {
        int minhc = 0, startrank = 0;
        Node* hot = NULL;
        Node* p = find_hashtable(str, minhc, startrank, hot);
        if (p != NULL) {
            ++seq;
            return (p->rank);
        }
        else {
            if (hashtable[minhc].str == NULL) {
                hashtable[minhc].str = &strpool[strtail * (wheellen + 1)];
                ++strtail;
                hashtable[minhc].rank = seq;
                hashtable[minhc].next = NULL;
                int p = startrank;
                for (int i = 0; i < wheellen; ++i) {
                    hashtable[minhc].str[i] = str[p];
                    p = (p + 1) % wheellen;
                }
                hashtable[minhc].str[wheellen] = 0;
            }
            else {
                hot->next = &nodepool[nodetail++];
                hot->next->str = &strpool[strtail * (wheellen + 1)];
                ++strtail;
                hot->next->rank = seq;
                hot->next->next = NULL;
                int p = startrank;
                for (int i = 0; i < wheellen; ++i) {
                    hot->next->str[i] = str[p];
                    p = (p + 1) % wheellen;
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
    scanf("%d%d", &m, &n);
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