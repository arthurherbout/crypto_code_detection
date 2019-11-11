#define _CRT_SECURE_NO_WARNINGS
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <string>
#include <vector>
#include <iostream>
#include <map>
#include <set>
#include <algorithm>
#include <queue>
#include <sstream>
using namespace std;
typedef long long ll;
typedef pair<int,int> pii;
#define SZ(x) (int)(x.size())
#define F0(i,n) for(i=0;i<n;i++)
#define F1(i,n) for(i=1;i<=n;i++)
#define CL(a,x) memset(x, a, sizeof(x))
const int inf = 1000000009;
const double pi = atan(1.0)*4.0;
const double eps = 1e-8;
ll gcd(ll x, ll y) { return y ? gcd(y, x%y) : x; }
int bc(int n) { return n ? bc((n-1)&n)+1 : 0; }
int dx[]={-1,0,1,0};
int dy[]={1,0,-1,0};
int i, j, k, k2, m, n, l, ans;
char ss[1000002];

int a[100005][4], last[100005][5], last2[100005][5][5], last3[100005][5][5][5], dp[100005][5][5], dp2[100005][5][5][5];

int main() {
//	freopen("d.in", "r", stdin);

	//freopen("D-small-attempt0.in", "r", stdin);
	//freopen("D-small-attempt1.out", "w", stdout);

	freopen("D-large.in", "r", stdin);
	freopen("D-large.out", "w", stdout);

	int tt, tn; cin >> tn;
	F1(tt,tn) {
        cerr << tt << endl;
		printf("Case #%d: ", tt);

		int typ;
		cin >> n >> m >> typ;
		F0(i,n)
		{
			F1(j,m) cin >> a[i][j];
		}
		F0(i,n) F1(j,m)
		{
			last[i][j] = i;
			if (i > 0)
			{
				F1(k,m) if (a[i-1][k] == a[i][j])
					last[i][j] = last[i-1][k];
			}
		}

		F0(i,n) F1(j,m) F1(k,m)
		{
			last2[i][j][k] = last[i][j];
			if (last[i][j] > 0)
			{
				int l = last[i][j] - 1;
				last2[i][j][k] = min(last2[i][j][k], last[l][k]);
				if (last[l][k] > 0)
				{
					int l2 = last[l][k] - 1;
					for (int k2 = 1; k2 <= m; k2++)
						if (a[l2][k2] == a[i][j])
						{
							last2[i][j][k] = min(last2[i][j][k], last2[l][k][k2]);
						}
				}
			}
		}

		F0(i,n) F1(j,m) F1(k,m) F1(k2,m)
		{
			last3[i][j][k][k2] = last2[i][j][k];
			if (last2[i][j][k] > 0)
			{
				int l = last[i][j] - 1;
				int l2 = last[l][j] - 1;

				// found 1 before
				bool found = 0;
				for (int z = 1; z <= m; z++)
					if (a[i][j] == a[l2][z])
					{
						found = 1;
						last3[i][j][k][k2] = min(last3[i][j][k][k2], last3[l][k][z][k2]);
					}
				if (!found)
				{
					int l3 = last2[i][j][k] - 1;

					last3[i][j][k][k2] = min(last3[i][j][k][k2], last2[l][k][k2]);
					if (last2[l][k][k2] > 0)
					{
						int l3 = last2[l][k][k2] - 1;
						for (int z = 1; z <= m; z++)
							if (a[l3][z] == a[i][j])
								last3[i][j][k][k2] = min(last3[i][j][k][k2], last3[l][k][k2][z]);
					}
				}
			}
		}

		int best = 0, bl = -1, br = -1;

		if (typ == 3)
		{
			F0(i,n) F1(j,m) F1(k,m) F1(k2,m)
				if (i-last3[i][j][k][k2]+1 > best)
				{
					best = i-last3[i][j][k][k2]+1;
					bl = i-best + 1;
					br = i;
				}
		}
		else
		{
			F0(i,n) F1(j,m) F1(k,m)
				if (i-last2[i][j][k]+1 > best)
				{
					best = i-last2[i][j][k]+1;
					bl = i-best + 1;
					br = i;
				}
		}
		
		cout << bl << " " << br << endl;
	}
    cerr << "Press any key to continue" << endl;
    while (1);

	return 0;
}
