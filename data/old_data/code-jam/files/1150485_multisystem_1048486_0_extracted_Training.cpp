#include <vector>
#include <list>
#include <map>
#include <set>
#include <queue>
#include <deque>
#include <stack>
#include <bitset>
#include <algorithm>
#include <functional>
#include <numeric>
#include <utility>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <cstdio>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <string.h>
#include <list>
#include <climits>
#include <assert.h>
#define rep(i,n) for(int i=0; i<n; i++)
#define reps(i,m,n) for(int i=m; i<n; i++)
#define repe(i,m,n) for(int i=m; i<=n; i++)
#define repi(it,stl) for(typeof((stl).begin()) it = (stl).begin(); (it)!=stl.end(); ++(it))
#define sz(a) ((int)(a).size())
#define mem(a,n) memset((a), (n), sizeof(a))
#define all(n) (n).begin(),(n).end()
#define rall(n) (n).rbegin(),(n).rend()
#define allarr(n) (n), (n)+( (sizeof (n)) / (sizeof (*n)) )
#define mp(a,b) make_pair((a),(b))
#define pnt(a,b) mp((a),(b))
#define pii pair<int,int>
#define pib pair<int,bool>,&n
#define vi vector<int>
#define vc vector<char>
#define vs vector<string>
#define sstr stringstream
typedef long long ll;
using namespace std;

char arr[103][103];
int n;

pair<double,int> WP[103], OWP[103], OOWP[103];


pair<double,int> calcWP(int idx) {
	int res = 0, len=0;
	rep(i,n) {
		if (arr[idx][i] == '.')
			continue;
		if (arr[idx][i] == '1')
			++res;
		++len;
	}
	return mp(res,len);
}

pair<double,int> calcOWP(int idx) {
	double res = 0;
	int len=0;
	rep(i,n) {
		if (i == idx || arr[idx][i]=='.')
			continue;
		pair<double,int> wp = WP[i];

		if(arr[i][idx]=='1')
			--wp.first;
		--wp.second;

		res += (double)wp.first/wp.second;
		++len;
	}
	return mp(res,len);
}

pair<double,int> calcOOWP(int idx) {
	double res = 0;
	int len=0;
	rep(i,n) {
		if (i == idx || arr[idx][i]=='.')
			continue;
		res += (double)OWP[i].first / OWP[i].second;
		++len;

	}
	return mp(res,len);
}

double calcRPI(int idx){
	return  0.25 * ((double)WP[idx].first/WP[idx].second) + 0.50 * ((double)OWP[idx].first/OWP[idx].second) + 0.25 * ((double)OOWP[idx].first/OOWP[idx].second);
}
int main() {

	freopen("in.in","r",stdin);
	freopen("out.out","w",stdout);

	int tst;
	scanf("%d", &tst);
	repe(t,1,tst) {
		scanf("%d", &n);
		rep(i,n) {
			scanf("%s", arr[i]);
			WP[i] = calcWP(i);
		}

		rep(i,n)
			OWP[i] = calcOWP(i);

		rep(i,n)
			OOWP[i] = calcOOWP(i);

		printf("Case #%d:\n", t);
		rep(i,n)
			printf("%.12lf\n",calcRPI(i));
	}
	return 0;
}

