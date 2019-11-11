#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <deque>
#include <queue>
#include <set>
#include <map>
#include <algorithm>
#include <functional>
#include <utility>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <cstdio>

using namespace std;

#define REP(i,n) for((i)=0;(i)<(int)(n);(i)++)
#define foreach(c,itr) for(__typeof((c).begin()) itr=(c).begin();itr!=(c).end();itr++)

int N;
int order[10];
int a[10];

bool check2(void){
	int i,j;
	REP(i,N){
		bool b1 = false, b2 = false;
		REP(j,N){
			if(j < i && a[j] > a[i]) b1 = true;
			if(j > i && a[j] < a[i]) b2 = true;
		}
		if(b1 && b2) return false;
	}
	return true;
}

bool check(vector <int> v){
	int step,i,j,k;
	
	if(v[order[N-1]] != 0) return false;
	REP(i,N) a[i] = v[i];
	if(!check2()) return false;
	
	REP(step,N){
		REP(i,N) a[i] = -1;
		REP(i,step) a[order[i]] = v[order[i]];
		a[order[step]] = 0;
		vector <int> other;
		for(i=step;i<N;i++) if(v[order[i]] != 0) other.push_back(v[order[i]]);
		sort(other.begin(),other.end());
		j = 0;
		REP(i,N) if(a[i] == -1){
			a[i] = other[j]; j++;
		}
		if(!check2()) return false;
	}
	
	return true;
}

void main2(void){
	int i;
	
	cin >> N;
	REP(i,N) {cin >> order[i]; order[i]--;}
	
	vector <int> ans;
	vector <int> v;
	REP(i,N) v.push_back(i);
	
	do {
		if(check(v)) ans = v;
	} while(next_permutation(v.begin(),v.end()));
	
	REP(i,N) cout << ' ' << ans[i] + 1;
	cout << endl;
}

//////////////////////////////// multiple testcases ////////////////////////////////

int main(void){
	int T,t;
	scanf("%d",&T);
	REP(t,T){
		printf("Case #%d:",t+1);
		main2();
	}
	return 0;
}
