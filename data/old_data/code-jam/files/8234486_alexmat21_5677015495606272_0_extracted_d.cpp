// Created by alex_mat21. And it works!

#include <iostream>
#include <cstdio>
#include <algorithm>
#include <vector>
#include <queue>
#include <cstring>
#include <map>
#include <set>
#include <deque>
#include <stack>
#include <sstream>
#include <stdio.h>
#include <math.h>
#include <bitset>
#include <string> 
#include <iomanip>
#include <cmath>
#include <utility>
 
#define FOR(i,n) for(int i=0,_n=n;i<_n;i++)
#define FORR(i,s,n) for(int i=s,_n=n;i<_n;i++)
#define mp make_pair
#define vi vector<int>
#define fs first
#define sd second
#define pii pair<int,int>

using namespace std;

int r, c,num;
int a[6][6];

int neib(int ii, int jj){
	int u=0;
	if (jj>0 && a[ii][jj-1]==a[ii][jj])
		u++;
	if (jj<r-1 && a[ii][jj+1]==a[ii][jj])
		u++;
	if (a[(ii+c-1)%c][jj]==a[ii][jj])
		u++;
	if (a[(ii+1)%c][jj]==a[ii][jj])
		u++;
	return u;
	}

int rek(int ii, int jj){
	if (ii<c){
		int i2=ii;
		int j2=jj+1;
		if (j2==r){
			j2=0;
			i2++;
			}
		for (int i=1; i<=3; i++){
			a[ii][jj]=i;
			if (((ii==1 && neib(ii-1,jj)<=a[ii-1][jj] ) ||neib(ii-1,jj)==a[ii-1][jj]) && ((jj==0) || neib(ii,jj-1)<=a[ii][jj-1]) && neib(ii,jj)<=a[ii][jj]){
				rek(i2,j2);
				}
			}
		a[ii][jj]=0;
		}
	else{
		int t0=1;
		/*FOR(i,r){
			if (neib(c-1,i)!=a[c-1][i])
				t0=0;
			if (neib(0,i)!=a[0][i])
				t0=0;
			}*/
		FOR(j,r){
			FOR(i,c){
				if (neib(i,j)!=a[i][j])
					t0=0;
				}
			}
		if (t0==1){
			num++;
			/*FOR(j,r){
				FOR(i,c)
					cout << a[i][j] <<' ';
				cout << endl;
				}
			cout << endl;*/
			}
		}
	}


int main (){
int t111;
cin >> t111;
int t,t1,t0;
for (int i111=0 ; i111<t111; i111++){
	cin >> r >> c;
	num=0;
	t=1;
	FOR(i,r){
		t=t*3;
		}
	FOR(i1,t){
		memset(a,0,sizeof a);
		t1=i1;
		FOR(i,r){
			a[0][i]=(t1%3)+1;
			t1=t1/3;
			}
		t0=1;
		FOR(i,r){
			if (neib(0,i)>a[0][i])
				t0=0;
			}
		if (t0==1){
			//cout << i1 << " !!!" << endl;
			rek(1,0);
			}
		}
	
	cout << "Case #"<< i111 +1 << ": " <<num << endl;
	}
return 0;
}
