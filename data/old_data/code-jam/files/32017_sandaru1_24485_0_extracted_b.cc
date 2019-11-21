#include <iostream>
#include <fstream>
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

int a,b,p;
vector<int> primes;

void calc_primes() {
	int x;
	ifstream fin("primes.txt");
	for(int i=0;i<1000;i++) {
		fin >> x;
		primes.push_back(x);
	}
}

bool share(int x,int y,int p) {
	for(int i=0;i<1000;i++) {
		if (primes[i]<p) continue;
		if ((x%primes[i])==0 && (y%primes[i])==0)
			return true;
		if (x<primes[i] || y<primes[i]) 
			break;
	}
	return false;
}

int calc(int a,int b,int p) {
	int set[10000];
	int ans = 0;
	bool used[10000];
	for(int i=a;i<=b;i++)  {
		set[i] = i;
		used[i] = false;
	}
	for(int i=a;i<=b;i++) {
		for(int k=a;k<=b;k++) {
			if (share(i,k,p)) {
				if (set[k]<set[i]) {
					set[i] = set[k];
				} else {
					set[k] = set[i];
				}
			}
		}
	}
	for(int i=a;i<=b;i++) {
		if (!used[set[i]]) {
			used[set[i]] = true;
			ans++;
		}
	}
	return ans;
}

int main() {
	calc_primes();
	int c;
	cin >> c;
	for(int i=0;i<c;i++) {
		cin >> a >> b >> p;
		cout << "Case #" << i+1 << ": " << calc(a,b,p) << endl;
	}
	return 0;
}
