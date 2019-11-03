#include<iostream>
#include<vector>
#include<string>
#include<cmath>
#include<algorithm>
#include<sstream>
#include<stdio.h>
#include<cstring>
#define fr(i,a,b) for(i=a;i<=b;++i)
#define ll long long
using namespace std;
const ll maxn=1002;
ll bestx,besty,ca,ti,n,i,x[maxn],y[maxn],now;
char ch[maxn];
bool found;
void check(ll a,ll b){
	ll i;
	fr(i,1,n)
		if(max(abs(a-x[i]),abs(b-y[i]))&1){
			if(ch[i]!='#')
				return;
		}
		else
			if(ch[i]!='.')
				return;				
	if(!found||a>bestx||a==bestx&&b>besty){
		found=true;
		bestx=a;
		besty=b;
	}
}
int main(){
	freopen("c2.in","r",stdin);
	freopen("c2.out","w",stdout);
	cin>>ca;
	fr(ti,1,ca){
		cin>>n;
		fr(i,1,n)
			cin>>x[i]>>y[i]>>ch[i];
		found=false;
		check(0,0);
		fr(now,1,20002){
			if(found)
				break;
			fr(i,-now,now){
				check(i,now-abs(i));
				check(i,-(now-abs(i)));
			}
		}		
		if(found)
			cout<<"Case #"<<ti<<": "<<bestx<<" "<<besty<<endl;
		else
			cout<<"Case #"<<ti<<": Too damaged"<<endl;
	}
}

