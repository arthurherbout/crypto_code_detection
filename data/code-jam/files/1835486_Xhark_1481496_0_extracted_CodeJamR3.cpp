#include <stdio.h>
#include <algorithm>
#include <vector>
#include <map>
#include <set>
#include <string>
#include <memory.h>


using namespace std;

char inp[1024];

int mp[256];
int dd[256][256];
map<char, vector<char> > go;
//map<int, char> xx;

int main(){
	freopen("D-small-attempt0.in","r",stdin);
	freopen("D-small-attempt0.out","w",stdout);
	int T;	
	scanf("%d",&T);
	char c;
	for(c='a';c<='z';c++) go[c].push_back(c);
	go['o'].push_back('0');
	go['i'].push_back('1');
	go['e'].push_back('3');
	go['a'].push_back('4');
	go['s'].push_back('5');
	go['t'].push_back('7');
	go['b'].push_back('8');
	go['g'].push_back('9');
		// "o" to "0", "i" to "1", "e" to "3", "a" to "4", "s" to "5", "t" to "7", "b" to "8" and/or "g" to "9".
	while(T-->0) {
		memset(dd, 0, sizeof(dd));
		fprintf(stderr, "%d\n", T);
		int K;
		scanf("%d",&K);
		scanf("%s",inp);
		int su, i, j;
		su = 0;
		for(c='0';c<='9';c++){
//			xx[su] = c;
			mp[c] = su ++;
		}
		for(c='a';c<='z';c++){
//			xx[su] = c;
			mp[c] = su ++;
		}
		for(i=0;inp[i] != '\0';i++){
			if (inp[i+1] != '\0') {
				char ss, ee;
				ss = inp[i];
				ee = inp[i+1];
				int p, q;
				for(p=0;p<go[ss].size();p++){
					for(q=0;q<go[ee].size();q++){
						dd[ mp[ go[ss][p] ] ][ mp[ go[ee][q] ] ] = 1;
					}
				}
				
			}
		}

		int sol=0;


		int ii, oo;

		ii = oo = 0;
		for(i=0;i<su;i++){
			int od, id;
			od = id = 0;
			for(j=0;j<su;j++){
				/*
				if (dd[i][j]) {
					printf("%c %c\n", xx[i],xx[j]);
				}*/
				if (dd[i][j]) od ++;
				if (dd[j][i]) id ++;
			}
			int md = max(od, id);
			if (md > 0){ 
				sol += md * 2;
	
				if (id > od) ii ++;
				if (id < od) oo ++;
			}
		}

		sol /= 2;

		if (ii && oo) sol --;
		

		static int cs = 1;
		printf("Case #%d: %d", cs ++, sol+1);
		printf("\n");

	}
	return 0;
}