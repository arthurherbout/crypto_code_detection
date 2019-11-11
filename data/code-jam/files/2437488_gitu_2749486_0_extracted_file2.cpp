/*------Coded by Gitanshu Behal------*/
#include<iostream>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include<cctype>
#include<ciso646>
#include<stdbool.h>
#include<cstddef>
#include<vector>
#include<stack>
#include<queue>
#include<map>
#include<algorithm>
#include<complex>
#include<functional>
#include<numeric>
#include<limits>
#include<stdexcept>
#include<string>
#include<utility>
#include<fstream>


		/*--------scan/print--------*/
# define s(n) scanf("%d",&n)
# define p(n) printf("%d",n)
# define pln(n) printf("%d\n",n)
# define sln(n) scanf("%d\n",&n)
# define plld(x) printf("%lld\n",x)

		/*--------FOR LOOPS--------*/
# define For(I,l,m) for(int I=l;I<=m;I++)
# define ForInt(I,l,m) for(I=l;I<=m;I++)
# define NFor(I,l,m) for(int I=l;I>=m;I--)
# define NForInt(I,l,m) for(I=l;I>=m;I--)

		/*--------others--------*/
# define TEST int t; scanf("%d",&t); while(t--)
# define ZERO(x) memset(x,0,sizeof(x))
# define SET(x,y) memset(x,y,sizeof(x))
# define NEWLINE printf("\n")

/*--------2 option--------*/
# define YES printf("YES\n")
# define NO printf("NO\n")

/*------DIRECTELY USABLE----> sort(array,array+size),swap(a,b),------*/

using namespace std;

typedef long long ll;
typedef vector<int> vi;
typedef pair<int, int> ii;
typedef vector<ii> vii;
typedef map<string, int> msi;


map<ii,int> visit;
int bound_x,bound_y;
int X[4] = {1,0,-1,0};
int Y[4] = {0,1,0,-1};


string BFS(ii s)
{
	string answer="";
	int k=1;
	ii sour;
	sour.first=0;
	sour.second=0;
	queue<ii> q;
	q.push(sour);
	visit[sour]=0;
	int flag=0;
	while (!q.empty()) 
	{
		ii u = q.front(); q.pop();
		k=visit[u]+1;
		For(i,0,3)
		{
					ii temp;
					temp.first=u.first+k*X[i];
					temp.second=u.second+k*Y[i];
			//		cout<<u.first+k*X[i]<<" "<<u.second+k*Y[i]<<" "<<k<<"\n";
					if(visit[temp] == 0 && !(temp.first==0 && temp.second==0))
					{
						visit[temp] = k;
						if(u.first+k*X[i]==s.first && u.second+k*Y[i]==s.second)
						{
							flag=1;
							break;
						}
						q.push(temp);
					}
		}
//		cout<<"yahan"<<flag;
			
		if(flag==1)
		{
			queue<ii> q2;
			q2.push(s);
			while (!q2.empty()) 
			{
			//	cout<<"yahan"<<k;
				if(k==0)
				{
					return answer;	
				}			
				ii u = q2.front(); q2.pop();
				For(i,0,3)
				{
							ii temp1;
							temp1.first=u.first+k*X[i];
							temp1.second=u.second+k*Y[i];
							if(visit[temp1] == k-1 && k!=1)
							{
//								cout<<"Y";
								if(X[i]==1 && Y[i]==0)
								answer=answer+'W';
								else if(X[i]==-1 && Y[i]==0)
								answer=answer+'E';
								else if(X[i]==0 && Y[i]==1)
								answer=answer+'S';
								else if(X[i]==0 && Y[i]==-1)
								answer=answer+'N';
								q2.push(temp1);
								break;
							}
							if(k==1 && temp1.first==0 && temp1.second==0)
							{
								if(X[i]==1 && Y[i]==0)
								answer=answer+'W';
								else if(X[i]==-1 && Y[i]==0)
								answer=answer+'E';
								else if(X[i]==0 && Y[i]==1)
								answer=answer+'S';
								else if(X[i]==0 && Y[i]==-1)
								answer=answer+'N';
								q2.push(temp1);
								break;
								
							}
				}
				k--;
				
			}
		//	return answer;
		}
	} 
}

/*-------begin of main---------*/
int main()
{
	int t;
	s(t);
	for(int G=0;G<t;G++)
	{
		visit.clear();
		int x,y;
		cin>>x>>y;
		ii temp;
		temp.first=x;
		temp.second=y;
		//cout<<"njfd  "<<visit[temp];
		string ans;
		ans=BFS(temp);
		string yehai="";
		for(int k=ans.length()-1;k>=0;k--)
		{
			yehai=yehai+ans[k];
		}
		cout<<"Case #"<<G+1<<": "<<yehai<<"\n";
		//cout<<ans;
		//printf("Case #%d: %d %d\n",G+1,ans1+1,ans2+1);
		
	}	
}
