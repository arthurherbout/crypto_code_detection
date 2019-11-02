#include <bits/stdc++.h>

using namespace std;

using ll = long long;
using ld = long double;
using D = double;
using uint = unsigned int;

#ifdef WIN32
    #define LLD "%I64d"
#else
    #define LLD "%lld"
#endif

#define pb push_back
#define mp make_pair
#define all(x) (x).begin(),(x).end()
#define fi first
#define se second

const int maxn = 105;
const int maxl = 55;

char s[maxn][maxl];
char bs[maxl];
int n, L;

int main()
{
    int NT = 0;
    scanf("%d", &NT);
    for (int T = 1; T <= NT; T++)
    {
        printf("Case #%d:", T);
		
		scanf("%d%d", &n, &L);
		for (int i = 0; i < n; i++) scanf("%s", s[i]);
		scanf("%s", bs);
		bool ok = true;
		for (int i = 0; i < n; i++) if (strstr(s[i], bs) != NULL) ok = false;
		if (!ok)
		{
			printf(" IMPOSSIBLE\n");
			continue;
		}
		printf(" ");
		for (int i = 0; i < 49; i++) printf("01");
		printf("0?");
		printf("1");
		printf(" ");
		for (int i = 0; i < L - 1; i++) printf("?");
		if (L == 1) printf("0");
		printf("\n");
		
        fprintf(stderr, "%d/%d cases done!\n", T, NT);
    }
    return 0;
}
