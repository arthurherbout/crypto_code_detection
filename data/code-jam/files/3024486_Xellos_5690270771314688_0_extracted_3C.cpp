// iostream is too mainstream
#include <cstdio>
// bitch please
#include <iostream>
#include <algorithm>
#include <cstdlib>
#include <vector>
#include <set>
#include <map>
#include <queue>
#include <stack>
#include <list>
#include <cmath>
#include <iomanip>
#define dibs reserve
#define OVER9000 1234567890LL
#define ALL_THE(CAKE,LIE) for(auto LIE =CAKE.begin(); LIE != CAKE.end(); LIE++)
#define tisic 47
#define soclose 1e-10
#define chocolate win
// so much chocolate
#define patkan 9
#define ff first
#define ss second
#define abs(x) ((x < 0)?-(x):x)
#define uint unsigned int
using namespace std;
// mylittledoge

int N;
vector< pair<bool,int> > zaz;
int ans;
set<int> Num;
set<int> In,Out;
vector<int> st;
int k;
int co;

void bt() {
	co++;
	if(co > 1e6) return;
	if(k == N) {
		if(ans == -1) ans =In.size();
		else ans =min(ans,(int)In.size());
		return;}
	if(zaz[k].ss != 0) {
		if(zaz[k].ff) {
			if(In.find(zaz[k].ss) != In.end()) return;
			set<int> In0 =In,Out0 =Out;
			if(Out.find(zaz[k].ss) != Out.end()) Out.erase(zaz[k].ss);
			In.insert(zaz[k].ss);
			k++;
			bt();
			k--;
			In =In0, Out =Out0;
			return;}
		if(Out.find(zaz[k].ss) != Out.end()) return;
		set<int> In0 =In,Out0 =Out;
		if(In.find(zaz[k].ss) != In.end()) In.erase(zaz[k].ss);
		Out.insert(zaz[k].ss);
			k++;
			bt();
			k--;
		In =In0, Out =Out0;
		return;}
	if(zaz[k].ff) {
		set<int> In0 =In,Out0 =Out;
		ALL_THE(Out0,it) {
			In.insert(*it);
			Out.erase(*it);
			k++;
			bt();
			k--;
			Out =Out0, In =In0;}
		ALL_THE(Num,it) if(In0.find(*it) == In0.end() && Out0.find(*it) == Out0.end()) {
			In.insert(*it);
			k++;
			bt();
			k--;
			In =In0, Out =Out0;}
		int a =(In.empty())?1:(*In.rbegin()+1);
		a =max(a,(Out.empty())?1:(*Out.rbegin()+1));
		In.insert(a);
			k++;
			bt();
			k--;
		In =In0, Out =Out0;
		return;}
	set<int> In0 =In,Out0 =Out;
	ALL_THE(In0,it) {
		Out.insert(*it);
		In.erase(*it);
			k++;
			bt();
			k--;
		Out =Out0, In =In0;}
	ALL_THE(Num,it) if(In0.find(*it) == In0.end() && Out0.find(*it) == Out0.end()) {
		Out.insert(*it);
			k++;
			bt();
			k--;
		In =In0, Out =Out0;}
	int a =(In.empty())?1:(*In.rbegin()+1);
	a =max(a,(Out.empty())?1:(*Out.rbegin()+1));
	Out.insert(a);
			k++;
			bt();
			k--;
	In =In0, Out =Out0;
	return;}

int main() {
	cin.sync_with_stdio(0);
	cin.tie(0);
	srand(time(0));
	int T;
	cin >> T;
	for(int t =0; t < T; t++) {
		cout << "Case #" << t+1 << ": ";
		cin >> N;
		zaz.resize(N);
		Num.clear();
		for(int i =0; i < N; i++) {
			string s;
			int a;
			cin >> s >> a;
			zaz[i] =make_pair((s == "E"),a);
			Num.insert(a);}
		In.clear();
		co =0;
		Out.clear();
		ans =-1;
		k =0;
		bt();
		if(ans == -1) cout << "CRIME TIME\n";
		else cout << ans << "\n";}
	return 0;}

// look at my code
// my code is amazing
