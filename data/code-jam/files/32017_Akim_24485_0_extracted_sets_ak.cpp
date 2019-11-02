#include <cstdio>
#define OMAX 2000000
using namespace std;
#include <iostream>

long long a,b,p;
bool prime[OMAX];

bool used[OMAX];

int col = 0;
int P[OMAX];

pair<int, int> BFS[OMAX];
int L, R;

int color[OMAX];

int len;

void try_add(int d, int F){
	if (d<0) return;
	if (d>len) return;
	if (used[d]) return;
	BFS[R++] = make_pair(d,F);
	used[d] = true;
}

int get(){
	cerr<<a<<" "<<b<<" "<<p<<endl;
	len = b-a;
	for (int i = 0; i < OMAX; i++) prime[i] = true;
	prime[0]=prime[1]=false;
	for (int i = 0; i < OMAX; i++) if (prime[i])
		for (int j = i*2; j < OMAX; j += i) prime[j] = false;
/*	col = 0;
	for (int i = 0; i < OMAX; i++) if ((prime[i])&&(i>=0)) P[col++] = i;
	for (int i = 0; i < OMAX; i++) used[i] = false;
	int zog = 0;
	for (int i = 0; i <= b-a; i++) if (!used[i]){
//		cerr<<"found zog"<<i+a<<endl;
		zog++;
		L = 0;
		R = 1;
		BFS[0] = make_pair(0,i);
		while (L<R){
			int fr = BFS[L].second;
			int gg = BFS[L++].first;
			long long no = gg+a;
			if (L%1000==0)
			cerr<<L<<endl;
			int h;
			for (h = 0; h < col; h++) if (no%P[h]==0){
				if (P[h]>=p){
					try_add(gg+P[h],h);
					try_add(gg-P[h],h);
				}
				while (no%P[h]==0) no /= P[h];
			} else if (P[h]*P[h]>no) break;
			if (no>=p){
				try_add(gg+no,h);
				try_add(gg-no,h);
			}
		}
	}
	return zog;    */
	for (int i = a; i <= b; i++) color[i] = i;
	for (int i = a; i <= b; i++){
		if (i%100==0)
		cerr<<i<<endl;
		for (int j = a; j <= b; j++)
			for (int k = p; k < 1000; k++)
				if ((prime[k])&&(i%k==0)&&(j%k==0)){
					int co = color[i];
					for (int t = a; t <= b; t++) if (color[t]==co) color[t] = color[j];
					break;
				}
	}
				int zog = 0;
	for (int i = a; i <= b; i++) if (color[i]==i) zog++;
	return zog;
}

int main(){
	freopen("sets.in", "rt", stdin);
	freopen("sets.out", "wt", stdout);
	int T;
	scanf("%d", &T);
	for (int t = 1; t <= T; t++){
		cin>>a>>b>>p;
		cout<<"Case #"<<t<<": "<<get()<<endl;
	}
	fclose(stdin);
	fclose(stdout);
	return 0;
}
