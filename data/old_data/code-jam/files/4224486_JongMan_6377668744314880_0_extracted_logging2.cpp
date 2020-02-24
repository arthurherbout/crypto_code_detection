#include<cstdio>
#include<vector>
#include<iostream>
#include<cmath>
#include<algorithm>
using namespace std;

const double EPSILON = 1e-8;
const double PI = 2.0 * acos(0.0);

// 2차원 벡터를 표현한다
struct vector2 {
  long long x, y;
  explicit vector2(long long x_ = 0, long long y_ = 0) : x(x_), y(y_) {}

  // 두 벡터의 비교
  bool operator == (const vector2& rhs) const { return x == rhs.x && y == rhs.y; }
  bool operator < (const vector2& rhs) const { if(x != rhs.x) return x < rhs.x; return y < rhs.y; }

  int quadrant() const {
    if(y >= 0 && x > 0) return 1;
    if(x <= 0 && y > 0) return 2;
    if(y <= 0 && x < 0) return 3;
    return 4;
  }

  // 벡터의 덧셈과 뺄셈
  vector2 operator + (const vector2& rhs) const { return vector2(x + rhs.x, y + rhs.y); }
  vector2 operator - (const vector2& rhs) const { return vector2(x - rhs.x, y - rhs.y); }
  vector2 operator - () const { return vector2(-x, -y); }

  // 내적/외적의 구현
  long long dot(const vector2& rhs) const { return x * rhs.x + y * rhs.y; }
  long long cross(const vector2& rhs) const { return x * rhs.y - rhs.x * y; }

};

// 원점에서 벡터 b 가 벡터 a 의 반시계 방향이면 양수, 시계 방향이면 음수, 평행이면 0 을 반환한다
long long ccw(vector2 a, vector2 b) {
  return a.cross(b);
}

// 점 p 를 기준으로 벡터 b 가 벡터 a 의 반시계 방향이면 양수, 시계 방향이면 음수, 평행이면 0 을 반환한다
long long ccw(vector2 p, vector2 a, vector2 b) {
  return ccw(a-p, b-p);
}

int n;
vector<vector2> p;

int solveFor(int tree) {
  if(n == 1) return 0;

  vector<vector2> q;
  for(int i = 0; i < n; ++i) 
    if(i != tree)
      q.push_back(p[i] - p[tree]);

  sort(q.begin(), q.end(), [](const vector2& a, const vector2& b) { 
       if(a.quadrant() != b.quadrant()) return a.quadrant() < b.quadrant();
       return ccw(a, b) > 0;
  });

  // printf("tree %d\n", tree);
  // for(auto qq: q) printf("\t point (%Ld, %Ld)\n", qq.x, qq.y);
  int ret = 987654321;

  int b = 0;
  for(int a = 0; a < q.size(); ++a) {
    int up = 0;
    int down = 0;
    for(int b = 0; b < q.size(); ++b) {
      long long c = ccw(q[a], q[b]);
      if(c > 0) ++up;
      if(c < 0) ++down;
    }

    ret = min(ret, min(up, down));
  }

  return ret;
}

int main() {
  int C;
  cin >> C;
  for(int cc = 0; cc < C; ++cc) {
    cout <<"Case #" << (cc+1) << ":" << endl;
    cin >> n;
    p.resize(n);
    for(int i = 0; i < n; ++i) 
      cin >> p[i].x >> p[i].y;

    for(int i = 0; i < n; ++i) {
      cout << solveFor(i) << endl;
    }
      
  }
  
  return 0;
}
