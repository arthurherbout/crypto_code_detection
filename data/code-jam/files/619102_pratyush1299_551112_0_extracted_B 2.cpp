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

    freopen("B.in" , "r" , stdin);
    freopen("B.txt" , "w" , stdout);
	int T,L,P,C;
	cin>>T;
	int no=0;
	while(T--)
	{
			  cin>>L>>P>>C;
			  int tt=0;
			  while(L*C<P)
			  {
//						   L=L*C;
						   C=C*C;
						   tt++;
			   			   }
			  cout<<"Case #"<<++no<<": "<<tt<<endl;
	 		  }
return 0;
}
