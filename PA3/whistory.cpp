#include <stdio.h>
#include <string.h>
#define HASHD 33
#define HASHSIZE 89591 //size of hashtable
//#define DEBUG

struct Node {
    char* str;
    int rank;
    Node* next;
};

class HashTable {
private:
    Node* hashtable;
    Node* nodepool;
    int nodetail;
    char* strpool;
    int strtail;

    int wheellen;
    int strnum;
    int hashsize;
    int hashd;
    int max_unit_str_len;

    int seq;

    int* DM;
public:
    HashTable(int n, int m, int hs, int hd) {
        wheellen = n;
        strnum = m;
        nodetail = strtail = 0;
        hashsize = hs;
        hashd = hd;
        hashtable = new Node[hashsize];
        nodepool = new Node[hashsize];
        max_unit_str_len = ((wheellen + 1) << 1);
        strpool = new char[strnum * max_unit_str_len];
        memset(hashtable, 0, sizeof(Node) * hashsize);
        memset(nodepool, 0, sizeof(Node) * hashsize);
        memset(strpool, 0, sizeof(char) * (strnum * max_unit_str_len));

        seq = 1;
        DM = new int[wheellen];
        DM[0] = 1 % hashsize;
        for (int i = 1; i < wheellen; ++i) {
            DM[i] = (DM[i - 1] * hashd) % hashsize;
        }
    }

    ~HashTable() {
        delete []hashtable;
        delete []nodepool;
        delete []strpool;
    }

    int char2digit(char c) {
        return (c - 'a' + 1);
    }

    int hashcode(char* str) {
        int hc = 0;
        for (int i = 0; i < wheellen; ++i) {
            hc = (hc * hashd + char2digit(str[i])) % hashsize;
        }
        return hc;
    }

    bool bfcmp(char* a, char* b) {
        for (int i = 0; i < wheellen; ++i) {
            if (a[i] != b[i])
                return false;
        }
        return true;
    }

    Node* find_hashtable(Node* sta, char* str, Node* &hot) {
        if (sta->str == NULL) {
            hot = sta;
            return NULL;
        }
        while (sta != NULL) {
            if (bfcmp(str, sta->str)) {
                return sta;
            }
            else {
                hot = sta;
                sta = sta->next;
            }
        }
        return sta;
    }

    int readstring(char* str) {
        int hc = hashcode(str);
        #ifdef DEBUG
        printf("%s : %d\n", str, hc);
        #endif
        Node* hot = NULL;
        Node* p = find_hashtable(&hashtable[hc], str, hot);
        if (p != NULL) {
            ++seq;
            return (p->rank);
        }
        else {
            char* ss = &strpool[strtail * max_unit_str_len];
            strcpy(&strpool[strtail * max_unit_str_len], str);
            strcpy(&strpool[strtail * max_unit_str_len + wheellen], str);
            ++strtail;
            for (int i = 0; i < wheellen; ++i) {
                if (hashtable[hc].str == NULL) {
                    hashtable[hc].str = ss;
                    hashtable[hc].rank = seq;
                }
                else {
                    hot->next = &nodepool[nodetail++];
                    hot->next->str = ss;
                    hot->next->rank = seq;
                    hot->next->next = NULL;
                }
                hc = (hc - char2digit(*ss) * DM[wheellen - 1 - i]) % hashsize;
                hc = (hc * hashd + char2digit(*(ss + wheellen))) % hashsize;

                ++ss;
                //hc = hashcode(ss);
                #ifdef DEBUG
                printf("%s : %d vs %d\n", ss, hc, hashcode(ss));
                #endif
                find_hashtable(&hashtable[hc], ss, hot);
            }
            ++seq;
            return 0;
        }
    }
};

int main() {
    int n = 0, m = 0;
    scanf("%d%d", &m, &n);
    getchar();
    HashTable ht(n, m, HASHSIZE, HASHD);
    char tempstr[2000];
    for (int i = 0; i < m; ++i) {
        gets(tempstr);
        printf("%d\n", ht.readstring(tempstr));
    }
    return 0;
}