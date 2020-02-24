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
#include <stack>

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cctype>
#include <cassert>

#include <cmath>
#include <complex>
using namespace std;
// }}}

/////////////////// PRE-WRITTEN CODE FOLLOWS, LOOK DOWN FOR THE SOLUTION ////////////////////////////////

// pre-written code {{{
#define FOR(i,a,b) for(int i=(int)(a);i<=(int)(b);++i)
// }}}

/////////////////// CODE WRITTEN DURING THE COMPETITION FOLLOWS ////////////////////////////////

#define COORD_TYPE double // data type used to store the coordinates: %d || %lld || %lf || %Lf
typedef complex<COORD_TYPE> point;
typedef vector<point> poly; // if used to store a polygon, don't repeat the first vertex 

#define EPSILON (1e-9) // epsilon used for computations involving doubles ; dec to 1e-9 for %Lf 

// safe comparison with 0: is_zero, is_negative, is_positive, signum {{{
bool is_negative(int x) { return x<0; } bool is_zero(int x) { return x==0; } bool is_positive(int x) { return x>0; }
bool is_negative(long long x) { return x<0; } bool is_zero(long long x) { return x==0; } bool is_positive(long long x) { return x>0; }
bool is_negative(double x) { return x < -EPSILON; } bool is_zero(double x) { return abs(x) <= EPSILON; } bool is_positive(double x) { return x > EPSILON; }
bool is_negative(long double x) { return x < -EPSILON; } bool is_zero(long double x) { return abs(x) <= EPSILON; } bool is_positive(long double x) { return x > EPSILON; }
template<class T> int signum(const T &A) { if (is_zero(A)) return 0; if (is_negative(A)) return -1; return 1; }
// }}}

// safe equality test for points {{{ 
template<class T> bool are_equal(const complex<T> &A, const complex<T> &B) { return is_zero(real(B)-real(A)) && is_zero(imag(B)-imag(A)); }
// }}}

// cross-product, dot_product, square_size (=dot_product(A,A)) for 2D vectors {{{
template<class T> T square_size(const complex<T> &A) { return real(A) * real(A) + imag(A) * imag(A); }
template<class T> T dot_product(const complex<T> &A, const complex<T> &B) { return real(A)*real(B) + imag(A)*imag(B); }
template<class T> T cross_product(const complex<T> &A, const complex<T> &B) { return real(A) * imag(B) - real(B) * imag(A); }
// }}}

// safe colinearity and orientation tests: colinear, clockwise, counterclockwise {{{
template<class T> bool colinear(const complex<T> &A, const complex<T> &B, const complex<T> &C) { return is_zero( cross_product( B-A, C-A )); }
template<class T> bool colinear(const complex<T> &B, const complex<T> &C) { return is_zero( cross_product( B, C )); }

template<class T> bool clockwise(const complex<T> &A, const complex<T> &B, const complex<T> &C) { return is_negative( cross_product( B-A, C-A )); }
template<class T> bool clockwise(const complex<T> &B, const complex<T> &C) { return is_negative( cross_product( B, C )); }

template<class T> bool counterclockwise(const complex<T> &A, const complex<T> &B, const complex<T> &C) { return is_positive( cross_product( B-A, C-A )); }
template<class T> bool counterclockwise(const complex<T> &B, const complex<T> &C) { return is_positive( cross_product( B, C )); }
// }}}

// size, normalize for 2D real vectors {{{
template<class T> T size(const complex<T> &A) { return sqrt(real(A) * real(A) + imag(A) * imag(A)); }
template<class T> void normalize(complex<T> &A) { T Asize = size(A); A *= (1/Asize); }
// }}}
// safe comparison function according to [argument,size]: compare_arg {{{
template<class T> bool compare_arg(const complex<T> &A, const complex<T> &B) { 
  // [0,0] ide uplne na zaciatok
  if (are_equal(B,point(0,0))) return 0;
  if (are_equal(A,point(0,0))) return 1;
  // chceme poradie: pod osou x zlava doprava, kladna poloos, nad osou sprava dolava, zaporna poloos
  int sgnA = signum(imag(A));
  int sgnB = signum(imag(B));
  if (sgnA == 0) if (signum(real(A))<0) sgnA = 2;
  if (sgnB == 0) if (signum(real(B))<0) sgnB = 2;
  if (sgnA != sgnB) return (sgnA < sgnB);
  // v ramci polroviny sortime podla clockwise
  if (counterclockwise(A,B)) return 1;
  if (clockwise(A,B)) return 0;
  // a ak sa este nerozhodlo, podla vzdialenosti, blizsie skor
  return (square_size(A) < square_size(B));
}
// }}}
// safe comparison functions acc. to [x,y] and [y,x]: compare_XY, compare_YX {{{
template<class T> bool compare_XY(const complex<T> &A, const complex<T> &B) { if (!is_zero(real(A)-real(B))) return (is_negative(real(A)-real(B))); return (is_negative(imag(A)-imag(B))); }
template<class T> bool compare_YX(const complex<T> &A, const complex<T> &B) { if (!is_zero(imag(A)-imag(B))) return (is_negative(imag(A)-imag(B))); return (is_negative(real(A)-real(B))); }
// }}}
#ifdef LESS_THAN_COMPARES_LEXICOGRAPHICALLY
// bool operator < such that   (a+bi) < (c+di)  <==> [a,b] < [c,d] {{{
template<class T> bool operator < (const complex<T> &A, const complex<T> &B) { if (real(A) != real(B)) return (real(A) < real(B)); return (imag(A) < imag(B)); }

template<class T> bool operator < (const T &A, const complex<T> &B) { return complex<T>(A) < B; }
template<class T> bool operator < (const complex<T> &A, const T &B) { return A < complex<T>(B); }

template<class T> bool operator > (const complex<T> &A, const complex<T> &B) { return B < A; }
template<class T> bool operator > (const T &A, const complex<T> &B) { return B < A; }
template<class T> bool operator > (const complex<T> &A, const T &B) { return B < A; } 
// }}}
#endif

// polygon area: twice_signed_poly_area, poly_area {{{
template<class T> T twice_signed_poly_area(const vector< complex<T> > &V) { T res = 0; for (unsigned int i=0; i<V.size(); i++) res += cross_product( V[i], V[(i+1)%V.size()] ); return res; }
template<class T> T poly_area(const vector< complex<T> > &V) { return abs(0.5 * twice_signed_poly_area(V)); }
// }}}
// compute a CCW convex hull with no unnecessary points: convex_hull {{{
template<class T> vector< complex<T> > convex_hull( vector< complex<T> > V ) { 
  // handle boundary cases
  if (V.size() == 2) if (are_equal(V[0],V[1])) V.pop_back();
  if (V.size() <= 2) return V;
  // find the bottommost point -- this can be optimized!
  sort(V.begin(), V.end(), compare_YX<COORD_TYPE> );

  complex<T> offset = V[0];
  for (unsigned int i=0; i<V.size(); i++) V[i] -= offset;
  sort(V.begin()+1, V.end(), compare_arg<COORD_TYPE> );

  int count = 2;
  vector<int> hull(V.size()+2);
  for (unsigned int i=0; i<2; i++) hull[i]=i;
 
  for (unsigned int i=2; i<V.size(); i++) {
    while (count>=2 && !counterclockwise( V[hull[count-2]], V[hull[count-1]], V[i] ) ) count--;
    hull[count++]=i;
  }
  
  vector< complex<T> > res;
  for (int i=0; i<count; i++) res.push_back( V[hull[i]] + offset );

  if (res.size()==2) if (are_equal(res[0],res[1])) res.pop_back();
  return res;
}

// }}}

// safe test whether a point C \in [A,B], C \in (A,B): is_on_segment, is_inside_segment {{{
template<class T> bool is_on_segment(const complex<T> &A, const complex<T> &B, const complex<T> &C) { 
  if (!colinear(A,B,C)) return 0; 
  return ! is_positive( dot_product(A-C,B-C) ); 
}
template<class T> bool is_inside_segment(const complex<T> &A, const complex<T> &B, const complex<T> &C) { 
  if (!colinear(A,B,C)) return 0; 
  return is_negative( dot_product(A-C,B-C) ); 
}
// }}}

// intersect lines (A,B) and (C,D), return 2 pts if identical: intersect_line_line {{{
template<class T> vector< complex<T> > intersect_line_line(const complex<T> &A, const complex<T> &B, const complex<T> &C, const complex<T> &D) {
  vector< complex<T> > res;
  
  complex<T> U = B-A, V = D-C;
  if (colinear(U,V)) { // identical or parallel
    if (colinear(U,C-A)) { res.push_back(A); res.push_back(B); }
    return res;
  }
  // one intersection point
  T k = ( cross_product(C,V) - cross_product(A,V) ) / cross_product(U,V);
  res.push_back(A + k*U);
  return res;
}
// }}}

// intersect segments [A,B] and [C,D], may return endpoints of a segment: intersect_segment_segment {{{
template<class T> vector< complex<T> > intersect_segment_segment(const complex<T> &A, const complex<T> &B, const complex<T> &C, const complex<T> &D) {
  vector< complex<T> > res;

  complex<T> U = B-A, V = D-C, W = C-A, X = D-A;
  if (colinear(U,V)) { // parallel
    // check for degenerate cases
    if (are_equal(A,B)) { if (is_on_segment(C,D,A)) res.push_back(A); return res; }
    if (are_equal(C,D)) { if (is_on_segment(A,B,C)) res.push_back(C); return res; }
    // two parallel segments
    if (!colinear(U,W)) return res; // not on the same line
    
    T ssU = square_size(U), ssW = square_size(W), ssX = square_size(X);

    // does A coincide with C or D? 
    
    if (is_zero(ssW)) {
      res.push_back(A);
      if (is_negative(dot_product(U,X))) return res; // B, D on different sides 
      if (is_negative(ssU-ssX)) res.push_back(B); else res.push_back(D); 
      return res;
    }
    if (is_zero(ssX)) {
      res.push_back(A);
      if (is_negative(dot_product(U,W))) return res; // B, C on different sides
      if (is_negative(ssU-ssW)) res.push_back(B); else res.push_back(C); 
      return res;
    }
    
    if (is_inside_segment(C,D,A)) {
      // A is inside CD, intersection is [A,?] 
      res.push_back(A);
      if (is_positive(dot_product(U,W))) { // B,C on the same side 
        if (is_negative(ssU-ssW)) res.push_back(B); else res.push_back(C); 
        return res;
      } else { // B,D on the same side 
        if (is_negative(ssU-ssX)) res.push_back(B); else res.push_back(D); 
        return res;
      }
    } else {
      // segment CD is strictly on one side of A
      if (ssW < ssX) {
        // A_C_D
        if (is_negative(dot_product(U,W))) return res; // B is on the other side
        if (is_negative(ssU-ssW)) return res; // B before C
        if (are_equal(B,C)) { res.push_back(B); return res; }
        res.push_back(C);
        if (is_negative(ssU-ssX)) res.push_back(B); else res.push_back(D); 
        return res;
      } else {
        // A_D_C
        if (is_negative(dot_product(U,X))) return res; // B is on the other side 
        if (is_negative(ssU-ssX)) return res; // B before D
        if (are_equal(B,D)) { res.push_back(B); return res; }
        res.push_back(D);
        if (is_negative(ssU-ssW)) res.push_back(B); else res.push_back(C); 
        return res;
      }
    }
  }    
  // not parallel, at most one intersection point
  T k = ( cross_product(C,V) - cross_product(A,V) ) / cross_product(U,V);
  complex<T> cand = A + k*U;
  if (is_on_segment(A,B,cand) && is_on_segment(C,D,cand)) res.push_back(cand);
  return res;
}
// }}}

int main() {
    int T; cin >> T;
    FOR(test,1,T) {
        int N; cin >> N;
        vector<point> P;
        double x,y;
        for (int n=0; n<N; ++n) { cin >> x >> y; P.push_back(point(x,y)); }
        double area = poly_area(convex_hull(P));
        vector<int> poradie;
        for (int n=0; n<N; ++n) poradie.push_back(n);
        bool done = false;
        do {
            vector<point> Q;
            for (int n=0; n<N; ++n) Q.push_back( P[poradie[n]] );
            bool ok = true;
            for (int n=0; n<N; ++n) {
                for (int add=2; add<=N-2; ++add) {
                    vector<point> tmp = intersect_segment_segment( Q[n], Q[(n+1)%N], Q[(n+add)%N], Q[(n+add+1)%N] );
                    if (!tmp.empty()) ok = false;
                    if (!ok) break;
                }
                if (!ok) break;
            }
            if (ok) {
                double narea = poly_area(Q);
                if (narea > area/2 + EPSILON) {
                    done = true;
                    printf("Case #%d:",test);
                    for (int n=0; n<N; ++n) printf(" %d",poradie[n]);
                    printf("\n");
                    break;
                }
            }
        } while (next_permutation(poradie.begin()+1,poradie.end()));
        assert(done);
    }
}
// vim: fdm=marker:commentstring=\ \"\ %s:nowrap:autoread
