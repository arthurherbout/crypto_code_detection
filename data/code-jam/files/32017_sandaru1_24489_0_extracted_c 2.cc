#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <map>

using namespace std;

string itoa(int val) {stringstream ss;ss << val;return ss.str();}
typedef vector<int> vi;
vi parseInt(string s) {stringstream ss(s);vi ans;while (!ss.eof()) {int temp; ss >> temp; ans.push_back(temp); } return ans;}
#define COPY(x,y) y.resize(x.size());copy(x.begin(),x.end(),y.begin())
#define pb push_back
#define SWAP(t,x,y) t temp=x;x=y;y=temp;
#define ll long long

int k;
int deck[6000];

void calc(int k) {
	memset(deck,0,sizeof(int)*k);

	int next[6000];
	int prev[6000];
	for(int i=0;i<=k;i++) {
		next[i] = (i+1)%k;
		prev[i] = (i-1+k)%k;
	}

	int pos = 0;
	for(int i=0;i<k;i++) {
		int x = 0;
		while(x<i) {
			if (deck[pos]==0) {
				x++;
			}
			pos = next[pos];
		}
		next[prev[pos]] = next[pos];
		prev[next[pos]] = prev[pos];
		deck[pos] = i+1;
	}
}

int main() {
	int c,n;
	cin >> c;
	for(int x=0;x<c;x++) {
		cin >> k;
		calc(k);
		cin >> n;
		cout << "Case #" << x+1 << ":";
		for(int i=0;i<n;i++) {
			int temp;
			cin >> temp;
			cout << " " << deck[temp-1];
		}
		cout << endl;
	}
	return 0;
}
