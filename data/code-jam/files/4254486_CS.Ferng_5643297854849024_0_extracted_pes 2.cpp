#include<stdio.h>
#include<string.h>
#include<math.h>
#include<algorithm>
#include<unordered_set>
#include<string>
using namespace std;

#define BASE 1000000007

int nCase;
int N, D, F[8][64];

bool go(int r) {
    if(r == 0) {
        for(int i = 0; i < N; ++i) if(F[r][i] != F[r][0]) return false;
        return true;
    }
    for(int d = 1; d <= D; d *= 2) {
        for(int g = 0; g < 2*d; ++g) {
            bool ok = true;
            for(int i = 0; i < N && ok; ++i) {
                F[r-1][i] = F[r][i] - ((i+g)%(2*d) < d);
                if(F[r-1][i] < 0) ok = false;
            }
            if(ok && go(r-1)) return true;
        }
    }
    return false;
}

int main() {
    int ff[64];
	scanf("%d", &nCase);
	for(int casei = 1; casei <= nCase; ++casei) {
		scanf("%d %d", &N, &D);
        for(int i = 0; i < N; ++i) scanf("%d", &ff[i]);
        int mx = ff[0], mn = ff[0];
        for(int i = 1; i < N; ++i) {
            if(ff[i] > mx) mx = ff[i];
            if(ff[i] < mn) mn = ff[i];
        }
        int ans = -1;
        for(ans = mx-mn; ans <= 5; ++ans) {
            memcpy(F[ans], ff, sizeof(int)*64);
            if(go(ans)) break;
        }
        if(ans > 5) printf("Case #%d: CHEATERS!\n", casei);
        else printf("Case #%d: %lld\n", casei, ans);
	}
}
