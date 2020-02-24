// cheburashka, bear-mouse, team template

#include <cstdlib>
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <map>
#include <algorithm>
#include <cstdio>
#include <stack>
#include <cstring>
#include <cmath>
#include <queue>
#include <set>
using namespace std;

typedef long long ll;
typedef vector < string > vs;
typedef vector <int> vi;
#define FOREACH(it,c) for(typeof((c).begin()) it=(c).begin();it!=(c).end();++it)
#define deb(x) cout << #x << ": " << x << endl;
#define debv(x) for(int i = 0; i < (x).size(); i++) cout << x[i] << ' '; cout << endl;
#define pb(x) push_back(x)

//string split given string a and delimiters
vs strsp(string a, string delim=" ")
{
  vs ret;
  string cr = "";
  for(int i = 0; i < a.size(); i++)
  {
    if(delim.find(a[i])==string::npos) cr += a[i];
    else { if(cr.size()) ret.push_back(cr); cr = ""; }
  }
  if(cr.size()) ret.push_back(cr);
  return ret;
}
//long long gcd using Euclid
long long gcd(long long a, long long b)
{
	long long tmp = 0;
	while(b != 0)
	{
		tmp = b;
		b = a % b;
		a = tmp;
	}
	return a;
}
//long long egcd
//if original arguments are X,M then X*first - M*second = 1, so X*first = 1 mod M
pair<long long, long long> egcd(long long a, long long b)
{
	if(a % b == 0)
	{
		return make_pair(0,1);
	}
	else
	{
		pair<long long, long long> ret = egcd(b,a%b);
		long long Q = a / b;
		return make_pair(ret.second, ret.first - ret.second * Q);
	}
}
const int OFFSET = 1000505;
		int keeps[2*OFFSET];
int main()
{
	int T;
	cin >> T;
	for(int tcase = 1; tcase <= T; tcase++)
	{
		int CN;
		cin >> CN;
		set<int> Q;
		int msum[205];
		msum[1] = msum[0] = 0;
		msum[2] = msum[3] = 1;

		memset(keeps,0,sizeof(keeps));
		
		for(int i = 4; i < 205; i += 2)
		{
			int m = i / 2;
			int next = m*(m-1)+m + msum[i-2];
			msum[i] = msum[i+1] = next;
		}

		
		for(int i = 0; i < CN; i++)
		{
			int V,P;
			cin >> P >> V;
			keeps[P+OFFSET] = V;
			Q.insert(P+OFFSET);
		}
		int tot = 0;
		
		while(true)
		{
			int found = 0;
			set<int> Q2;
			FOREACH(it, Q)
			{
				int cn = *it;
				if(keeps[cn] <= 1) continue;
				found = 1;
				tot += msum[keeps[cn]];
				for(int i = 1; i <= keeps[cn]/2; i++)
				{
					keeps[cn-i]++;
					keeps[cn+i]++;
					if(keeps[cn-i] > 1) Q2.insert(cn-i);
					if(keeps[cn+i] > 1) Q2.insert(cn+i);
				}
				if(keeps[cn]%2 == 1)
				{
					keeps[cn] = 1;
				}
				else keeps[cn] = 0;
			}
			if(found == 0) break;
			//deb(Q2.size());
			Q = Q2;
		}
		printf("Case #%d: %d\n",tcase,tot);
	}
	return 0;
}
