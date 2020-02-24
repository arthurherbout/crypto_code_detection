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

#define inf 1000000000

int main()
{

	freopen("input.txt","r",stdin);
	int T;
	scanf("%d",&T);
	for(int t1=1;t1<=T;t1++)
	{

		int N, L;
		scanf("%d%d",&N,&L);

		vector<string> A;
		for(int i=0;i<N;i++)
		{
			string S;
			cin>>S;
			A.push_back(S);
		}

		sort(A.begin(), A.end());

		vector<string> B;
		for(int i=0;i<N;i++)
		{
			string S;
			cin>>S;
			B.push_back(S);
		}

		int ans = inf;

		for(int x=0;x<B.size();x++)
		{
			vector<int> change;
			for(int i=0;i<A[0].size();i++)
				if( A[0][i] != B[x][i] )
					change.push_back(i);
			vector<string> C = B;
			for(int j=0;j<change.size();j++)
			{
				for(int i=0;i<C.size();i++)
					if( C[i][change[j]] == '0' )
						C[i][change[j]] = '1';
					else
						C[i][change[j]] = '0';
			}
			sort(C.begin(), C.end());
			// for(int i=0;i<C.size();i++)
			// 	cout<<C[i]<<endl;
			// cout<<"-------------\n";
			if( A == C )
			{
				int y = change.size();
				ans = min(ans,y);
			}
		}

		printf("Case #%d: ",t1);
		if( ans == inf )
			printf("NOT POSSIBLE\n");
		else
			printf("%d\n",ans);

	}

	return 0;

}
