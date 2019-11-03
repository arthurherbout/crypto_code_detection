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

int r,c,d;
char m[510][510];
ll t[510][510],t1[510][510];
ll y[510][510];

ll get(int i,int j){
	if(i>=0 && j>=0) return t[i][j];
	else return 0;
}
ll get1(int i,int j){
	if(i>=0 && j>=0) return t1[i][j];
	else return 0;
}
ll gey(int i,int j){
	if(i>=0 && j>=0) return y[i][j];
	else return 0;
}

int main(){
	int ts = in();
	rep(cas,ts){
		r = in(); c = in(); d = in();
		rep(i,510)rep(j,510)t[i][j]=t1[i][j]=y[i][j]=0;
		rep(i,r){
			scanf("%s",m[i]);
			rep(j,c) m[i][j] -= '0';
		}
		rep(i,r){
			rep(j,c){
				t[i][j] = get(i-1,j) + get(i,j-1) - get(i-1,j-1) + m[i][j]*i;
				t1[i][j] = get1(i-1,j) + get1(i,j-1) - get1(i-1,j-1) + m[i][j];
				y[i][j] = gey(i-1,j) + gey(i,j-1) - gey(i-1,j-1) + m[i][j]*j;
			}
		}
		cout << "Case #" << cas+1 << ": ";
		for(int sz=min(r,c); sz>2; --sz){
			rep(i,r-sz+1){
				rep(j,c-sz+1){
					ll tt = get(i+sz-1,j+sz-1)-get(i+sz-1,j-1)-get(i-1,j+sz-1)+get(i-1,j-1);
					ll yy = gey(i+sz-1,j+sz-1)-gey(i+sz-1,j-1)-gey(i-1,j+sz-1)+gey(i-1,j-1);
					tt -= m[i][j]*i; tt -= m[i][j+sz-1]*i;
					tt -= m[i+sz-1][j]*(i+sz-1); tt -= m[i+sz-1][j+sz-1]*(i+sz-1);
					yy -= m[i][j]*j; yy -= m[i][j+sz-1]*(j+sz-1);
					yy -= m[i+sz-1][j]*j; yy -= m[i+sz-1][j+sz-1]*(j+sz-1);
					ll mm = get1(i+sz-1,j+sz-1)-get1(i+sz-1,j-1)-get1(i-1,j+sz-1)+get1(i-1,j-1);
					mm -= m[i][j]; mm -= m[i][j+sz-1];
					mm -= m[i+sz-1][j]; mm -= m[i+sz-1][j+sz-1];
					if(2*tt == (2*i+sz-1)*mm)if(2*yy == (2*j+sz-1)*mm){
						cout << sz << endl;
						goto next;
					}
				}
			}
		}
		cout <<"IMPOSSIBLE"<<endl;
		next:;
	}
    return 0;
}

