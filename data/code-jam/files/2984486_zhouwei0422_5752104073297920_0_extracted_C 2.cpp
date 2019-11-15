#include <algorithm>  
#include <bitset>  
#include <cctype>  
#include <cmath>  
#include <complex>  
#include <cstdio>  
#include <cstdlib>  
#include <cstring>  
#include <ctime>  
#include <deque>  
#include <functional>  
#include <iomanip>  
#include <iostream>  
#include <fstream>
#include <list>  
#include <map>  
#include <numeric>  
#include <queue>  
#include <set>  
#include <sstream>  
#include <stack>  
#include <string>  
#include <utility>  
#include <vector>   
using namespace std;

ifstream ifile;
ofstream ofile;

int T,cases;
int N;

int v[1001];

void solve()
{

	ifile>>N;

	for(int i = 0; i < N; i++)
		ifile>>v[i];
	
	vector<int> dp(N);

	for(int i = 0; i < N; i++)
		dp[i]=1;

	int res = 0;
	for(int i = 0; i < N; i++)
	{
		for(int j = 0; j < i ; j++)
		{
			if(v[i]>v[j])
			{
				dp[i]=max(dp[i],1+dp[j]);
			}
		}
		res = max(res, dp[i]);
	}

	if(res>N/2)
		ofile<<"Case #"<<cases<<": "<<"BAD"<<endl;
	else
		ofile<<"Case #"<<cases<<": "<<"GOOD"<<endl;
}

int main(void)
{
	ifile.open("input.txt");
	ofile.open("output.txt");

	ifile>>T;
	for(int i = 0; i < T; i++)
	{
		cases=i+1;
		solve();
	}

	ifile.close();
	ofile.close();
	return 0;
}