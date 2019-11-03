#include <cstdio>
#include <cstring>
#include <iostream>
#include <algorithm>
#include <vector>

using namespace std;

int T, cas;

const int N = 2020;
int a[N], b[N], q[N], value[N], in[N];
vector<int> v[N];

void run() {
    int n;
    scanf("%d", &n);
    for (int i = 0; i < n; i++)
        scanf("%d", &a[i]);
    for (int i = 0; i < n; i++)
        scanf("%d", &b[i]);
    memset(in, 0, sizeof(in));
    memset(value, 0, sizeof(value));
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++)
            if (a[j] == a[i]) {
                v[i].push_back(j);
                in[j] += 1;
                break;
            }
        for (int j = i - 1; j >= 0; j--)
            if (a[j] + 1 == a[i]) {
                v[i].push_back(j);
                in[j] += 1;
                break;
            }
    }
    for (int i = 0; i < n; i++) {
        for (int j = i - 1; j >= 0; j--)
            if (b[j] == b[i]) {
                v[i].push_back(j);
                in[j] += 1;
                break;
            }
        for (int j = i + 1; j < n; j++)
            if (b[j] + 1 == b[i]) {
                v[i].push_back(j);
                in[j] += 1;
                break;
            }
    }
    int front = 0, back = 0;
    for (int i = 0; i < n; i++)
        if (in[i] == 0)
            q[back++] = i;
    int cur = n;
    while (front < back) {
        for (int i = front + 1; i < back; i++)
            if (q[i] > q[front])
                swap(q[i], q[front]);
        int x = q[front++];
        for (int i = 0; i < (int) v[x].size(); i++) {
            int y = v[x][i];
            in[y] -= 1;
            if (in[y] == 0)
                q[back++] = y;
        }
        value[x] = cur--;
    }
    printf("Case #%d:", cas);
    for (int i = 0; i < n; i++)
        printf(" %d", value[i]);
    printf("\n");
    for (int i = 0; i < n; i++)
        v[i].clear();
}

int main() {
    scanf("%d", &T);
    for (cas = 1; cas <= T; cas++)
        run();
    return 0;
}
