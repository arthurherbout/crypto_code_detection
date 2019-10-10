#include<iostream>
#include<algorithm>
#include<vector>
#include<set>
#include<map>
#include<deque>
#include<string>
#include<cctype>
#include<cmath>
#include<sstream>
#include<numeric>
#include<complex>
#include<queue>
using namespace std;

int setBit(int num, int index, int value){return value ? (num|(1<<index)) : (num&(~(1<<index)));}
int getBit(int num, int index){return (num >> index) & 1 ? 1 : 0;}

int N, K, P, MOD = 30031, base;
int table[1000][1<<10];
int get(int ind, int last){

	if(ind >= N)return last == base;
	if(table[ind][last] != -1)return table[ind][last];

	int r = 0;
	for(int i = 0 ; i < P ; i++){
		if(getBit(last, i) == 0)continue; //no bus stopping there
		if(i != P-1 && getBit(last, P-1) == 1)continue;	//max distance violation
		int nlast = setBit(last, i, 0);
		nlast = nlast << 1;
		nlast = setBit(nlast, 0, 1);
		r = (r+get(ind+1, nlast))%MOD;
	}

	return table[ind][last] = r;
}

int main(){

	//freopen("1.in", "rt", stdin);
	freopen("D-small-attempt0.in", "rt", stdin);freopen("D-small-attempt0.out", "wt", stdout);
	//freopen("D-small-attempt1.in", "rt", stdin);freopen("D-small-attempt1.out", "wt", stdout);
	//freopen("D-large.in", "rt", stdin);freopen("D-large.out", "wt", stdout);

	int tt; cin >> tt;
	for(int t = 0 ; t < tt ; t++){

		cin >> N >> K >> P;
		int i;

		base = 0;
		for(i = 0 ; i < K ; i++)
			base = setBit(base, i, 1);

		memset(table, -1, sizeof table);
		cout << "Case #" << t+1 << ": " << get(K, base) << endl;

	}

	return 0;
}
