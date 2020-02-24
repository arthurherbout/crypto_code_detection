#include<iostream>
#include<vector>
#include<string>
#include<map>
#include<queue>
#include<stack>
#include<cfloat>
#include<algorithm>
#define e 1e-6
#define M 55
#define SIM 1000000
using namespace std;

long long int_pow(int base, int exp)
{
    long long result = 1;
    while (exp)
    {
        if (exp & 1)
           result *= base;
        exp /= 2;
        base *= base;
    }
    return result;
}

bool equal(const double& a, const double& b)
{
	return (a + e >= b) && (a - e <= b);
}
int main(void)
{
	int T;
	cin >> T;
	for (int z = 1; z <= T; ++z)
	{
		cerr << z << endl;
		int n, k;
		double u, tmp;
		vector<double> p;
		cin >> n >> k >> u;
		priority_queue<double, vector<double>, greater<double> > pq;
		
		for(int i = 0; i < n; ++i)
		{
			cin >> tmp;
			p.push_back(tmp);
		}
		
		sort(p.begin(), p.end());
		
		for (int i=1; i <= k; ++i)
			pq.push(p[n - i]);
		
		while (u > e)
		{
			double now = pq.top();
			int c = 1;
			pq.pop();
			while (!pq.empty() && equal(pq.top(), now))
			{
				c++;
				pq.pop();
			}
			
			if (!pq.empty())
			{
				double goal = pq.top();
				if ((goal - now)*c < u + e )
				{
					for(int i = 0; i < c ; ++i)
						pq.push(goal);
					u -= (goal - now)*c;
				}
				else
				{
					for(int i = 0; i < c ; ++i)
						pq.push(now + u/c);
					u = 0;
				}
			}
			else
			{
				for(int i = 0; i < c ; ++i)
					pq.push(min(1., now + u/c));
				u = 0;
			}
		}
		
		for (int i=1; i <= k; ++i)
		{
			p[n - i] = pq.top();
			pq.pop();
		}
		
		int count = 0;
		for (int i = 0 ; i < SIM; ++i)
		{
			int s = 0;
			for (int j = 0; j < n ; ++j)
			{
				double r = (double)rand() / RAND_MAX;
				if (p[j] >= r)
					s++;
			}
			if (s >= k)
				count++;
		}
		
		double ans = ((double)count) / SIM;
		printf("Case #%d: %.9lf\n", z, ans);
	}
}

