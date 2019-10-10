#include <bits/stdc++.h>

using namespace std;

const int MAXN = 10;

bool check[30];
int f[MAXN];
char sk[MAXN], sl[MAXN], chuoi[MAXN];
int k, l, s, nsk, nsl, num_strings;
double prob, res;

void kmp() {
    for (int i = 1, j = 0; i < nsl; i++) {
        while (j && sl[j] != sl[i]) j = f[j - 1];
        if (sl[j] == sl[i]) j++;
        f[i] = j;
    }
}

int max_bananas() {
    int x = nsl - f[nsl-1];
    int res = 0;
    for (int i = 0; ; i += x) {
        if (i + nsl - 1 < s) res++;
        else break;
    }
    return res;
}

int count_instance() {
    int res = 0;
    for (int i = 0; i < s-nsl+1; i++) {
        if (strncmp(chuoi+i, sl, nsl) == 0) res++;
    }
    return res;
}

void dq(int pos) {
    if (pos == s) {
        int instances = count_instance();
        res += prob * instances;
        return;
    }
    for (int i = 0; i < k; i++) {
        chuoi[pos] = sk[i];
        dq(pos + 1);
    }
}

bool unable_target() {
    memset(check, false, sizeof check);
    for (int i = 0; i < k; i++) check[sk[i]-'A'] = true;
    for (int i = 0; i < l; i++)
        if (!check[sl[i]-'A']) return true;
    return false;
}

double solve() {
    if (unable_target()) return 0;
    nsk = strlen(sk);
    nsl = strlen(sl);
    kmp();
    num_strings = 1;
    for (int i = 0; i < s; i++)
        num_strings *= k;
    prob = 1.0 / num_strings;
    res = 0;
    dq(0);
    int bananas = max_bananas();
    return bananas - res;
}

int main() {
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);

    int nTest; scanf("%d\n", &nTest);
    for (int t = 1; t <= nTest; t++) {
        scanf("%d%d%d\n", &k, &l, &s);
        scanf("%s\n", sk);
        scanf("%s\n", sl);
        printf("Case #%d: %.9lf\n", t, solve());
    }

    return 0;
}
