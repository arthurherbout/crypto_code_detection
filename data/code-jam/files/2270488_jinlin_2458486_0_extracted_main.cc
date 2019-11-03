#include <iostream>
#include <cstring>
#include <vector>
#include <cstdio>
#include <map>

using namespace std;

int T, K, N;
int keys[201];
int type[201];
int remain[201];
vector<int> contents[201];
vector<int> ans;
map<int,int> key2key[201];

void init() {
	for(int i=1; i<=200; ++i)
		key2key[i].clear();
	for(int i=0; i<N; ++i)
		for(int j=0; j<contents[i].size(); ++j)
			key2key[type[i]][contents[i][j]]++;
}

void open(int k) {
	keys[type[k]]--;
	for(int i=0; i<contents[k].size(); ++i) {
		keys[contents[k][i]]++;
		key2key[type[k]][contents[k][i]]--;
	}
	remain[type[k]]--;
	remain[0]--;
	type[k] = -type[k];
}

void unopen(int k) {
	type[k] = -type[k];
	remain[type[k]]++;
	remain[0]++;
	for(int i=0; i<contents[k].size(); ++i) {
		keys[contents[k][i]]--;
		key2key[type[k]][contents[k][i]]++;
	}
	keys[type[k]]++;
}

int flags[201];
int flag;

bool dfs(int tI, int tII) {
	//fprintf(stderr, "(%d,%d)\n", tI, tII);
	if(tI==tII) return true;
	if(flags[tI]==flag) return false;
	flags[tI] = flag;
	for(map<int,int>::iterator itr=key2key[tI].begin(); itr!=key2key[tI].end(); ++itr) {
		if(itr->second==0) continue;
		if(dfs(itr->first, tII)) return true;
	}
	return false;
}

bool check() {
	//fprintf(stderr, "%d\n", remain[0]);
	if(remain[0]==0) return true;
	for(int i=0; i<N; ++i) {
		if(type[i]<0) continue;
		if(keys[type[i]]==0) continue;
		if(remain[type[i]]<=keys[type[i]]) {
			open(i);
			return check();
		}
		//fprintf(stderr, "-%d\n", i);
		++flag;
		for(int j=0; j<contents[i].size(); ++j)
			if(dfs(contents[i][j], type[i])) {
				open(i);
				return check();
			}
	}
	return false;
}

void solve() {
	static int copy[201];
	for(int i=0; i<N; ++i) {
		if(type[i]<0) continue;
		if(keys[type[i]]==0) continue;
		//fprintf(stderr, "%d\n", i);
		open(i);
		memcpy(copy, type, sizeof(type));
		if(check()) {
			for(int j=0; j<N; ++j)
				if(copy[j]!=type[j]) unopen(j);
			ans.push_back(i+1);
			solve();
			return;
		}
		for(int j=0; j<N; ++j)
			if(copy[j]!=type[j]) unopen(j);
		unopen(i);
	}
}

int main() {
	scanf("%d", &T);
	for(int cas=1; cas<=T; ++cas) {
		scanf("%d%d", &K, &N);
		memset(keys, 0, sizeof(keys));
		memset(remain, 0, sizeof(remain));
		memset(flags, 0, sizeof(flags));
		flag=0;
		for(int tmp, i=0; i<K; ++i) {
			scanf("%d", &tmp);
			keys[tmp]++;
		}
		for(int Ki, i=0; i<N; ++i) {
			contents[i].clear();
			scanf("%d%d", type+i, &Ki);
			remain[type[i]]++;
			remain[0]++;
			for(int tmp, j=0; j<Ki; ++j) {
				scanf("%d", &tmp);
				contents[i].push_back(tmp);
			}
		}
		ans.clear();
		init();
		solve();
		printf("Case #%d:", cas);
		if(ans.empty()) printf(" IMPOSSIBLE");
		else for(int i=0; i<ans.size(); ++i)
			printf(" %d", ans[i]);
		printf("\n");
	}
}
