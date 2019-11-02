#include<iostream>
#include<cstring>
#include<algorithm>
#include<sstream>
#include<string>
#include<vector>
#include<cmath>
#include<cstdio>
#include<cstdlib>
#include<fstream>
#include<cassert>
#include<numeric>
#include<set>
#include<map>
#include<queue>
#include<list>
#include<deque>
using namespace std;

#define FOR(i,a,b) for(int i = (a); i < (b); ++i)
#define REP(i,n) FOR(i,0,n)
#define FORE(it,x) for(typeof(x.begin()) it=x.begin();it!=x.end();++it)
#define pb push_back
#define all(x) (x).begin(),(x).end()
#define CLEAR(x,with) memset(x,with,sizeof(x))
#define sz size()
typedef vector<int> VI;
typedef long long ll;

const int MIL = 1000*1000;
bool is_composite[MIL];
vector<int> primes;

void get_primes() {
    primes.pb(2);
    for(int i = 3; i < MIL; i += 2) {
        if(!is_composite[i])
            primes.pb(i);
        for(int j = i+i+i; j <= MIL; j += i+i) {
            is_composite[j] = true;
        }
    }
}

int power(int a, int N, int MOD) {
    if(N == 0) return 1;
    if(N == 1) return a % MOD;
    long long half = power(a, N/2, MOD);
    long long ret = (half * half) % MOD;
    if(N % 2) ret = (ret * a) % MOD;
    return ret;
}

int modular_inverse(int a, int prime) {
    return power(a, prime-2, prime);
}

int div(long long a, int b, int MOD) {
    return (a * modular_inverse(b, MOD)) % MOD;
}

void eliminate(vector<VI>& mat, int MOD) {
    int h = mat.size(), w = mat[0].size();

    for(int j = 0; j < w; ++j) {
        if(j >= mat.sz) break;
        for(int i = j; i < h; ++i) {
            if(mat[i][j] != 0) {
                swap(mat[j], mat[i]);
                break;
            }
        }
        if(mat[j][j] == 0) break;
        int inverse = modular_inverse(mat[j][j], MOD);
        for(int i = j+1; i < h; ++i) {
            long long mult = (mat[i][j] * (long long)inverse) % MOD;
            for(int k = j; k < w; ++k) {
                mat[i][k] = (mat[i][k] - (mat[j][k] * mult) % MOD + MOD) % MOD;
            }
        }
    }
    while(mat.sz > 0 && accumulate(all(mat.back()), 0) == 0) {
        mat.pop_back();
    }
    if(mat.size() != 2) return;
    long long mult = div(mat[0][1], mat[1][1], MOD);
    mat[0][1] = (mat[0][1] - (mult * mat[1][1]) % MOD + MOD) % MOD;
    mat[0][2] = (mat[0][2] - (mult * mat[1][2]) % MOD + MOD) % MOD;
}

int d, k, a[11];

void solve() {
    if(k == 1) {
        printf("I don't know.\n");
        return;
    }
    REP(i,k-1) {
        if(a[k-1] == a[i]) {
            printf("%d\n", a[i+1]);
            return;
        }
    }
    int upper_bound = 1;
    REP(i,d) upper_bound *= 10;
    int solution = -1;
    int max_seen = *max_element(a, a+k);
    for(int i = 0; i < primes.size(); ++i) {
        int MOD = primes[i];
        if(MOD > upper_bound) break;
        if(max_seen >= MOD) continue;
        vector<VI> matrix;
        matrix.reserve(k-1);
        REP(i,k-1) {
            vector<int> row(3);
            row[0] = a[i];
            row[1] = 1;
            row[2] = a[i+1];
            matrix.pb(row);
        }
        eliminate(matrix, MOD);
        if(matrix.sz <= 1) {
            printf("I don't know.\n");
            return;
        }
        else if(matrix.sz == 2) {
            long long A = div(matrix[0][2], matrix[0][0], MOD);
            long long B = div(matrix[1][2], matrix[1][1], MOD);
            int cand = (A * a[k-1] + B) % MOD;
            if(solution == -1)
                solution = cand;
            if(solution != cand) {
                printf("I don't know.\n");
                return;
            }

        }
        else if(matrix.sz > 2) {
            continue;
        }
    }
    printf("%d\n", solution);
}

int main()
{
    get_primes();
    int cases;
    scanf("%d", &cases);
    for(int cc = 0; cc < cases; ++cc)
    {
        scanf("%d %d", &d, &k);
        REP(i,k) scanf("%d", &a[i]);
        printf("Case #%d: ", cc+1);
        solve();
    }
}

