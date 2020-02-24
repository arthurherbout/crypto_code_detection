#include <iostream>
#include <set>
#include <map>
#include <queue>
#include <stack>
#include <algorithm>
#include <vector>
#include <string>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <cmath>
#include <sstream>
using namespace std;

#define sz(A) (int)(A).size()
#define vs vector<string>
#define vi vector<int>
#define ll long long
#define FOR(A,B) for(int A=0; A < (int) (B);A++)
#define pb push_back
#define pii pair<int,int>
#define inf (1<<25)
#define infL 10000000000000000LL
#define F first
#define S second
#define all(x) x.begin() , x.end()

map<int, int> pos;
int c;

int maior() {
	int mm = 0;
	int pp = 0;
	for(map<int,int>::iterator it = pos.begin(); it != pos.end(); it++) {
		if((*it).second > mm) {
			mm = (*it).second;
			pp = (*it).first;
		}
	}
	return pp;
}

int calc() {
	int res = 0;
	while(1) {
		int pmax = maior();
		int qtd = pos[pmax];
		if(qtd == 1) break;
		res++;

		if(qtd > 2)
			pos[pmax] = qtd - 2;
		else
			pos.erase( pos.find(pmax) );

		pos[pmax-1]++;
		pos[pmax+1]++;
	}
	return res;
}

int main() {
	int T;
	scanf("%d", &T);
	FOR(t,T) {
		printf("Case #%d: " , t+1);
		scanf("%d", &c);
		pos.clear();
		FOR(i,c) {
			int aux, qtd;
			scanf("%d %d", &aux, &qtd);
			pos[aux] = qtd;
		}
		printf("%d\n", calc());
	}

    return 0;
}

