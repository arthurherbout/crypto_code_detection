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

int N;
int A[maxN];
int B[maxN];
int L1[maxN];
int L2[maxN];	//elements to the left greater than the current element
int L3[maxN];	//elements to the right which are smaller than this element
vector<int> R;

int calc1(int val)
{
	int ans = 0;
	for(int i=0;i<R.size();i++)
		if( R[i] > val )
			ans++;
	return ans;
}

int solve(int c)
{
	int maxPos = 0;
	for(int i=0;i<N;i++)
		if( A[i] > A[maxPos] )
			maxPos = i;
	if( c )
	{
		for(int i=0;i<N;i++)
			A[i] = -A[i];
	}
	for(int i=0;i<maxPos;i++)
		L1[i] = A[i];
	for(int i=0;i<maxPos;i++)
	{
		L2[i] = 0;
		for(int j=0;j<i;j++)
			if( L1[j] > L1[i] )
				L2[i]++;
		L3[i] = 0;
		for(int j=i+1;j<maxPos;j++)
			if( L1[j] < L1[i] )
				L3[i]++;
	}
	R.clear();
	for(int i=maxPos+1;i<N;i++)
		R.push_back(A[i]);
	int cR = 0;
	for(int i=0;i<R.size();i++)
	{
		for(int j=i+1;j<R.size();j++)
			if( R[j] > R[i] )
				cR++;
	}
	int cL = 0;
	for(int i=0;i<maxPos;i++)
		cL += L2[i];
	int ans = cL + cR;
	for(int x=maxPos;x>0;x--)
	{
		int v1 = 0;
		int pos = -1;
		for(int i=0;i<x;i++)
		{
			int val = L2[i] + L3[i] - (x-i) - calc1(L1[i]); 
			if( val > v1 )
			{
				v1 = val;
				pos = i;
			}
		}
		if( pos == -1 )
			return ans;
		for(int i=0;i<pos;i++)
			if( L1[i] > L1[pos] )
				L3[i]--;
		for(int i=pos+1;i<x;i++)
			if( L1[i] < L1[pos] )
				L2[i]--;
		for(int i=pos+1;i<x;i++)
		{
			swap(L1[i],L1[i-1]);
			swap(L2[i],L2[i-1]);
			swap(L3[i],L3[i-1]);
		}
		R.push_back(L1[x-1]);
		ans -= v1;
	}
	return ans;
}

int main()
{
	freopen("input.txt","r",stdin);
	int T;
	scanf("%d",&T);
	int t1 = 1;
	while( T-- )
	{
		scanf("%d",&N);
		for(int i=0;i<N;i++)
			scanf("%d",&A[i]);
		int ans = solve(0);
		for(int i=0;i<N;i++)
			B[N-1-i] = A[i];
		for(int i=0;i<N;i++)
			A[i] = B[i];
		ans = min(ans,solve(1));
		printf("Case #%d: %d\n",t1,ans);
		t1++;
	}
	return 0;
}
