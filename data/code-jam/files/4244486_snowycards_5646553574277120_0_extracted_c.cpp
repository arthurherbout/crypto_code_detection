#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <cstring>

#include <algorithm>
#include <vector>
#include <queue>
#include <stack>
#include <map>

using namespace std;

#define rep(i,n) for(int i = 0, _##i = n; i < _##i; ++i)
#define dwn(i,n) for(int i = n; i >= 0; --i)
#define itr(v, it) for(__typeof(v.begin()) it = v.begin(); it != v.end(); it++)

#define vi vector<int>
#define pb push_back
#define min(a,b) (a<b?a:b)
#define ll long long

#define INF (int) 1e9
#define EPS 1e-9
bool solveTrue(vector<int> &preDenoms, int V) {
	int array[V+1];
	for(int i = 0; i <= V; i++)
		array[i] = 0;

	do {
		int sum = 0;
		for(int i = 0; i < preDenoms.size(); i++) {
			sum += preDenoms[i];
			if(sum <= V)
				array[sum] = 1;
			//cout << sum << endl;
		}
	} while(next_permutation(preDenoms.begin(), preDenoms.end()));

	for(int z = 1; z <= V; z++) {
		if(!array[z]) {
			//cout << z << endl;
			int maxHits = 0;
			int nextInt = 1;
			for(int i = 1; i <= V; i++) {
				if(find(preDenoms.begin(), preDenoms.end(), i) == preDenoms.end()) { //doesnt contain i
					int numHits = 0;
					for(int j = 1; j <= V-i; j++) {
						if(array[j] && !array[j+i])
							numHits++;
					}
					if(maxHits < numHits) {
						maxHits = numHits;
						nextInt = i;
					}
				}
			}
			preDenoms.pb(nextInt);
			return false;
		}
	}
	return true;
}


int numFailed(vector<int> &preDenoms, int V) {
	int additional = 0;
	int array[V+1];
	rep(i, V+1)
		array[i] = 0;

	do {
		int sum = 0;
		for(int i = 0; i < preDenoms.size(); i++) {
			sum += preDenoms[i];
			if(sum <= V)
				array[sum]++;
			//cout << sum << endl;
		}
	} while(next_permutation(preDenoms.begin(), preDenoms.end()));

	int failed = 0;
	for(int i = 1; i <= V; i++)
		if(!array[i])
			failed++;

	for(int i = 1; i <= V; i++) {
		if(!array[i]) {
			preDenoms.pb(i);
			break;
		}
	}
	return failed;
}

int numFailed2(vector<int> &preDenoms, int V) {
	int additional = 0;
	int array[V+1];
	rep(i, V+1)
		array[i] = 0;

	do {
		int sum = 0;
		for(int i = 0; i < preDenoms.size(); i++) {
			sum += preDenoms[i];
			if(sum <= V)
				array[sum]++;
			//cout << sum << endl;
		}
	} while(next_permutation(preDenoms.begin(), preDenoms.end()));

	int failed = 0;
	for(int i = 1; i <= V; i++)
		if(!array[i])
			failed++;

	return failed;
}


void solve() {

	int C, D, V;
	vector<int> preDenoms;
	cin >> C >> D >> V;
	rep(i, D) {
		int temp = 0;
		cin >> temp;
		preDenoms.pb(temp);
	}

	vector<int> preDenoms2 = preDenoms;
	vector<int> preDenoms3 = preDenoms;
	int additional = 0;

	while(numFailed(preDenoms, V)) {
		
		additional++;
	}

	int additional2 = 0;

	while(numFailed2(preDenoms2, V)) {
		for(int i = 1; i <= V; i++) {
			if(find(preDenoms2.begin(), preDenoms2.end(), i) == preDenoms2.end()) {
				preDenoms2.pb(i);
				break;
			}
		}
		additional2++;
	}

	int additions = -1;
	do {
		additions++;
	} while(!solveTrue(preDenoms3, V));

	//printf("%d %d \n", additional, additional2);
	printf("%d\n", min(additions, min(additional, additional2)));
}

int main() {

	int cases;
	cin >> cases;

	rep(current, cases) {

		printf("Case #%d: ", current+1);
		solve();
	}

	return 0;
}