// Created by alex_mat21. And it works!

#include <iostream>
#include <fstream>
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
#include <ctime>
#include <cstdlib>

#define FOR(i,n) for(int i=0,_n=n;i<_n;i++)
#define FORR(i,s,n) for(int i=s,_n=n;i<_n;i++)

using namespace std;

ifstream in;
ofstream out;

const int MAX_INT = 10000000;

int hd, hk, ad,ak,b,d;

int x[101][101][101][101];
int y[101][101][101][101];

int rek(int hd1, int hk1, int ad1, int ak1){
	//cout << hd1 << ' ' << hk1 << ' ' << ad1 << ' ' << ak1 << endl;
	if (y[hd1][hk1][ad1][ak1]==2)
		return x[hd1][hk1][ad1][ak1];
	if (y[hd1][hk1][ad1][ak1]==1)
		return -1;
	y[hd1][hk1][ad1][ak1]=1;
	int m = MAX_INT;
	int m1;
	if (hk1<=ad1){
		x[hd1][hk1][ad1][ak1]=1;
		y[hd1][hk1][ad1][ak1]=2;
		return 1;
		}
	if (hd1>ak1){
		m1=rek(hd1-ak1,hk1-ad1,ad1,ak1);
		if (m1>-1)
			m=min(m,m1);
	}
	if (hd>ak1){
		m1=rek(hd-ak1,hk1,ad1,ak1);
		if (m1>-1)
			m=min(m,m1);
	}
	if (hd1>ak1){
		m1=rek(hd1-ak1,hk1,min(ad1+b,hk),ak1);
		if (m1>-1)
			m=min(m,m1);
	}
	if (hd1>ak1-d){
		int u = max(ak1-d,0);
		m1=rek(hd1-u,hk1,ad1,u);
		if (m1>-1)
			m=min(m,m1);
	}
	y[hd1][hk1][ad1][ak1]=2;
	if (m==MAX_INT)
		x[hd1][hk1][ad1][ak1]=-1;
	else
		x[hd1][hk1][ad1][ak1]=m+1;
	return x[hd1][hk1][ad1][ak1];
}

void main_solve() {
	in >> hd >> ad >> hk >> ak >> b >> d;
	memset(x,0,sizeof x);
	memset(y,0,sizeof y);
	FOR(i,hd+1){
		FOR(j,hk+1){
			FOR(k,ak+1){
				y[i][0][j][k]=2;
			}
		}
	}
	rek(hd,hk,ad,ak);
	
	if (x[hd][hk][ad][ak]>0)
		out << x[hd][hk][ad][ak] << endl;
	else
		out << "IMPOSSIBLE" << endl;
}

int main() {
	in.open("input.in");
	out.open("output.out");
	clock_t start;
	int t;
	in >> t;
	for (int i=0 ; i<t; i++) {
		cout << "Starting case: " << i+1 << endl;
		out << "Case #"<< i+1 << ": ";
		start = clock();
		main_solve();
		cout << "Time: " << (clock()-start)/(double)CLOCKS_PER_SEC << "s" << endl;
	}
	in.close();
	out.close();
	return 0;
}
