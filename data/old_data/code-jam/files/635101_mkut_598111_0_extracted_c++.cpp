#include <iostream>
#include <algorithm>
#include <map>
using namespace std;

int c(int a, int b)
{
	static map<pair<int,int>,int> memo;
	if(a < b*2)
		return c(a, a-b);
	if(memo.count(pair<int,int>(a,b)) > 0)
		return memo[pair<int,int>(a,b)];
	
	long long tmp = 1;
	for(int i = 0; i < b; i++)
	{
		tmp *= (a-i);
		tmp %= 100003;
	}
	long long div = 1;
	for(int i = 1; i <= b; i++)
	{
		div *= i;
		div %= 100003;
	}
	long long ret = 0;
	while(ret*div % 100003 != tmp)
		ret++;
	memo[pair<int,int>(a,b)] = ret;
	return ret;
}

class solver
{
	int n;
	map<pair<int,int>,int> memo;
	
public:
	solver(int N)
	{
		n = N;
	}
	
	int solve()
	{
		int ans = 0;
		for(int i = 2; i <= n; i++)
		{
			ans += solve(i, i-1);
			ans %= 100003;
		}
		return ans;
	}
	
private:
	int solve(int p, int d)
	{
		if(p == n)
			return 1;
		
		if(memo.count(pair<int,int>(p,d)) > 0)
			return memo[pair<int,int>(p,d)];
		
		int ret = 0;
		for(int i = d; p + i <= n; i++)
		{
			ret += solve(p+i, i) * c(i-1, d-1);
			ret %= 100003;
		}
		
		memo[pair<int,int>(p,d)] = ret;
		return ret;
	}
};

int main()
{
	int T;
	cin >> T;
	for(int No = 1; No <= T; No++)
	{
		int n;
		cin >> n;
		solver prob(n);
		cout << "Case #" << No << ": " << prob.solve() << endl;
	}
	
	return 0;
}
