/*
ID: your_id_here
PROG: test
LANG: C++
*/

#include <algorithm>
#include <iostream>
#include <iterator>
#include <sstream>
#include <fstream>
#include <cassert>
#include <climits>
#include <cstdlib>
#include <cstring>
#include <string>
#include <cstdio>
#include <vector>
#include <cmath>
#include <queue>
#include <deque>
#include <stack>
#include <map>
#include <set>
using namespace std;
int main()
{

    freopen("Al.in" , "r" , stdin);
    freopen("Al.txt" , "w" , stdout);
	int T,N;
	cin>>T;
	int no=0;
	while(T--)
	{
			  cin>>N;
			  int A[N],B[N];
			  for(int i=0;i<N;i++)
			  cin>>A[i]>>B[i];
			  int count=0;
			  for(int i=0;i<N;i++)
			  {
					  for(int j=0;j<N;j++)
					  {
							  if(i==j) continue;
							  if(A[i]>A[j] && B[i]<B[j]) count++;
							  else if(A[i]<A[j] && B[i]>B[j]) count++;
					   		  }
							  }
			  count=count>>1;
			  cout<<"Case #"<<++no<<": "<<count<<endl;
	 		  }
return 0;
}
