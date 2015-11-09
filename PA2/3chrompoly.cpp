#include <stdio.h>
#include <string.h>

typedef enum {EMPTY, LINK, LOOP, NOT}TrivialType;
typedef enum {SUBT, CONT}CommandType;

TrivialType test_trivial(int**graph, int pointnum) {
    //case EMPTY
    bool ifeex = false;
    for (int i = 0; i < pointnum; ++i) {
        for (int j = 0; j < pointnum; ++j) {
            if (graph[i][j] > 0) {
                ifeex = true;
                break;
            }
        }
        if (ifeex)
            break;
    }
    if (!ifeex)
        return EMPTY;

    //case LINK
    int init = 0, step = 0;
    while (1) {
        graph[init][init] = 1;
        bool hasc = false;
        for (int j = 0; j < pointnum; ++j) {
            if (graph[init][j] > 0) {
                if (graph[j][j] == 0) {
                    init = j;
                    hasc = true;
                    step++;
                    break;
                }
            }
        }
        if (!hasc)
            break;
    }
    for (int i = 0; i < pointnum; ++i)
        graph[i][i] = 0;
    if (step == pointnum - 1) {
        if (graph[init][0] == 1)
            return LOOP;
        else
            return LINK;
    }
    else
        return NOT;
}

void divideCP(int** graph, int pointnum, CommandType commandtype) {
    if (test_trivial(graph, pointnum) != NOT)
        return;
    int cv1 = 0, cv2 = 0;
    for (int i = 0; i < pointnum; ++i) {
        for (int j = 0; j < pointnum; ++j) {
            if (graph[i][j] > 0) {
                cv1 = i;
                cv2 = j;
                break;
            }
        }
        break;
    }
    if (commandtype == SUBT) {

    }
    else if (commandtype == CONT) {

    }
}

int main() {
    int n = 0, m = 0;
    scanf("%d%d", &n, &m);
    int **graph = new int*[n];
    for (int i = 0; i < n; ++i) {
        graph[i] = new int[n];
        memset(graph[i], 0, sizeof(int) * n);
    }
    for (int i = 0; i < m; ++i) {
        int v1 = 0, v2 = 0;
        scanf("%d%d", &v1, &v2);
        graph[v1][v2] = 1;
        graph[v2][v1] = 1;
    }
    for (int i = 0; i < n; ++i) {
        graph[i][i] = -1;
    }
    test_trivial(graph, n);
    return 0;
}
