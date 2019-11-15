#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <deque>
#include <queue>
#include <stack>
#include <set>
#include <map>
#include <algorithm>
#include <functional>
#include <utility>
#include <bitset>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <cstdio>

using namespace std;

#define REP(i,n) for((i)=0;(i)<(int)(n);(i)++)
#define snuke(c,itr) for(__typeof((c).begin()) itr=(c).begin();itr!=(c).end();itr++)

struct point {int x,y;};

int N;
point P[15];
int p[15];
int q[15];

int area(point P, point Q, point R){
	return ((Q.x - P.x) * (R.y - P.y) - (Q.y - P.y) * (R.x - P.x));
}

int sign(int x){
	if(x > 0) return 1;
	if(x < 0) return -1;
	return 0;
}

bool ssintersect(point p1, point p2, point p3, point p4){
	if(sign(area(p1,p2,p3)) * sign(area(p1,p2,p4)) == 1) return false;
	if(sign(area(p3,p4,p1)) * sign(area(p3,p4,p2)) == 1) return false;
	if(max(p1.x,p2.x) < min(p3.x,p4.x)) return false;
	if(max(p1.y,p2.y) < min(p3.y,p4.y)) return false;
	if(max(p3.x,p4.x) < min(p1.x,p2.x)) return false;
	if(max(p3.y,p4.y) < min(p1.y,p2.y)) return false;
	return true;
}

bool check(void){
	int i,j;
	
	REP(i,N) REP(j,N){
		if(i == j || i == (j+1)%N || j == (i+1)%N) continue;
		if(ssintersect(P[p[i]], P[p[(i+1)%N]], P[p[j]], P[p[(j+1)%N]])){
		//	cout << i << ' ' << j << endl;
			return false;
		}
	}
	
	return true;
}

double area(void){
	double ans = 0.0;
	int i;
	for(i=1;i<=N-2;i++) ans += area(P[p[0]], P[p[i]], P[p[i+1]]);
	if(ans < 0.0) ans = -ans;
	return ans;
}

void main2(void){
	int i;
	
	cin >> N;
	REP(i,N) cin >> P[i].x >> P[i].y;
	
//	cout << ssintersect(P[0], P[2], P[1], P[3]) << endl;
	
	double maxarea = -1.0;
	REP(i,N) p[i] = i;
	
	do {
	//	REP(i,N) cout << p[i] << ' ';
	//	cout << endl;
		if(check()){
		//	cout << "A" << endl;
			double tmp = area();
		//	cout << tmp << endl;
			if(tmp > maxarea){
				maxarea = tmp;
				REP(i,N) q[i] = p[i];
			}
		}
	} while(next_permutation(p+1, p+N));
	
	REP(i,N) cout << ' ' << q[i];
	cout << endl;
}

int main(void){
	int TC,tc;
	cin >> TC;
	REP(tc,TC){
		printf("Case #%d:", tc+1);
		main2();
	}
	return 0;
}
