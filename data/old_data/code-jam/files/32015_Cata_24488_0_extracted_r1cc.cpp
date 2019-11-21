#include <iostream>
#include <cstdio>
#include <vector>
#include <algorithm>
using namespace std;

int main()
{
	int N;
	cin>>N;
	for (int test=1; test<=N; test++)
	{
		vector<long long> v;
		v.clear();
		int m,n,x,y,z,i,j;
		cin>>n>>m>>x>>y>>z;
		v.reserve(n);
		long long X,Y;
		X=x%z;Y=y%z;
		vector<int> a(m);
		for (i=0; i<m; i++)
			cin>>a[i];
		for (i=0; i<n; i++)
		{
			v.push_back(a[i%m]);
			a[i%m]=(X*a[i%m]+Y*(i+1))%z;
		}
		/*cout<<"V: ";
		for (i=0; i<n; i++)
			cout<<v[i]<<" ";
		cout<<endl;*/
		vector<long long> t(n, 1);
		long long rez=0;
		for (i=n-1; i>=0; i--)
		{
			for (j=i+1; j<n; j++)
				if (v[i]<v[j])
				{
					t[i]=t[i]+t[j];
					t[i]%=1000000007;
				}
			rez+=t[i];
		}
		//printf("Case #%d: %lld\n", test, rez%1000000007);
		cout<<"Case #"<<test<<": "<<(int)(rez%1000000007)<<endl;
	}
	return 0;
}
