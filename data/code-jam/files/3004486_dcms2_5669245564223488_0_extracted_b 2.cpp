#include <bits/stdc++.h>
#define fr(i,a,b) for (int i = (a), __ = (b); i < __; ++i)	
#define st first
#define nd second
#define dbg(x) cout << #x << " " << x << endl
using namespace std;

const double eps = 1e-7;
const int inf = 0x3f3f3f3f;
typedef pair<int,int> ii;
typedef long long ll;
typedef vector<int> vi;

string str[15];
int p[15], mark[26], passo = 0;

int main() {
    int nt; scanf("%d", &nt); ++nt;
    fr(_,1,nt) {
        int n; scanf("%d", &n);
        fr(i,0,n) {
            cin >> str[i];
            p[i] = i;
        }
        int ans = 0;
        do {
            ++passo;
            char prev = '.';
            fr(i,0,n) {
                fr(j,0,str[p[i]].size()) {
                    if (str[p[i]][j] != prev) {
                        prev = str[p[i]][j];
                        if (mark[str[p[i]][j]-'a'] == passo) {
                            goto lol;
                        }
                        mark[str[p[i]][j]-'a'] = passo;
                    }
                }
            }
            ++ans;
            lol:;
        } while (next_permutation(p,p+n));
        printf("Case #%d: %d\n", _, ans);
    }
    return 0;
}
