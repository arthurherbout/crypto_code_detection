#include<iostream>
#include<fstream>
#include<vector>
using namespace std;
//ifstream fin("input.in");
ifstream fin("A-small-attempt1.in");
ofstream fout("output.out");
int main()
{
	__int64 C,n,a,b,c,d,i,j,k,m,x0,y0,x1,x2,x3,y1,y2,y3,p,q,ans;
	fin>>C;
	vector<pair<int,int> > v;
	for(q=0;q<C;++q)
	{
		v.clear();
		ans=0;
		fin>>n>>a>>b>>c>>d>>x0>>y0>>m;
		v.push_back(pair<int,int>(x0,y0));
		//cout<<x0<<" "<<y0<<endl;
		__int64 X=x0, Y=y0;
		for(i = 1 ;i< n;++i)
		{
			X = ((a * X)%m + b) % m;
			Y = ((c * Y)%m + d) % m;
			v.push_back(pair<int,int>(X,Y));
			//cout<<X<<" "<<Y<<endl;
		}
		int sz=v.size();
		for(i=0;i<sz-2;++i)
			for(j=i+1;j<sz-1;++j)
				for(k=j+1;k<sz;++k)
				{
					x1=v[i].first;
					x2=v[j].first;
					x3=v[k].first;
					y1=v[i].second;
					y2=v[j].second;
					y3=v[k].second;
					X=x1+x2+x3;
					Y=y1+y2+y3;
					if(X%3==0 && Y%3==0)
					{
						//X=X/3;
						//Y=Y/3;
						//for(p=0;p<v.size();++p)
						//	if(v[p].first==X && v[p].second==Y)
						//	{
								++ans;
						//		break;
						//	}
					}
				}
	fout<<"Case #"<<q+1<<": "<<ans<<endl;
	}
	return 0;
}

  

		