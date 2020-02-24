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
#include <cstring>
#include <fstream>

using namespace std;

#define maxN 1005

int A[maxN];
int vis[maxN];

map < vector < int > , int > Map;
map < vector < int > , int >::iterator it;

void solve(int pos,vector<int> A1)
{

	if( pos == A1.size() )
	{
		if( Map.count(A1) > 0 )
			Map[A1]++;
		else
			Map[A1] = 1;
		return;
	}

	for(int i=0;i<A1.size();i++)
	{
		vector<int> B = A1;
		swap(B[pos],B[i]);
		solve(pos+1,B);
	}

}

void dfs(int pos)
{
	if( vis[pos] == 1 )
		return;
	vis[pos] = 1;
	dfs(A[pos]);
}

int main()
{

	#if 0

	vector<int> A1;
	A1.push_back(0), A1.push_back(1), A1.push_back(2), A1.push_back(3), A1.push_back(4), A1.push_back(5);

	solve(0,A1);

	int denm = 0;
	for(it=Map.begin();it!=Map.end();it++)
	{
		denm += (*it).second;
		// cout<<": "<<(*it).second<<endl;
	}

	vector < pair < double , vector <int> > > B; 

	for(it=Map.begin();it!=Map.end();it++)
	{
		vector<int> temp = (*it).first;
		// for(int i=0;i<temp.size();i++)
		// 	cout<<temp[i]<<" ";
		// cout<<": "<<(double)(*it).second/(double)denm<<endl;
		B.push_back(make_pair((double)(*it).second/(double)denm,temp));
	}

	sort(B.begin(), B.end());

	for(int i=0;i<B.size();i++)
	{
		vector<int> temp = B[i].second;
		cout<<B[i].first<<": ";
		for(int j=0;j<temp.size();j++)
			cout<<temp[j]<<" ";
		cout<<" --> ";
		int inv = 0;
		for(int i=0;i<temp.size();i++)
			for(int j=i+1;j<temp.size();j++)
				if( temp[i] > temp[j] )
					inv++;
		cout<<inv;
		cout<<endl;

	}

	#endif

	#if 1

	freopen("input.txt","r",stdin);
	
	int T;
	scanf("%d",&T);
	
	for(int t1=1;t1<=T;t1++)
	{

		printf("Case #%d: ",t1);

		int N;
		scanf("%d",&N);

		for(int i=0;i<N;i++)
			scanf("%d",&A[i]);

		int c = 0;

		for(int i=0;i<N;i++)
		{
			for(int j=i+1;j<N;j++)
				if( A[i] > A[j] )
					c++;
		}

		// cout<<"yo "<<c<<" ";
		if( 2*c <= N*(N-1)/2 )
			printf("BAD\n");
		else
			printf("GOOD\n");
	
	}

	#endif

	return 0;

}
