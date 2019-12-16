#include <vector>
#include <list>
#include <map>
#include <set>
#include <deque>
#include <stack>
#include <bitset>
#include <algorithm>
#include <functional>
#include <numeric>
#include <utility>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <cstdio>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <fstream>

using namespace std;

string itoa(int val) {stringstream ss;ss << val;return ss.str();}
typedef vector<int> vi;
vi parseInt(string s) {stringstream ss(s);vi ans;while (!ss.eof()) {int temp; ss >> temp; ans.push_back(temp); } return ans;}
#define COPY(x,y) y.resize(x.size());copy(x.begin(),x.end(),y.begin())
#define pb push_back
#define SWAP(t,x,y) t temp=x;x=y;y=temp;
#define fr(i,s,e) for (int i = int(s); i < int(e); i++)
#define fr2(i,c) for (unsigned int i = 0; i < (c).size(); i++)
#define cl(a,val) memset(a,val,sizeof(a)); 
#define ll long long
#define INF 1000000000

bool broken[50][50];
bool used[50][50];
int r,c;
int powers[11] = {1,2,4,8,16,32,64,128,256,512,1024};

int calc() {
	int ans = 0;
	fr(num,0,powers[c]+1) {
		bool sit[15];
		cl(sit,0);
		fr(k,0,c) {
			if ((powers[k] & num)==powers[k]) {
				sit[k] = true;
			}
		}
		bool ok = true;
		fr(i,1,c) {
			if (sit[i] && sit[i-1]) {
				ok = false;
				break;
			}
		}
		if (!ok) continue;
		int cur = 0;
		cl(used,0);
		fr(i,0,c) {
			if (sit[i]) {
				fr(k,0,r) {
					if (!broken[k][i]) {
						used[k][i] = true;
						cur++;
					}
				}
			}
		}
		fr(i,0,r) {
			fr(k,0,c) {
				if (!used[i][k] && !broken[i][k]) {
					bool ok = true;
					if (used[i+1][k]) ok = false;
					if (used[i+1][k+1]) ok = false;
					if (k>0 && used[i+1][k-1]) ok = false;
					if (i>0 && used[i-1][k]) ok = false;
					if (i>0 && used[i-1][k+1]) ok = false;
					if (k>0 && i>0 && used[i-1][k-1]) ok = false;
					if (ok) {
						used[i][k] = true;
						//cur++;
					}
				}
			}
		}
		ans = max(cur,ans);
	}
	return ans;
}

int main() {
	int t;
	
	ifstream fin("c.in");
	ofstream fout("c.ans");
	
	fin >> t;
	
	fr(i,0,t) {
		cl(broken,0);
		fin >> r >> c;
		fr(k,0,r) {
			string s;
			fin >> s;
			fr(j,0,c) {
				if (s[j]=='x') {
					broken[k][j] = true;
				}
			}
		}
		int x = calc();
		cout << "Case #" << i+1 << ": " << x << endl;
		fout << "Case #" << i+1 << ": " << x << endl;
	}
	
	fin.close();
	fout.close();
	return 0;
}
