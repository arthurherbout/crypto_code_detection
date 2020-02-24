#include <iostream>
#include <string>
#include <math.h>

#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <algorithm>

#define REP(i,n) for(int i=0;i<(n);i++)
#define FOR(v,p,k) for(int v=p;v<=k;v++)
#define SQUARE(x) x*x

using namespace std;
typedef long long ll;

typedef std::pair<long,int> mypair;

bool comparator ( const mypair& l, const mypair& r)
{
	return l.first > r.first; 
}

int main()
{
	int T;
	long E,R,N;
	vector<mypair> v(10001);
	long minE[1001];//minimale, avant l'activité
	long maxE[1001];//maxi dispo, avant
	long long res;
	bool was_done[1000];
	cin>>T;
	FOR(c,1,T)
	{
		res=0;
		cin>>E;
		cin>>R;
		cin>>N;
		REP(i,N)
		{
			minE[i]=0;
			maxE[i]=E;
			cin>>v[i].first;
			v[i].second=i;
			was_done[i]=false;
		}
		sort(v.begin(),v.begin()+N,comparator); 
		REP(i,N)
		{
			int ind=v[i].second;
			res+=v[i].first*(maxE[ind]-minE[ind]);
			int currentE=maxE[ind]-R;
			int ind2=ind-1;
			was_done[ind]=true;
			while(currentE>0 && ind2>=0 && !was_done[ind2])
			{
				minE[ind2]=currentE;
				ind2--;
				currentE-=R;
			}
			currentE=minE[ind]+R;
			ind2=ind+1;
			while(currentE<E && ind2<N && !was_done[ind2])
			{
				maxE[ind2]=currentE;
				ind2++;
				currentE+=R;
			}
		}
		printf("Case #%d: %lld\n",c,res);
	}
}
