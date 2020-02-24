#include <cstdio>
#include <cstring>
using namespace std;

int K;
int n;
int d[100];

int deck[1000001];

int inc(int idx) {return (idx+1)%K;}

int next(int idx) {
    int ret = inc(idx);
    while (1) {
        if (deck[ret]) ret = inc(ret);
        else return ret;
    }
}

int main() {
    int T; scanf("%d", &T);
    for (int it = 1; it <= T; it++) {
        scanf("%d", &K);
        scanf("%d", &n); for (int i = 0; i < n; i++) scanf("%d", d+i);

        memset(deck, 0, sizeof(deck));
        int idx  = K-1;
        int at   = 0;
        int left = K;

        for (int i = 1; i <= K; i++) {
            while (at + left < i) at += left;
            while (1) {
                at++;
                idx = next(idx);
                if (at == i) {
                    left--;
                    at = 0;
                    deck[idx] = i;
                    break;
                }
            }
        }

        printf("Case #%d:", it);
        for (int i = 0; i < n; i++) printf(" %d", deck[d[i] - 1]);
        printf("\n");
    }
}
