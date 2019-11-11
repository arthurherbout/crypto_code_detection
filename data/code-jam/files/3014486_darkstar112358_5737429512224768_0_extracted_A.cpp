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

#define maxN 10005

int A[maxN];

int main()
{
	freopen("input.txt","r",stdin);
	int T;
	scanf("%d",&T);
	int t1 = 1;
	while( T-- )
	{
		int N, K;
		scanf("%d%d",&N,&K);
		for(int i=0;i<N;i++)
			scanf("%d",&A[i]);
		sort(A,A+N);
		int left = 0, right = N-1;
		int ans = N;
		while( left < right )
		{
			if( A[left] <= K - A[right] )
			{
				left++;
				right--;
				ans--;
			}
			else
			{
				right--;
			}
		}
		printf("Case #%d: %d\n",t1,ans);
		t1++;
	}
	return 0;
}
