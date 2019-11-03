#include <vector>
#include <list>
#include <map>
#include <set>
#include <queue>
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
#include <climits>
#include <string>
#include <cstring>
#include <cassert>

using namespace std;

int solve(vector<pair<double,double>> &p)
{
	if(p.size()==1)
		return 0;
	sort(p.begin(),p.end());
	auto p1 = p[0];
	auto p2 = p[1];
	// if(p1.first==p2.first)
	// 	return 0;
	double ti = p1.second*(360-p1.first);
	double dist = p2.second*ti;
	if(p1.second>=p2.second) {
		if(dist<=360*p2.first)
			return 1;
		return 0;
	}else {
		if(dist>=360*p2.first)
			return 1;
		return 0;
	}
}

int main()
{
	int t;
	cin>>t;
	for (int T = 1; T <= t; ++T) {
		int N,cnt=0;
		vector<pair<double,double>> p;
		cin>>N;
		for (int i = 0; i < N; ++i) {
			int D,H,M;
			cin>>D>>H>>M;
			for (int j = 0; j < H; ++j) {
				p.push_back({(double)D,(double)(M+j)});
			}
		}
		int ans = solve(p);
		printf("Case #%d: %d\n",T,ans);
	}
	
	return 0;
}