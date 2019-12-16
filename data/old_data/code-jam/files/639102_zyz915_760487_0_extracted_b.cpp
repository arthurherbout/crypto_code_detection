#include <cstdio>
#include <cstring>
#include <iostream>

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

int main(){
	int tt,cas,i,j,len,n;
	long long L;
	cin>>tt;
	for (cas=1;cas<=tt;cas++){
		cin>>L>>n;
		for (i=1;i<=n;i++)
			cin>>b[i];
		sort(b+1,b+n+1);
		len=b[n];
		for (i=0;i<len;i++){
			dis[i].x=0;
			dis[i].y=inf;
		}
		for (i=1;i<=n;i++){
			dis[b[i]%len].x=b[i]/len;
			dis[b[i]%len].y=1;
		}
		memset(v,1,len);
		point p;
		for (j=0;j<len;j++){
			int ch=-1;
			for (i=0;i<len;i++)
				if (v[i]&&(ch==-1||dis[i]<dis[ch]))
					ch=i;
			if (ch==-1||dis[ch].y==inf) break;
			v[ch]=false;
			for (i=1;i<=n;i++){
				int r=b[i]%len;
				int t=(ch+r)%len;
				p.x=dis[ch].x+dis[r].x+(ch+r)/len;
				p.y=dis[ch].y+dis[r].y;
				if (p<dis[t]) dis[t]=p;
			}
		}
		printf("Case #%d: ",cas);
		if (dis[L%len].y==inf)
			cout<<"IMPOSSIBLE"<<endl;
		else cout<<(L/len-dis[L%len].x+dis[L%len].y)<<endl;
	}
	return 0;
}
