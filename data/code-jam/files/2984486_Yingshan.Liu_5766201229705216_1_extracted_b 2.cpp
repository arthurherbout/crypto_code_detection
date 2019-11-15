#include <iostream>
#include <vector> 

using namespace std;

const int MAXN = 1001;
bool used[MAXN];

int count(vector< vector<int> > &tree, int now) {
	used[now] = true;
	int max=0, submax=0;
	for (int i=0; i<tree[now].size(); i++) {
		if (used[tree[now][i]]) continue;
		int cnt = count(tree, tree[now][i]);
		if (cnt > max) {
			submax = max;
			max = cnt;
		} else if (cnt > submax) {
			submax = cnt;
		}
	}
	used[now] = false;
	
	if (submax == 0) return 1;
	return max + submax + 1;
}

void work() {
	int n, x, y; 
	cin >> n;
	vector< vector<int> > tree(n+1);
	for (int i=0; i<n-1; i++) {
		cin >> x >> y;
		tree[x].push_back(y);
		tree[y].push_back(x);		
	}
	
	for (int i=1; i<=n; i++) used[i]=false;
	int max = 0;
	for (int i=1; i<=n; i++) {
		int cnt = count(tree, i);
		if (cnt > max) max = cnt;
	} 
	cout << n - max << endl;
}

int main() {
	freopen("B-large.in", "r", stdin);
	freopen("B-large.out", "w", stdout); 
	
	int c;
	cin >> c;
	for (int i=0; i<c; i++) {
		cout << "Case #" << i+1 << ": "; 
		work();
	}
}