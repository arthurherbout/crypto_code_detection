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

bool used[1000000];

int main(void){
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
				bases.push_back(b);
			}
			if(ch == '\n' || ch == EOF) break;
		}
		int a = 2;
		int vlen = bases.size();
		for(int b=0;b<1;b++){
			int base = bases[b];
			for(;;a++){
				int cur = a;
				int t=0;
				memset(used,0,sizeof(used));
				used[cur] = 1;
				while(1){
					cur = happysum(cur,base);
					if(used[cur]) break;
					used[cur] = 1;
				}
				if(cur == 1){
					bool ok = 1;
					for(int bb=1;bb<vlen;bb++){
						int prev = a;
						int cur = a;
						int bbb = bases[bb];
						memset(used,0,sizeof(used));
						used[cur] = 1;
						while(1){
							cur = happysum(cur,bbb);
							if(used[cur]) break;
							used[cur] = 1;
						}
						if(cur != 1){
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

