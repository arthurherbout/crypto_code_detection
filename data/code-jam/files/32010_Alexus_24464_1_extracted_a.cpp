#define _CRT_SECURE_NO_DEPRECATE
#include <cstdio>
#include <algorithm>
#include <string>
#include <cstring>
#include <cassert>
#include <cstdlib>
#include <set>
#include <cmath>
#include <map>
#include <vector>
#include <iostream>

using namespace std;

#define PROBLEM "a"

const int ITER = 3000000;
const double EPS = 1e-9;

struct point{
  double x, y;
  point(){
    x=y=0;
  }
  point(double xx, double yy){
    x = xx;
    y = yy;
  }
  point operator +(const point& other){
    return point(x + other.x, y + other.y);
  }
  point operator -(const point& other){
    return point(x - other.x, y - other.y);
  }
  double len(){
    return sqrt(x*x + y*y);
  }
};

point tria[2][3];
double coef, phi;

void count_phi(){
  point s1 = tria[1][1] - tria[1][0];
  double spv = tria[0][1].x * (s1.x) + tria[0][1].y * (s1.y);
  double deg1 = spv / tria[0][1].len() / s1.len();
  if (deg1 < -1+EPS) deg1 = -1;
  if (deg1 > 1-EPS) deg1 = 1;
  deg1 = acos(deg1);
  double vpv = tria[0][1].x * s1.y - tria[0][1].y * s1.x;
  if (vpv < 0) deg1 = -deg1;
  phi = deg1;
}

void turn(point& s){
  double r = s.len();
  double deg = atan2(s.y, s.x);
  deg += phi;
  s = point(r*cos(deg), r*sin(deg));
}

void perform(point& s){
  point res = s;
  res.x *= coef;
  res.y *= coef;
  turn(res);
  res = res + tria[1][0];
  s = res;
}

int main(){
  freopen(PROBLEM".in", "r", stdin);
  freopen(PROBLEM".out", "w", stdout);
  int tn, tst;
  scanf("%d", &tn);
  for (tst=1; tst<=tn; tst++){
    printf("Case #%d: ", tst);
    int z, i;
    for (z=0; z<2; z++){
      for (i=0; i<3; i++){
        scanf("%lf%lf", &tria[z][i].x, &tria[z][i].y);
        if ((z>0) || (i>0)) tria[z][i] = tria[z][i] - tria[0][0];
      }
    }
    point s(0, 0);
    if ((tria[0][1].len() < EPS) && (tria[0][2].len() < EPS)){
      s=s;
    }
    else if ((tria[0][1].len() < EPS) || (tria[0][2].len() < EPS)){
      point v1, v2;
      if (tria[0][1].len() < EPS){
        v1 = tria[0][2];
        v2 = tria[1][2] - tria[1][0];
      }
      else{
        v1 = tria[0][1];
        v2 = tria[1][1] - tria[1][0];
      }
      coef = v2.len() / v1.len();
      for (i=0; i<ITER; i++){
        s.x *= coef;
        s.y *= coef;
        s = s + tria[1][0];
      }
    }
    else{
      coef = (tria[1][1] - tria[1][0]).len() / tria[0][1].len();
      count_phi();
      for (i=0; i<ITER; i++){
        perform(s);
      }
    }
    printf("%.10lf %.10lf\n", s.x + tria[0][0].x, s.y + tria[0][0].y);
  }
  return 0;  
}