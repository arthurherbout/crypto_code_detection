#include <cstdio>
#include <cstring>
#include <iostream>
#include <algorithm>
#include <queue>

using namespace std;

const int N=100010;
const long long inf=0x1fffffffffffffffll;
struct point{
	long long x,y;
	bool operator<(const point &a){ return y-x<a.y-a.x;}
};

point dis[N];
bool v[N];
int b[N];
queue <int> Q;

int main(){
	int tt,cas,i,j,len,n;
	long long ll;
	cin>>tt;
	for (cas=1;cas<=tt;cas++){
		cin>>ll>>n;
		for (i=1;i<=n;i++)
			cin>>b[i];
		sort(b+1,b+n+1);
		len=b[n];
		for (i=0;i<len;i++){
			dis[i].x=0;
			dis[i].y=inf;
		}
		point p;
		for (i=1;i<=n;i++){
			p.x=b[i]/len;
			p.y=1;
			if (p<dis[b[i]%len]) 
				dis[b[i]%len]=p;
		}
		memset(v,1,len);
		while (!Q.empty()) Q.pop();
		for (i=0;i<len;i++)
			if (dis[i].y<=inf){
				Q.push(i);
				v[i]=false;
			}
		while (!Q.empty()){
			int ch=Q.front();
			Q.pop();
			for (i=1;i<=n;i++){
				int r=b[i]%len;
				int t=(ch+r)%len;
				p.x=dis[ch].x+dis[r].x+(ch+r)/len;
				p.y=dis[ch].y+dis[r].y;
				if (p<dis[t]){
					dis[t]=p;
					if (!v[t]) continue;
					v[t]=false;
					Q.push(t);
				}
			}
			v[ch]=true;
		}
		printf("Case #%d: ",cas);
		if (dis[ll%len].y==inf)
			cout<<"IMPOSSIBLE"<<endl;
		else cout<<(ll/len-dis[ll%len].x+dis[ll%len].y)<<endl;
	}
	return 0;
}
