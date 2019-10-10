#include <bits/stdc++.h>
#define LL long long
#define ABS(a) (((a) > 0) ? (a) : (-(a)))
#define MAX(a,b) (((a)>(b)) ? (a) : (b))
#define MIN(a,b) (((a)<(b)) ? (a) : (b))
#define FOR(i,n) for(int i=0;i<(n);++i)
#define pb push_back
using namespace std;
template<typename T> ostream& operator<<(ostream& s, vector<T>& v)
{ s << '{'; for (int i = 0 ; i < v.size(); ++i) s << (i ? "," : "") << v[i]; return s << '}'; }
template<typename S, typename T> ostream& operator<<(ostream &s, pair<S,T>& p)
{ return s << "(" << p.first << "," << p.second << ")"; }


typedef double coord_t;         // coordinate type
typedef double coord2_t;  // must be big enough to hold 2*max(|coordinate|)^2
 
struct Point {
	coord_t x, y;
 
	bool operator <(const Point &p) const {
		return x < p.x || (x == p.x && y < p.y);
	}
};
 
// 2D cross product of OA and OB vectors, i.e. z-component of their 3D cross product.
// Returns a positive value, if OAB makes a counter-clockwise turn,
// negative for clockwise turn, and zero if the points are collinear.
coord2_t cross(const Point &O, const Point &A, const Point &B)
{
	return (A.x - O.x) * (B.y - O.y) - (A.y - O.y) * (B.x - O.x);
}
 
// Returns a list of points on the convex hull in counter-clockwise order.
// Note: the last point in the returned list is the same as the first one.
vector<Point> convex_hull(vector<Point> P)
{
	int n = P.size(), k = 0;
	vector<Point> H(2*n);
 
	// Sort points lexicographically
	sort(P.begin(), P.end());
 
	// Build lower hull
	for (int i = 0; i < n; ++i) {
		while (k >= 2 && cross(H[k-2], H[k-1], P[i]) <= 0) k--;
		H[k++] = P[i];
	}
 
	// Build upper hull
	for (int i = n-2, t = k+1; i >= 0; i--) {
		while (k >= t && cross(H[k-2], H[k-1], P[i]) <= 0) k--;
		H[k++] = P[i];
	}
 
	H.resize(k);
	return H;
}

ostream& operator<<(ostream &s, Point p)
{ return s << "(" << p.x << "," << p.y << ")"; }

bool collinear(Point p1, Point p2, Point p3){
  bool col = cross(p3, p1, p2) == 0;
  //cout << p1 << " " << p2 << " " << p3 << " " << col << endl;
  return col;
}

set<Point> convex_boundary(vector<Point> P){
  set<Point> boundary;
  vector<Point> hull = convex_hull(P);
  for(int i=0; i<hull.size()-1; i++){
    Point p1 = hull[i];
    Point p2 = hull[i+1];
    FOR(j,P.size()){
      if(collinear(p1,p2,P[j])){
        boundary.insert(P[j]);
      }
    }
  }
  return boundary;
}

int main(){
  int TC;cin>>TC;
  FOR(tc,TC){
    int N;cin>>N;
    vector<Point> pts;
    map<Point,int> tree_map;
    FOR(i,N){
      Point pt;
      cin >> pt.x >> pt.y;
      pts.pb(pt);
      tree_map[pt] = i;
    }

    int mintrees[N];
    FOR(i,N) mintrees[i] = 10000000;

    if(N == 1) mintrees[0] = 0;

    FOR(perm,1<<N){
      // construct new list of trees on bit i set => not cut down
      vector<Point> cur_trees;
      int numtrees = 0;
      FOR(b,N){
        if(perm & (1<<b)){
          cur_trees.pb(pts[b]);
          numtrees++;
        }
      }
      int cut_down = N - numtrees;
      if(numtrees == 0)
        continue;

      set<Point> convexbd = convex_boundary(cur_trees);
      for(set<Point>::iterator it = convexbd.begin(); it != convexbd.end(); it++){
        int ct = tree_map[*it];
        if(cut_down < mintrees[ct]){
          mintrees[ct] = cut_down;
        }
      }
    }

    cout << "Case #" << tc+1 << ":" << endl;
    FOR(i,N){
      cout << mintrees[i] << endl;
    }
  }
}
