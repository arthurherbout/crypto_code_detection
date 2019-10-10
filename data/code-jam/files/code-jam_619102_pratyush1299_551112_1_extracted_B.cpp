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

    freopen("Bl.in" , "r" , stdin);
    freopen("Bl.txt" , "w" , stdout);
	long long T,L,P,C,L1,L2,ans;
int no=0;
	cin>>T;
	while(T--)
	{
	cin>>L>>P>>C;
    ans=0;
	L1=P/L+!!(P%L);
	L2=C;
	while(L1>L2)
	{
				L2++;L2--;
	L2=L2*L2;
	ans++;
	}
	cout<<"Case #"<<++no<<": "<<ans<<endl;
	 		  }
return 0;
}
