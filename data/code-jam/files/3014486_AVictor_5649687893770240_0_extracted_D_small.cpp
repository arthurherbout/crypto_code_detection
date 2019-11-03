#include <iostream>
#include <vector>
#include <cstdlib>
#include <cmath>
#include <algorithm>
#include <cstdio>
#include <limits>
#include <set>
#include <map>
#include <cassert>
#include <fstream>
#include <queue>
#include <cstring>

using namespace std;


int N, M;
set< string > letters[10];
int best, cnt;

void rec(vector<set<string> > x, int i)
{
	if (i==M)
	{
		int res = 0;
		for(int j=0;j<N;j++) {
			res += x[j].size();
			if (x[j].size()==0) return;
		}
		if (res > best) { best = res; cnt = 1; 	}
		else if (res == best) { cnt ++; }
		return;
	}
	for(int j=0;j<N;j++)
	{
		vector<set<string> > y(x);
		for(set< string >::iterator it = letters[i].begin(); it!=letters[i].end(); it ++)
		y[j].insert(*it);
		rec(y, i+1);
	}

}
int main()
{
	int test_cnt;
	cin >> test_cnt;
	for(int test_num=1;test_num<=test_cnt;test_num++)
	{
		cin >> M >> N;
		for(int i=0;i<M;i++)
		{
			letters[i].clear();
			string s;
			cin >> s;
			for(int j=0;j<=s.length();j++) 
				letters[i].insert(s.substr(0, j));
		}
		best = 0;
		cnt = 0;
		vector<set<string> > x(N);
		rec(x, 0);
		printf("Case #%d: %d %d\n", test_num, best, cnt);	
	}
    
	return 0;
}
