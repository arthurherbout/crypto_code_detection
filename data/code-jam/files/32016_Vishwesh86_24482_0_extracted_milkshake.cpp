#include<iostream>
#include<fstream>
#include<vector>
using namespace std;
#define bit(n,i) ((n&(1<<(i)))!=0)
ifstream fin("B-small-attempt5.in");
//ifstream fin("input.in");
ofstream fout("output.out");
vector<vector<pair<int,int> > > v;
int main()
{
	int c,n,m,i,j,k,p,q,t,x;
	fin>>c;
	for(i=0;i<c;++i)
	{
		v.clear();
		fin>>n;
		fin>>m;
		v.resize(m);
		for(j=0;j<m;++j)
		{
			fin>>t;
			v[j].resize(t);
			for(k=0;k<t;++k)
				fin>>v[j][k].first>>v[j][k].second;
		}
		int mincount=100000,ans=-1;
		bool flag=0;
		for(j=0;j<= ((1<<n)-1);++j)// 1 malted 0 unmalted
		{
			
			//for(x=0;x<n;++x)
			//	if(bit(j,x))//atleast 1 malted
			//	{
					flag=0;
					for(p=0;p<v.size();++p)
					{
						flag=false;
						for(q=0;q<v[p].size();++q)
						{
							if(bit(j,v[p][q].first-1)==v[p][q].second)
							{
								flag=true;
								break;
							}
						}
						if(!flag)
							break;
					}
			//		if(!flag)
			//			break;
				
					if(flag)
					{
						int count=0;
						for(k=0;k<n;++k)
							if(bit(j,k))
								++count;
						if(count<mincount)
						{
							ans=j;
							mincount=count;
						}
					}
			//		break;
			//	}
		}
		fout<<"Case #"<<i+1<<": ";
		if(ans==-1)
			fout<<"IMPOSSIBLE"<<endl;
		else
		{
			for(j=0;j<n;++j)
			{
				if(j==0)
					fout<<bit(ans,j);
				else
					fout<<" "<<bit(ans,j);
				
			}
			fout<<endl;
		}
				
	}
}





