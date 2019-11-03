#include <iostream>
#include <sstream>
#include <string>
#include <algorithm>
#include <list>
#include <vector>
#include <set>
#include <map>
#include <queue>

//c lib
#include <cmath>
#include <cstring>
#include <cstdlib>

//alias
#define ll long long


using namespace std;

inline int happysum(int n,int b){
	int a = 0,c;
	while(n){
		c = n%b;
		a += c*c;
		n /= b;
	}
	return a;
}

#define MAXDP 20000000

int happy[11][MAXDP];

int main(void){
	for(int b=3;b<=10;b++){
		happy[b][1] = 1;
		for(int i=2;i<MAXDP;i++){
			int cur = i;
			bool f;
			if(happy[b][cur]){
				continue;
			}
			vector<int> immi;
			while(1){
				if(cur<i){
					cur = happy[b][cur];
					f = 1;
					break;
				}
				cur = happysum(cur,b);
				immi.push_back(cur);
				if(happy[b][cur]){
					cur = happy[b][cur];
					f = 0;
					break;
				}
				happy[b][cur] = -1;
			}
			if(cur == 1){
				happy[b][i] = 1;
				if(f == 0){
					for(int j=0;j<immi.size();j++){
						happy[b][immi[j]] = 1;
					}
				}
			}
		}
	}

	int ncas,ti;
	char buf[1024];
	cin >> ncas;
	gets(buf);
	for(ti=1;ti<=ncas;ti++){
		/*Write Code Here*/
		int ch;
		vector<int> bases;
		while(1){
			ch = getchar();
			if('0' <= ch && '9' >= ch){
				ungetc(ch,stdin);
				int b;
				scanf("%d",&b);
				if(b==2) continue;
				bases.push_back(b);
			}
			if(ch == '\n' || ch == EOF) break;
		}
		int a = 2;
		int vlen = bases.size();
		for(int b=vlen-1;b>=vlen-1 && b>=0;b--){
			int base = bases[b];
			for(;;a++){
				int cur = a;
				if(happy[base][cur] == 1){
					bool ok = 1;
					for(int bb=vlen-2;bb>=0;bb--){
						if(happy[bases[bb]][cur] != 1){
							ok = 0;
							break;
						}
					}
					if(ok) break;
				}
			}
		}
		printf("Case #%d: %d\n",ti,a);
	}
	return 0;
}

