#include<cstdio>
#include<cstdlib>
#include<cstring>
#include<cmath>
#include<cassert>
#include<algorithm>
#include<iostream>
#include<sstream>
#include<vector>
#include<string>
#include<set>
#include<map>
#include<queue>

using namespace std;

#define repp(i,a,b) for(int i=(int)(a);i<=(int)(b);++i)
#define rep(i,n) repp(i,0,(n)-1)

#define pb push_back
#define mp make_pair

typedef long long ll;
typedef pair<int,int> pint;

int in(){int a; scanf("%d",&a); return a;}
int dx[4]={1,0,-1,0},dy[4]={0,1,0,-1};

typedef pair<pint,int> tint;
#define one first.first
#define two first.second
#define thr second


int main(){
	int ttt=in();
	rep(cas,ttt){
		int n=in(); int m=in();
		char d[10010][15];
		char l[110][30];
		bool ind[10010][30]={{}};
		int len[10010];
		rep(i,n) scanf("%s",d[i]);
		rep(i,m) scanf("%s",l[i]);
		rep(i,n) len[i]=strlen(d[i]);
		rep(i,n){
			rep(j,strlen(d[i]))
				ind[i][d[i][j]-'a']=1;
		}
		cout << "Case #" << cas+1 << ": ";
		rep(cl,m){
			int lmax=-1; int res=-1;
//			cout << endl << "cl:" << cl << endl;
			rep(cd,n){
//					cout << " cd: " << cd << endl;
				bool bad[10010]={};
				rep(j,n) if(len[cd]!=len[j]) bad[j]=1;
				int lose=0;
				rep(i,26){
//					cout << "  i: " << i << " " << l[cl][i] << endl;
					bool choose=0;
					rep(td,n){
						if(!bad[td]) if(ind[td][l[cl][i]-'a']){
							choose=1; break;
						}
					}
					if(choose){
//						 cout << "    choose: " << endl; 
						if(!ind[cd][l[cl][i]-'a']) ++lose;
						rep(j,n)if(len[cd]==len[j]){
							rep(k,len[cd]){
								if((d[cd][k]==l[cl][i]) != (d[j][k]==l[cl][i])){
									bad[j]=1;
									break;
								}
							}
						}
					}else{
					}
/*					cout << "     ";
					rep(qqq,n){
						cout << bad[qqq] << " ";
					}cout << endl;/**/
				}
//				cout << "  lose: " << lose << endl;
				if(lmax<lose){
					lmax=lose; res=cd;
				}
			}
			cout << d[res];
			if(cl<m-1) cout << " "; else cout << endl;
		}
	}
    return 0;
}

