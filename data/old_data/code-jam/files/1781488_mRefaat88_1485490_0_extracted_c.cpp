#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>
#include <cctype>
#include <algorithm>
#include <functional>
#include <cmath>
#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include <stack>
#include <queue>
#include <map>
#include <set>
using namespace std;

#define fo(i,n) for(i=0;i<(n);++i)

typedef vector<int> vi ;
typedef vector<string> vs ;
typedef vector<double> vd ;
#define sz(x) ((int)(x).size())
#define all(x) x.begin(),x.end()
#define pb(x) push_back(x)
typedef long long ll;
ll dp[101][101];
vector<pair<ll, int> > A;
vector<pair<ll, int> > B;
ll calc(int curA, int curB)
{
	if(curA >= sz(A) || curB >= sz(B)) return 0;
	ll &r = dp[curA][curB];
	if(r != -1) return r;
	r = 0;
	if(A[curA].second == B[curB].second)
	{
		r = calc(curA+1, curB+1)+ min(A[curA].first, B[curB].first);
		ll sumA = A[curA].first;
		ll sumB = 0;
		int ty = A[curA].second;
		for(int i = curA+1; i < sz(A) ; i++) {
			sumB = B[curB].first;
			for(int j = curB+1 ; j < sz(B) ; j++) {

				if(A[i].second == B[j].second && A[i].second == ty)
					r = max(r, calc(i+1,j+1)+min(sumB+B[j].first, sumA + A[i].first));

				//r = max(r, calc(i,j+1)+ min(sumA, sumB));
				//r = max(r, calc(i+1,j)+ min(sumA, sumB));

				if(B[j].second == B[curB].second && B[j].second == ty)
					sumB +=B[j].first;
			}
			if(A[i].second == A[curA].second && A[i].second == ty)
				sumA +=A[i].first;

		}
	}
	r = max(r, max(calc(curA,curB+1), calc(curA+1,curB)));
	return r;
}
int main()
{
	freopen("C-small-attempt5.in","rt",stdin);
	freopen("c.out","wt",stdout);
	int tt,n,m;
	cin>>tt;
	for(int t = 1; t <= tt ; t++) {
		memset(dp,-1,sizeof(dp));
		cin>>n>>m;
		A.clear();
		A.resize(n);
		for(int i = 0 ; i < n ; i++)
		{
			cin>>A[i].first>>A[i].second;
		}
		B.clear();
		B.resize(m);
		for(int i = 0 ; i < m ; i ++) {
			cin>>B[i].first>>B[i].second;
		}
		ll res = calc(0,0);
		cout<<"Case #"<<t<<": "<< res<<endl;


	}
	return 0;
}
