#include<iostream>
#include<fstream>
#include<vector>
using namespace std;
//ifstream fin("input.in");
ifstream fin("B-small-attempt0.in");
ofstream fout("output.out");
bool isPrime(int n)
{
	int i;
	if(n<2)
		return 0;
	for(i=2;i*i<=n;++i)
		if(n%i==0)
			return false;
	return true;
}
int count;
vector<vector<int> > g;
int done[1001];
void dfs(int n)
{
	int i;
	//cout<<n<<" ";
	for(i=0;i<g[n].size();++i)
	{
		if(!done[g[n][i]])
		{
			done[g[n][i]]=1;
			dfs(g[n][i]);
		}
	}
}


int main()
{
	vector<int> v;
	bool p[1001];
	int i,j,k;
	for(i=0;i<1001;++i)
		p[i]=0;
	for(i=2;i<=1000;++i)
		if(isPrime(i))
		{
			p[i]=1;
			v.push_back(i);
	//		cout<<"prime "<<i<<endl;
		}
	//	cout<<"Size "<<v.size()<<endl;
	int C;
	fin>>C;
	int a,b,m,q;

	for(i=0;i<C;++i)
	{
		fin>>a>>b>>m;
		g.clear();
		g.resize(1001);
		for(j=0;j<1001;++j)
			done[j]=0;
		
		for(j=a;j<b;++j)
		{
			for(k=j+1;k<=b;++k)
			{
				for(q=0;q<v.size();++q)
				{
					if(v[q]>=m && j%v[q]==0 && k%v[q]==0)
					{
						g[j].push_back(k);
						g[k].push_back(j);
	//					cout<<j<<" "<<k<<" "<<v[q]<<endl;
					}
				}
			}
		}
		count=0;
		for(j=a;j<=b;++j)
		{
			if(!done[j])
			{
				++count;
	//			cout<<endl;
				done[j]=1;
				dfs(j);
			}
		}
		fout<<"Case #"<<i+1<<": "<<count<<endl;
	}
	return 0;
}