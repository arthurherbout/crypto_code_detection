#include <iostream>
#include <string>
#include <string.h>
#include <cstring>
#include <vector>
#include <algorithm>
#include <set>
#include <map>
#include <bitset>

using namespace std;

map <int,int> mp;

int n;

int main(){
	freopen("input.txt","r",stdin);
	freopen("output.txt","w",stdout);

	int t;
	scanf("%d",&t);
	for (int tt=1; tt<=t; tt++){
		printf("Case #%d: ",tt);

		mp.clear();
		scanf("%d",&n);
		for (int i=1; i<=n; i++){
			int x,y;
			scanf("%d%d",&y,&x);
			mp[y]=x;
		}

		map<int,int>::iterator it;

		int k=1;
		int res=0;
		while (k){
			k=0;
			for (it=mp.begin(); it!=mp.end(); it++){
				int x,y;
				x=it->first;
				y=it->second;
				if (y==2){
					res++;
					if (mp.count(x-1)==0)
						mp[x-1]=1; else
						mp[x-1]++;
					if (mp.count(x+1)==0)
						mp[x+1]=1; else
						mp[x+1]++;
					mp.erase(x);
					k=1;
					break;
				} else
					if (y>2){
						res++;
						mp[x]-=2;
						if (mp.count(x-1)==0)
							mp[x-1]=1; else
							mp[x-1]++;
						if (mp.count(x+1)==0)
							mp[x+1]=1; else
							mp[x+1]++;
						k=1;
						break;
					}
			}
		}
		printf("%d\n",res);
	}

	return 0;
}