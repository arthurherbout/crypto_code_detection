//#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <string>
#include <fstream>
#include <sstream>
#include <list>
#include <algorithm>
#include <math.h>


using namespace std;
using std::string;


ifstream fin("input.txt");
ofstream fout("output.txt");


double newg, newR, newr;
double aa;

double computey(double x, double R)
{
	//cout << "sqrt: "  << sqrt(R*R - x*x) << endl;
	return sqrt(R*R - x*x);
}

double computex(double y, double R)
{
	//cout << "sqrt: "  << sqrt(R*R - x*x) << endl;
	return sqrt(R*R - y*y);
}


double computearea(double x1, double x2, double y1, double y2, double R)
{
	double angle;
	double sinangle;
	double sa, sb, sc;
	double tempy1, tempy2;
	double tempx1, tempx2;
	
	if (x2*x2+y2*y2 <= R*R) {	// whole in
		return aa;
	}
	else {	// partly in
		tempy2 = computey(x1,R);
		tempx2 = computex(y1,R);
		if (tempy2 > y2 && tempx2 > x2) {
			sa = y2-computey(x2,R);
			sb = x2-computex(y2,R);
			sc = sqrt(sa*sa+sb*sb);
			sinangle = sc/2/newR;
			//cout << "sinangle1: " << sinangle << endl;
			return aa + (newR*newR*asin(sinangle) - newR*sqrt(1-sinangle*sinangle) * sc/2) - sa*sb/2;
		}
		if (tempy2 < y2 && tempx2 > x2){
			sa = y2-tempy2;
			sb = y2-computey(x2,R);
			sc = sqrt((sa-sb)*(sa-sb)+newg*newg);
			sinangle = sc/2/newR;
			//cout << "sinangle2: " << sinangle << endl;
			return aa - (sa+sb)*newg/2 + (newR*newR*asin(sinangle) - newR*sqrt(1-sinangle*sinangle) * sc/2);
			
		}
		if (tempy2 < y2 && tempx2 < x2){
			sa = tempy2-y1;
			sb = tempx2-x1;
			sc = sqrt(sa*sa+sb*sb);
			sinangle = sc/2/newR;
			//cout << "sinangle3: " << sinangle << endl;
			//cout << "sa*sb/2: " << sa*sb/2 << endl;
			//cout << "sa: " << sa << endl;
			//cout << "sb: " << sb << endl;
			return sa*sb/2 + (newR*newR*asin(sinangle) - newR*sqrt(1-sinangle*sinangle) * sc/2);
		}
	}
}

void solve(int round)
{
	string s;
	
	int i;
	double f, R, t, r, g;

	double x1, x2, y1, y2;

	double incc;
	
	fin >> f >> R >> t >> r >> g;
		
	//cout << f << R << t << r << g << endl;
	
	double ar, af;	// areas
	double P;
	
	newg = g-2*f;
	newR = R-t-f;
	newr = r+f;
	double sqrt2=sqrt(2);
	
	if (newg < 0 || newR < sqrt2*newr)
		P = 1;
	else {
		af = 0;
		aa = newg * newg;
		ar = M_PI * R * R;
		
		//cout << "newg:" << newg << "\nnewR:" << newR << "\nnewr: " << newr << endl;
		
		incc = newg+2*newr;
		
		x1 = newr;
		while (x1 < newR/sqrt2) {
			x2 = x1 + newg;
			
			y1 = x1;
			y2 = x2;
			
			bool flag = 0;
			while (y1 < computey(x1, newR)) {
				if (flag == 0)
					af += computearea(x1, x2, y1, y2, newR)/2;
				else
					af += computearea(x1, x2, y1, y2, newR);
				flag = 1;
				//cout << x1 << " " << x2 << " " << y1 << " " << y2 << endl;
				y1 += incc;
				y2 += incc;
			}
			x1 += incc;
		}

		P = 1 - af*8 / ar;
	}
	
	fout.precision(6);
	fout << "Case #" << round << ": " << fixed << P << endl;
}

int main()
{
	int i;
	int N;
	int cnt;
	
	fin >> N;
	
	for(int i = 0; i < N; i++) {
		solve(i+1);
	}
	
	return 0;
}
