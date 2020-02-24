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

int n,h,l;

int arr[10003];
int main() {

	freopen("in.in","r",stdin);
	freopen("out.out","w",stdout);

	int tst;
	scanf("%d",&tst);
	repe(t,1,tst){
		scanf("%d%d%d",&n,&l,&h);
		printf("Case #%d: ",t);

		rep(i,n){
			scanf("%d",&arr[i]);
		}
		repe(i,l,h){
			int res=0;
			rep(j,n){
				if(i % arr[j]== 0 || arr[j]%i==0)
					++res;
			}
			if(res == n){
				printf("%d\n",i);
				goto end;
			}
		}

		printf("NO\n");
		end:;
	}

	return 0;
}

