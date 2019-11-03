#include<iostream>
#include<algorithm>
#include<cmath>
#include<cstring>
#include<string>
#include<memory.h>
#include<queue>
#include<stack>
using namespace std;

struct Stat
{
	int n,dd,dp,ds;
	int t,d,w,e,s;
};
struct cmp
{
	bool operator()(const Stat &a,const Stat &b)
	{
		return a.d>b.d;
	}
};
int w[401];
int& wall(int n)
{
	return w[n+200];
}
int main()
{
	freopen("input.txt","r",stdin);
	freopen("output.txt","w",stdout);
	int nc;
	cin>>nc;

	int N;
	Stat st,st2;
	bool succ;
	int count;
	queue<Stat> q;
	priority_queue<Stat,vector<Stat>,cmp> pq;
	for(int tc=1;tc<=nc;tc++)
	{
		cin>>N;
		memset(w,0,sizeof(w));
		count=0;
		for(int i=0;i<N;i++)
		{
			cin>>st.d>>st.n>>st.w>>st.e>>st.s>>st.dd>>st.dp>>st.ds;
			st.t=0;
			pq.push(st);
		}
		while(!pq.empty())
		{
			st=pq.top();
			pq.pop();
			succ=0;
			if(!q.empty()&&q.front().d!=st.d)
				while(!q.empty())
				{
					for(int j=q.front().w;j<q.front().e;j++)
						if(wall(j)<q.front().s)
							wall(j)=q.front().s;
					q.pop();
				}
			for(int i=st.w;i<st.e;i++)
				if(wall(i)<st.s)
				{
					succ=1;
					q.push(st);
					break;
				}
			if(succ)
				count++;
			if(st.t+1<st.n)
			{
				st2=st;
				st2.t++;
				st2.d+=st2.dd;
				st2.w+=st2.dp;
				st2.e+=st2.dp;
				st2.s+=st2.ds;
				pq.push(st2);
			}
		}
		cout<<"Case #"<<tc<<": "<<count<<endl;
	}	
	return 0;
}