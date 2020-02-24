#include<iostream>
#include<set>
#include<map>
#include<string>
#include<stdio.h>
#include<sstream>
#include<algorithm>
#include<sstream>
#include<queue>
#include<cmath>
#include<string.h>
using namespace std ;
double x0,Y0,x1,Y1,r0,r1 ;
double X[15],Y[15] ;
int m ;

double solve()
{
 double pi = acos(-1) ;
 double dist = sqrt((x0 - x1)*(x0 - x1) + (Y0 - Y1)*(Y0 - Y1)) ;
 if(dist >= r0 + r1 + 1e-9) return 0 ;
 
 if(dist + r0 <= r1 + 1e-9) return pi*r0*r0 ;
 if(dist + r1 <= r0 + 1e-9) return pi*r1*r1 ;
 
 double cosCBA = (r1*r1 + dist*dist - r0*r0)/(2*r1*dist) ;
 double CBA = acos(cosCBA) ;
 double CBD = 2*CBA ;
 
 double cosCAB = (r0*r0 + dist*dist - r1*r1)/(2*r0*dist) ;
 double CAB = acos(cosCAB) ;
 double CAD = 2*CAB ;

 double area = 1./2*CBD*r1*r1 - 1./2*r1*r1*sin(CBD) ;
 area       += 1./2*r0*r0*(CAD - sin(CAD)) ;
 return area ;
}

main()
{
 int i,j,n,runs ;
 
 cin >> runs ;
 for(int t=1;t<=runs;t++)
 {
  cin >> n >> m ;
  cin >> x0 >> Y0 >> x1 >> Y1 ;
  for(i=0;i<m;i++)
  {
   cin >> X[i] >> Y[i] ;
  }
  printf("Case #%d:",t) ;
  for(i=0;i<m;i++)
  {
   r0 = sqrt((x0 - X[i])*(x0 - X[i]) + (Y0 - Y[i])*(Y0 - Y[i])) ;
   r1 = sqrt((x1 - X[i])*(x1 - X[i]) + (Y1 - Y[i])*(Y1 - Y[i])) ;
   double area = solve() ;
   printf(" %.9lf ",area) ;
  }
  printf("\n") ;
 }
}
