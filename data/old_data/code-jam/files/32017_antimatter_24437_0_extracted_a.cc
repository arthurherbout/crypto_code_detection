#include <cstdio>
#include <cstring>
using namespace std;

long long n,A,B,C,D,x0,y0,M;
long long x[100000];
long long y[100000];

long long bucket[9];
int main() {
    int N; scanf("%d", &N);
    for (int it = 1; it <= N; it++) {
        scanf("%lld %lld %lld %lld %lld %lld %lld %lld", &n, &A, &B, &C, &D, &x0, &y0, &M);

        long long X = x0, Y = y0;
        x[0] = X, y[0] = Y;

        for (int i = 1; i < n; i++) {
            X = (A * X + B) % M;
            Y = (C * Y + D) % M;
            x[i] = X;
            y[i] = Y;
        }

        memset(bucket,0,sizeof(bucket));

        for (int i = 0; i < n; i++) {
            int xb = x[i] % 3;
            int yb = y[i] % 3;
            bucket[xb*3 + yb]++;
        }

        long long ret = 0;
        for (int i = 0; i < 9; i++) {
            for (int j = i; j < 9; j++) {
                for (int k = j; k < 9; k++) {
                    int x = (i/3) + (j/3) + (k/3);
                    int y = (i%3) + (j%3) + (k%3);
                    if ((x%3) || (y%3)) continue;

                    if (i == j && j == k) {
                        ret += (bucket[i] * (bucket[i] - 1) * (bucket[i] - 2)) / 6;
                    }
                    else if (i == j) {
                        ret += (bucket[i] * (bucket[i] - 1) * bucket[k]) / 2;
                    }
                    else if (j == k) {
                        ret += (bucket[i] * bucket[j] * (bucket[j] - 1)) / 2;
                    }
                    else {
                        ret += bucket[i] * bucket[j] * bucket[k];
                    }
                }
            }
        }
        printf("Case #%d: %lld\n", it, ret);
    }
}
