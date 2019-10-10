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

int solve(int r,int c,int n)
{
	vector<int> q(r*c,0);
	for (int i = 0; i < n; ++i) {
		q[i]=1;
	}
	sort(q.begin(),q.end());
	int ret = INT_MAX;
	do{
		// for (int i = 0; i < r*c; ++i)
		// {
		// 	cout<<q[i]<<" ";
		// }
		// cout<<endl;
		int mat[r][c];
		for (int i = 0; i < r; ++i) {
			for (int j = 0; j < c; ++j) {
				mat[i][j] = q[i*c+j];
				// if(q[i*r+j])
				// 	mat[i][j]=1;
				// else
				// 	mat[i][j]=0;
				// cout<<mat[i][j]<<endl;
			}
		}
		// for (int i = 0; i < r; ++i) {
		// 	for (int j = 0; j < c; ++j) {
		// 		cout<<mat[i][j]<<" ";
		// 	}
		// 	cout<<endl;
		// }

		int cnt=0;
		for (int i = 0; i < r; ++i) {
			for (int j = 0; j < c; ++j) {
				if(mat[i][j]==0)
					continue;
				if((i+1<r)&&(mat[i+1][j]==1))
					cnt++;
				if((j+1<c)&&(mat[i][j+1]==1))
					cnt++;
			}
		}
		// cout<<cnt<<endl;
		ret = min(ret,cnt);
	}while(next_permutation(q.begin(),q.end()));
	return ret;
}

int main()
{
	int t;
	cin>>t;
	for (int T = 1; T <= t; ++T) {
		int N,R,C;
		cin>>R>>C>>N;
		int ans = solve(R,C,N);
		printf("Case #%d: %d\n",T,ans);
	}
	return 0;
}