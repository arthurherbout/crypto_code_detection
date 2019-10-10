// another fine solution by misof
// #includes {{{
#include <algorithm>
#include <numeric>

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <queue>
#include <set>
#include <map>

#include <cstdio>
#include <cstdlib>
#include <cctype>
#include <cassert>
#include <cstring>

#include <cmath>
#include <complex>
using namespace std;
// }}}

/////////////////// PRE-WRITTEN CODE FOLLOWS, LOOK DOWN FOR THE SOLUTION ////////////////////////////////

// pre-written code {{{
#define REP(i,n) for(int i=0;i<(int)(n);++i)
// }}}

/////////////////// CODE WRITTEN DURING THE COMPETITION FOLLOWS ////////////////////////////////

int T; string Tline;

double x1, yy1, x2, y2, x3, y3, x4, y4, x5, y5, x6, y6;

int main() {
  getline(cin,Tline); stringstream(Tline) >> T;
  for (int t=1; t<=T; t++) {
    fprintf(stderr,"Case #%d\n",t);
    cin >> x1 >> yy1 >> x2 >> y2 >> x3 >> y3 >> x4 >> y4 >> x5 >> y5 >> x6 >> y6;
    double ux=x2-x1, uy=y2-yy1, vx=x3-x1, vy=y3-yy1, sx=x5-x4, sy=y5-y4, tx=x6-x4, ty=y6-y4;
    double VP = ux*vy - vx*uy;

    double X = (x4+x5+x6)/3, Y = (y4+y5+y6)/3;
    
    REP(loop,1000000) {
      double oldX=X, oldY=Y;
      X -= x1; Y -= yy1;
      double p = (X*vy - Y*vx) / (ux*vy - vx*uy);
      double q = (X*uy - Y*ux) / (vx*uy - ux*vy);
      X = x4 + p*sx + q*tx;
      Y = y4 + p*sy + q*ty;
      //printf(" meantime %.15f %.15f\n",X,Y);
      //if (oldX==X && oldY==Y) { fprintf(stderr,"done in %d\n",loop); break; }
    }

    double a=0,b=0,c=0,d=0,e=0,f=0;
    
    a -= 1;

    c += x4;

    a += sx*vy/VP;
    c -= sx*vy*x1/VP;
    b -= sx*vx/VP;
    c += sx*vx*yy1/VP;

    a -= tx*uy/VP;
    c += tx*uy*x1/VP;
    b += tx*ux/VP;
    c -= tx*ux*yy1/VP;
    
    e -= 1;

    f += y4;

    d += sy*vy/VP;
    f -= sy*vy*x1/VP;
    e -= sy*vx/VP;
    f += sy*vx*yy1/VP;

    d -= ty*uy/VP;
    f += ty*uy*x1/VP;
    e += ty*ux/VP;
    f -= ty*ux*yy1/VP;

    //fprintf(stderr,"test %.15f\n",a*X+b*Y+c);
    //fprintf(stderr,"test %.15f\n",d*X+e*Y+f);

    if (abs(b*d - a*e) < 1e-9) {
      fprintf(stderr,"BACHA!!!\n");
    } else {
      Y = - (c*d - a*f) / (b*d - a*e);
      X = - (c*e - b*f) / (a*e - b*d);
    }


    printf("Case #%d: %.15f %.15f\n",t,X,Y);
  }
  return 0;
}
// vim: fdm=marker:commentstring=\ \"\ %s:nowrap:autoread
