#include <algorithm>
#include <iostream>
#include <cstring>
#include <cstdio>
#include <cmath>
#include <map>

using namespace std;

int main() {
	int T;
	scanf("%d", &T);
	for(int cas=1; cas<=T; ++cas) {
		int n;
		scanf("%d", &n);
		map<int, int> mp;
		for(int i=0; i<n; ++i) {
			int x;
			scanf("%d", &x);
			++mp[x];
		}
		int ans = n;
		while(!mp.empty()) {
			map<int,int>::iterator itr = mp.begin();
			if(itr->second==0) {
				mp.erase(itr);
				continue;
			}
			int tmp = 1;
			for(int i=itr->first; mp[i+1]>=mp[i]; ++i) {
				++tmp;
			}
			for(int i=itr->first; i<itr->first+tmp; ++i)
				--mp[i];
			ans = min(ans, tmp);
		}
		printf("Case #%d: %d\n", cas, ans);
	}
}
