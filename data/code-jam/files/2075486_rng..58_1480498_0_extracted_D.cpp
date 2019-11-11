#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <deque>
#include <queue>
#include <set>
#include <map>
#include <algorithm>
#include <functional>
#include <utility>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <cstdio>

using namespace std;

#define REP(i,n) for((i)=0;(i)<(int)(n);(i)++)
#define foreach(c,itr) for(__typeof((c).begin()) itr=(c).begin();itr!=(c).end();itr++)

typedef long long ll;

#define PI acos(-1.0)
struct point {int x,y;};

int N;
int x[5010],y[5010];
vector <pair <int, int> > p;
point P[5010]; // sorted (lex)
int sz,st[5010];
vector <double> dir;

int area(point P, point Q, point R){
	return ((Q.x - P.x) * (R.y - P.y) - (Q.y - P.y) * (R.x - P.x));
}

double func(double theta, ll M){ // cout << theta << endl;
	int i,j;
	vector <double> vx,vy;
	
	REP(i,4){
		double cx = cos(theta), cy = sin(theta);
		int id = 0;
		double best = -(1.0E+18);
		
		REP(j,N){
			double tmp = cx * x[j] + cy * y[j];
			if(tmp > best){
				best = tmp;
				id = j;
			}
		}
		
		vx.push_back(x[id]);
		vy.push_back(y[id]);
		theta += PI / 2.0;
	}

	REP(i,4) REP(j,i){
		swap(vx[i],vy[i]);
		vx[i] = -vx[i];
	}
	
//	cout << theta << endl;
//	REP(i,4) cout << vx[i] << ' ' << vy[i] << endl;
	
	double ans = 0.0;
	
	REP(i,4){
		double X = 0.0, Y = 0.0;
		REP(j,3) if(j+1 < M){
			X += vx[(i+j)%4];
			Y += vy[(i+j)%4];
			double D = sqrt(max(X*X + Y*Y, 0.0));
			ans = max(ans,D);
		}
		
		X = Y = 0.0;
		REP(j,4){
			ll cnt = (M + 3 - j) / 4;
			X += vx[(i+j)%4] * cnt;
			Y += vy[(i+j)%4] * cnt;
		}
		double D2 = sqrt(max(X*X + Y*Y, 0.0));
		ans = max(ans,D2);
	}
	
	return ans;
}

void add(point P, point Q){
	int dx = P.x - Q.x, dy = P.y - Q.y;
	if(dx == 0 && dy == 0) return;
	int i;
	dir.push_back(atan2((double)dy, (double)dx) + 1.0);
}

void convex_hull(void){ // N >= 2, counter-clockwise, including boundary
	int i;
	vector <int> ans;
	
	// lower
	sz = 0;
	REP(i,N) {while(sz >= 2 && area(P[st[sz-2]],P[st[sz-1]],P[i]) < 0) sz--; st[sz] = i; sz++;}
	REP(i,sz) ans.push_back(st[i]);
	
	// upper
	sz = 0;
	REP(i,N) {while(sz >= 2 && area(P[st[sz-2]],P[st[sz-1]],P[i]) > 0) sz--; st[sz] = i; sz++;}
	for(i=sz-2;i>=1;i--) ans.push_back(st[i]);
	
	int n = ans.size();
	vector <double> angles;
	REP(i,n){
		point A = P[ans[i]], B = P[ans[(i+1)%n]];
		int dx = A.x - B.x, dy = A.y - B.y;
		angles.push_back(atan2((double)dy, (double)dx));
	}
	
	REP(i,n) dir.push_back((angles[i] + angles[(i+1)%n]) / 2.0);
}

void generate_dir(void){
	dir.push_back(0.0);
	int i;
	REP(i,10000) dir.push_back(i);
	if(N == 2) add(P[0],P[1]);
	if(N >= 3) convex_hull();
}

void main2(void){
	int M,i;
	
	cin >> N >> M;
	REP(i,N) cin >> x[i] >> y[i];
	
	p.clear();
	REP(i,N) p.push_back(make_pair(x[i],y[i]));
	sort(p.begin(),p.end());
	
	REP(i,N){
		P[i].x = p[i].first;
		P[i].y = p[i].second;
	}
	
	dir.clear();
	generate_dir();
	
	double ans = 0.0;
	REP(i,dir.size()){
		double tmp = func(dir[i],M);
		ans = max(ans,tmp);
	}
	
	ans *= sqrt(2.0);
	printf("%.9f\n", ans);
}

//////////////////////////////// multiple testcases ////////////////////////////////

int main(void){
	int T,t;
	scanf("%d",&T);
	REP(t,T){
		printf("Case #%d: ",t+1);
		main2();
	}
	return 0;
}
