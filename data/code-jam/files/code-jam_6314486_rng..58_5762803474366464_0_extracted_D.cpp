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

#define INF 1.0E+18
#define eps 1.0E-9
	
int N;
double x[5010],y[5010],z[5010];

int L,H;
double la[5010],lb[5010]; // y >= x * la + lb
double ha[5010],hb[5010]; // y <= x * ha + hb

double f(double X){
	double low = -INF * INF, high = INF * INF;
	int i;
	
	REP(i,L){
		low = max(low, la[i] * X + lb[i]);
		high = min(high, ha[i] * X + hb[i]);
	}
	
	return high - low;
}

double check(void){
	double low = -INF, high = INF;
	int iter;
	
	REP(iter,5000){
		double l = (low * 2.0 + high) / 3.0;
		double r = (low + 2.0 * high) / 3.0;
		double fl = f(l);
		double fr = f(r);
		
		if(fl < fr){
			low = l;
		} else {
			high = r;
		}
	}
	
	return f(low);
}
	
void main2(void){
	int i,j;
	
	cin >> N;
	REP(i,N) cin >> x[i] >> y[i] >> z[i];
	
	REP(i,N){
		double d = x[i] * x[i] + y[i] * y[i] + z[i] * z[i];
		d = sqrt(d);
		x[i] /= d;
		y[i] /= d;
		z[i] /= d;
	}
	
	L = H = 0;
	
	REP(i,N){
		double xx = x[i] * cos(1.0) - y[i] * sin(1.0);
		double yy = x[i] * sin(1.0) + y[i] * cos(1.0);
		x[i] = xx;
		y[i] = yy;
		
		xx = x[i] * cos(2.0) - z[i] * sin(2.0);
		double zz = x[i] * sin(2.0) + z[i] * cos(2.0);
		x[i] = xx;
		z[i] = zz;
		
		yy = y[i] * cos(3.0) - z[i] * sin(3.0);
		zz = y[i] * sin(3.0) + z[i] * cos(3.0);
		y[i] = yy;
		z[i] = zz;
		
		double a = x[i] / z[i];
		double b = y[i] / z[i];
		
		if(z[i] > 0.0){
			// ax + by >= -1
			if(b > 0.0){
				// by >= -ax - 1
				la[L] = -a / b;
				lb[L] = -1 / b;
				L++;
			} else {
				// -by <= ax + 1
				ha[H] = a / (-b);
				hb[H] = 1 / (-b);
				H++;
			}
		} else {
			// ax + by <= -1
			if(b > 0.0){
				// by <= -ax - 1
				ha[H] = (-a) / b;
				hb[H] = (-1) / b;
				H++;
			} else {
				// -by >= ax + 1
				la[L] = a / (-b);
				lb[L] = 1 / (-b);
				L++;
			}
		}
	}
	
	double tmp = check();
	cout << ((tmp > eps) ? "NO" : "YES") << endl;
}

int main(void){
	int TC,tc;
	cin >> TC;
	REP(tc,TC){
		printf("Case #%d: ", tc+1);
		main2();
	}
	return 0;
}
