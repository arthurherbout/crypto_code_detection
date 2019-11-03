#include <cstdio>
#include <algorithm>
#include <vector>
#include <cstring>

using namespace std;

typedef long long ll;
typedef pair<int,int> ii;

int n;

int main()
{
	int cases;
	scanf("%d", &cases);
	for(int iii=1; iii<=cases; ++iii){
		scanf("%d", &n);
		vector<ii> v;
		while(n--){
			int a, b;
			scanf("%d%d", &a, &b);
			v.push_back(ii(a,b));
		}
		int res = 0;
		while(true){
			int i = 0;
			while(i < v.size() && v[i].second == 1) ++i;
			if(i >= v.size()) break;
			res++;
			vector<ii> u;
			int j = 0;
			bool b = false;
			for(int j=0; v[j].first < v[i].first; ++j){
				if(v[j].first+1 == v[i].first){ b = true; u.push_back(ii(v[j].first,v[j].second+1)); }
				else u.push_back(v[j]);
			}
			if(!b) u.push_back(ii(v[i].first-1,1));
			if(v[i].second > 2) u.push_back(ii(v[i].first,v[i].second-2));
			if(i+1 >= v.size() || v[i+1].first != v[i].first+1) u.push_back(ii(v[i].first+1,1));
			for(int j=i+1; j < v.size(); ++j){
				if(v[j].first-1 == v[i].first) u.push_back(ii(v[j].first,v[j].second+1));
				else u.push_back(v[j]);
			}
			v = u;
		}
		printf("Case #%d: %d\n", iii, res);
	}
	return 0;
}
