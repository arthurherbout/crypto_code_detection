#include<vector>
#include<cmath>
#include<complex>
#include<iostream>
#include<stdio.h>
#include<string>
#include<stdlib.h>
#include<float.h>
#include<set>
#include<map>
#include<queue>
#include<math.h>
#include<algorithm>
using namespace std;


typedef vector<int> vi;
typedef vector<vi> vvi;
typedef vector<double> vd;
typedef vector<vd> vvd;
typedef vector<vvd> vvvd;
typedef pair<int, int> pii;
typedef vector<pii> vpii;

#define pb push_back
#define mp make_pair
#define snd second
#define fst first
#define debug printf("--%d--\n",__LINE__)
#define ll long long int

int T;
int N;

int pa[1000];

int grp(int p){
	if (pa[p]<0) return p;
	return grp(pa[p]);
}

int main(void){
	cin >> T;
	for(int casenum=1;casenum<=T;casenum++){
		cin >> N;
		vector<string> s(N);
		for(int i=0;i<N;i++){
			cin >> s[i];
		}
		
		for(int i=0;i<2*N;i++){
			pa[i] = -1;
		}
		int onecnt = 0;
		for(int i=0;i<N;i++){
			for(int j=0;j<N;j++){
				if (s[i][j]=='1'){
					onecnt++;
					int a = grp(i);
					int b = grp(N+j);
					if (a!=b){
						if (rand()%2) pa[a] = b; else pa[b] = a;
					}
				}
			}
		}
		int hist[2*N][2] = {0};
		for(int i=0;i<2*N;i++) hist[grp(i)][i/N]++;
		//for(int i=0;i<2*N;i++) cout << hist[i][0] << "," << hist[i][1] << endl;
		int sum = 0, sum2 = 0;
		for(int i=0;i<2*N;i++){
			if (min(hist[i][0],hist[i][1])>0){
				int t = max(hist[i][0],hist[i][1]);
				sum += t*t;
				sum2 += t;
			}
		}
		sum += N-sum2;
		sum -= onecnt;
		
		if (sum2 > N){
			sum = N*N - onecnt;
		}
		
		
		cout << "Case #" << casenum << ": ";
		cout << sum << endl;
		
		
		
	}
	
	return 0;
}
