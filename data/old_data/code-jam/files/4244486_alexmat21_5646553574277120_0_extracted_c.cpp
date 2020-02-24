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

int main (){
int t111;
cin >> t111;
int c,d,v;
int a[100];
int b[100];
for (int i111=0 ; i111<t111; i111++){
	cin >> c >> d >> v;
	FOR(i,d)
		cin >> a[i];
	int t=0;
	memset(b,0,sizeof b);
	b[0]=1;
	FOR(i,d){
		for (int j=v; j>=0; j--){
			if (b[j]==1 && j+a[i]<=v){
				b[j+a[i]]=1;
				//cout << j+a[i] << "  !!"<< endl;
				}
			}
		}
	for (int i=1; i<=v; i++){
		if (b[i]==0){
			t++;
			for (int j=v; j>=0; j--){
				if (b[j]==1 && j+i<=v){
					b[j+i]=1;
					}
				}
			}
		}
	
	cout << "Case #"<< i111 +1 << ": " << t<< endl;
	}
return 0;
}
