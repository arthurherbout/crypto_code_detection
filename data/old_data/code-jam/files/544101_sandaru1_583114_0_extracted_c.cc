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
#include <cstring>

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

map<pair<int,int>,bool> dp;

bool win(int a,int b,bool arya) {
  if (a==b)
    return !arya;
  if (a<=0 || b<=0)
    return arya;
  
  pair<int,int> dpx;
  dpx.first = a;
  dpx.second = b;
  
  if (dp.find(dpx)!=dp.end()) {
    if (dp[dpx])
      return arya;
    return !arya;
  }
  
  dp[dpx] = true;
  int start = b/a;
  
  for(int k=start;k*a<=b && k>0;k--) {
    int nb  = b-a*k;
    bool res = win(min(a,nb),max(a,nb),!arya);
    if (res==arya)
      return arya;
  }
  dp[dpx] = false;
  return !arya;
}

int main() {
	int t;
	
	ifstream fin("c.in");
	ofstream fout("c.ans");
	
	fin >> t;
	
	fr(x,0,t) {
	  int a1,a2,b1,b2;
	  fin >> a1 >> a2 >> b1 >> b2;
	  int res = 0;
	  for(int a=a1;a<=a2;a++) {
	    for(int b=b1;b<=b2;b++) {
	      if (win(min(a,b),max(a,b),true)) {
	        res++;
	      }
	    }
	  }
		cout << "Case #" << x+1 << ": " << res << endl;
		fout << "Case #" << x+1 << ": " << res << endl;
	}
	
	fin.close();
	fout.close();
	return 0;
}
