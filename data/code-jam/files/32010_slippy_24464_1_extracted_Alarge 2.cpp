#include <vector>
#include <algorithm>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <cstdio>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <deque>
#include <set>

using namespace std;

const double EPS = 1e-10;
const double PI = 3.14159265358979323846264338328;

#define PB push_back
#define FOR(a,start,end) for(int a = int(start); a < int(end); a++)
#define INF INT_MAX
#define MSG(a) cout << #a << " = " << a << endl;
#define SORT(a) sort(a.begin(),a.end())


bool lineIntersect( double x[], double y[], double r[] )
{
    double n[2]; n[0] = y[3] - y[2]; n[1] = x[2] - x[3];
    double denom = n[0] * ( x[1] - x[0] ) + n[1] * ( y[1] - y[0] );
    if( fabs( denom ) < EPS ) return false;
    double num = n[0] * ( x[0] - x[2] ) + n[1] * ( y[0] - y[2] );
    double t = -num / denom;
    r[0] = x[0] + t * ( x[1] - x[0] );
    r[1] = y[0] + t * ( y[1] - y[0] );
    return true;
}


bool lineSegIntersect(double x0, double x1, double x2, double x3, double y0, double y1, double y2, double y3)
{
	double t1 = (y3-y0)*(x1-x0) - (x3-x0)*(y1-y0);
	double t2 = (y2-y0)*(x1-x0) - (x2-x0)*(y1-y0);
	double t3 = (y1-y2)*(x3-x2) - (x1-x2)*(y3-y2);
	double t4 = (y0-y2)*(x3-x2) - (x0-x2)*(y3-y2);

	if(t1*t2 > 0 || t3*t4 > 0)  return 0;
	if(t1 != 0 || t2 != 0 || t3 != 0 || t4 != 0)  return 1;

	return ( max(y1,y0) >= min(y3,y2) && max(x1,x0) >= min(x3,x2) && max(y3,y2) >= min(y1,y0) && max(x3,x2) >= min(x1,x0) );
}




int main()
{
   ofstream fout;
   fout.open("outputAlarge.txt");
//   ifstream fin("input.txt");
 ifstream fin("A-large.in");

    
   int N;
   fin >> N;
   
   FOR(t,1,N+1)
   {
	//	fout << "BREAK" << endl;
		
		int x1,y1,x2,y2,x3,y3,x1p,x2p,x3p,y1p,y2p,y3p;
		fin >> x1 >> y1 >> x2 >> y2 >> x3 >> y3;
		fin >> x1p >> y1p >> x2p >> y2p >> x3p >> y3p;

	double V[6][7];
	FOR(k,0,6)
	FOR(l,0,7)
		V[k][l] = 0;
	V[0][0] = x1;
	V[0][1] = y1;
	V[0][2] = 1;
	V[1][3] = x1;
	V[1][4] = y1;
	V[1][5] = 1;
	V[2][0] = x2;
	V[2][1] = y2;
	V[2][2] = 1;
	V[3][3] = x2;
	V[3][4] = y2;
	V[3][5] = 1;
	V[4][0] = x3;
	V[4][1] = y3;
	V[4][2] = 1;
	V[5][3] = x3;
	V[5][4] = y3;
	V[5][5] = 1;	
	V[0][6] = x1p;
	V[1][6] = y1p;
	V[2][6] = x2p;
	V[3][6] = y2p;
	V[4][6] = x3p;
	V[5][6] = y3p;
		
	int N = 6;
	
	FOR(k,0,N)
	{
		/*
	FOR(t,0,6)
	{
		FOR(u,0,7)
			fout << V[t][u] << " ";
		fout << endl;
	}
	fout << endl;
*/
		if(fabs(V[k][k]) < EPS)
		{
			int done = 0;
			FOR(j,k+1,N)
				if(fabs(V[j][k]) > EPS)
				{
					FOR(p,0,7)
						swap(V[j][p],V[k][p]);
					done = 1;
					break;
				}
			if(done == 0)
				continue;
		}
		
		double m = V[k][k];
		FOR(u,k,N+1)
			V[k][u] /= m;
		
		FOR(j,k+1,N)
		{
			double mult = -V[j][k];
			FOR(u,k,N+1)
				V[j][u] += mult*V[k][u];
		}
		
	}
	

	
	vector<double> found(6,0);
	found[5] = V[5][6];
	
	for(int k = N-2; k >= 0; k--)
	{
		if(fabs(V[k][k]) < EPS)
		{
			found[k] = 1E100;  //found[k] can be anything
			continue;
		}
	
		double a = V[k][6];
		FOR(l,k+1,N)
			a -= V[k][l]*found[l];
		found[k] = a;
	}
/*
	FOR(k,0,6)
		fout << found[k] << " ";
		fout << endl;
*/

		double A=found[0],B=found[1],C=found[2],D=found[3],E=found[4],F=found[5];
		double det = B*D-(A-1)*(E-1);
		
		if(fabs(det) < EPS && fabs(C*D - F*(A-1)) > EPS)
		{
			fout << "Case #" << t << ": " << "No Solution" << endl;
			continue;
		}
		else if(fabs(det) < EPS && fabs(C*D-F*(A-1)) < EPS)
		{
			//infinitely many solns
			//check vertices
			
			if(fabs((A-1)*x1p + B*y1p + C) < EPS)
			{
				char c[100];
				sprintf(c,"%.7lf %.7lf",x1p,y1p);
				string ss(c);
				fout << "Case #" << t << ": " << ss << endl;
				continue;
			}
			if(fabs((A-1)*x2p + B*y2p + C) < EPS)
			{
				char c[100];
				sprintf(c,"%.7lf %.7lf",x2p,y2p);
				string ss(c);
				fout << "Case #" << t << ": " << ss << endl;
				continue;
			}				 
			if(fabs((A-1)*x3p + B*y3p + C) < EPS)
			{
				char c[100];
				sprintf(c,"%.7lf %.7lf",x3p,y3p);
				string ss(c);
				fout << "Case #" << t << ": " << ss << endl;
				continue;
			}
			
			double endx1,endx2,endy1,endy2;
			if(fabs(A-1) < EPS)
			{
				endy1 = -C/B;
				endy2 = -C/B;
				endx1 = -20000;
				endx2 = 20000;
			}
			else
			{
				endy1 = -20000;
				endy2 = 20000;
				endx1 = -C-B*endy1;
				endx1 /= (A-1);
				endx2 = -C-B*endy2;
				endx2 /= (A-1);
			}	
			
			vector<double> xx;
			vector<double> yy;
			
			if(lineSegIntersect(endx1,endx2,x1p,x2p,endy1,endy2,y1p,y2p) != 0)
			{
				double xxx[] = {endx1,endx2,x1p,x2p};
				double yyy[] = {endy1,endy2,y1p,y2p};
				double rrr[2];
				bool tm = lineIntersect( xxx,yyy,rrr);
							
				xx.PB(rrr[0]);
				yy.PB(rrr[1]);
			}
			if(lineSegIntersect(endx1,endx2,x1p,x3p,endy1,endy2,y1p,y3p) != 0)
			{
				double xxx[] = {endx1,endx2,x1p,x3p};
				double yyy[] = {endy1,endy2,y1p,y3p};
				double rrr[2];
				bool tm = lineIntersect( xxx,yyy,rrr);
							
				xx.PB(rrr[0]);
				yy.PB(rrr[1]);
			}			
			if(lineSegIntersect(endx1,endx2,x3p,x2p,endy1,endy2,y3p,y2p) != 0)
			{
				double xxx[] = {endx1,endx2,x3p,x2p};
				double yyy[] = {endy1,endy2,y3p,y2p};
				double rrr[2];
				bool tm = lineIntersect( xxx,yyy,rrr);
							
				xx.PB(rrr[0]);
				yy.PB(rrr[1]);
			}						
			
			if(xx.size() < 2)
			{
				fout << "Case #" << t << ": " << "No Solution" << endl;
				continue;
			}			
			else
			{
				double finalx = (xx[0]+xx[1])/2;
				double finaly = (yy[0]+yy[1])/2;
				char c[100];
				sprintf(c,"%.7lf %.7lf",finalx,finaly);
				string ss(c);
				fout << "Case #" << t << ": " << ss << endl;
				continue;
			}
		}
		else
		{
			double y = F*(A-1)-C*D;
			y /= det;
			double x;
			if(fabs(A-1) < EPS)
			{
				x = -F-(E-1)*y;
				x /= D;
			}
			else
			{
				x = -C-B*y;
				x /= (A-1);
			}
			
			char c[100];
			sprintf(c,"%.7lf %.7lf",x,y);
			string ss(c);
			fout << "Case #" << t << ": " << ss << endl;
		}

			
      
   }
   
   return 0; 
    
    
    
    
    
    
}

