#include <cstdio>
#include <set>
#include <cstring>
#include <vector>
using namespace std;

long long A, B, P;

bool primes[1000001];
vector<int> pp;

int par[2000001];

vector<int> factors[1000001];

int root(int n) {
    if (par[n] == -1) return n;
    return par[n] = root(par[n]);
}
void merge(int a, int b) {
    int A = root(a), B = root(b);
    if (A == B) return;
    if (rand() % 2) {
        par[A] = B;
    } else {
        par[B] = A;
    }
}

int main() {
    memset(primes, 1, sizeof(primes));
    primes[0] = primes[1] = 0;
    for (int i = 4; i <= 1000000; i += 2) primes[i] = 0;
    pp.push_back(2);
    for (int i = 3; i <= 1000000; i += 2) {
        if (!primes[i]) continue;
        pp.push_back(i);
        for (int j = 3; j * i <= 1000000; j += 2) {
            primes[i*j] = 0;
        }
    }

    int C; scanf("%d", &C);
    for (int it = 1; it <= C; it++) {
        scanf("%lld %lld %lld", &A,& B, &P);

        memset(par, -1, sizeof(par));

        for (int i = 0; i < pp.size(); i++) {
            if (pp[i] < P) continue;
            int x = pp[i];

            // smallest multiple in the range
            long long lo = (A / x) * x;
            long long hi = ((B+x-1) / x) * x;

            long long found = -1;
            while (lo <= hi) {
                if (lo >= A && lo <= B) {
                    found = lo;
                    break;
                }
                lo += x;
            }

            if (found != -1) {
                for (long long z = found+x; z <= B; z += x) {
                    merge(found - A, z - A);
                }
            }
        }
        
//        for (long long i = A; i <= B; i++) {
//            int idx = i-A;
//            par[idx] = -1;
//
//            factors[idx].clear();
//            long long x = i;
//            for (int j = 0; j < pp.size(); j++) {
//                if (!(x % pp[j])) {
//                    factors[idx].push_back(pp[j]);
//                    while (!(x%pp[j])) x /= pp[j];
//                }
//            }
//            if (x > 1) factors[idx].push_back(x);
//        }
//
//        for (long long i = A; i <= B; i++) {
//            int I = i - A;
//            for (long long j = i+2; j <= B; j++) {
//                int J = j - A;
//                if (root(I) == root(J)) continue;
//
//                int aa = 0, bb = 0;
//                while (aa < factors[I].size() && bb < factors[J].size()) {
//                    if (factors[I][aa] == factors[J][bb]) {
//                        if (factors[I][aa] >= P) {
//                            merge(I, J);
//                            break;
//                        }
//                        else {
//                            aa++;
//                            bb++;
//                        }
//                    }
//                    else if (factors[I][aa] > factors[J][bb]) bb++;
//                    else aa++;
//                }
//            }
//        }

        set<int> roots;
        for (long long i = A; i <= B; i++) {
            int I = i - A;
            roots.insert(root(I));
        }
        printf("Case #%d: %d\n", it, roots.size());
    }
}
