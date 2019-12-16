#include <iostream>
#include <set>
#include <vector>
#include <algorithm>
using namespace std;

struct cmp1 {
  int nx, ny;
  cmp1(int a, int b) : nx(a), ny(b) {}
  bool operator()(const pair<int, int>& a, const pair<int, int>& b) const {
    return nx * a.first + ny * a.second < nx * b.first + ny * b.second;
  }
};

void doit(int casenum) {
  int N;
  cin >> N;
  cerr.precision(18);
  cout.setf(ios::fixed);
  cout.precision(18);
  vector<pair<int, int> > points(4*N);
  for(unsigned i = 0; i < 4*N; ++i)
    cin >> points[i].first >> points[i].second;
  for(unsigned i = 0; i < 4*N; ++i) {
    for(unsigned j = 0; j < 4*N; ++j) {
      if(i == j) continue;
      int dx = points[j].first - points[i].first;
      int dy = points[j].second - points[i].second;
      if(!dx && !dy) abort();
      int nx = -dy;
      int ny = dx;
      double nc = nx * points[i].first + ny * points[i].second - 1e-12;
      
      unsigned cnt = 0;
      for(unsigned k = 0; k < 4 * N; ++k) {
        if(nx * points[k].first + ny * points[k].second >= nc)
          cnt++;
      }
      if(cnt != 2 * N)
        continue;
      vector<pair<int, int> > p0, p1;
      for(unsigned k = 0; k < 4 * N; ++k) {
        if(nx * points[k].first + ny * points[k].second >= nc)
          p0.push_back(points[k]);
        else
          p1.push_back(points[k]);
      }
      sort(p0.begin(), p0.end(), cmp1(dx, dy));
      sort(p1.begin(), p1.end(), cmp1(dx, dy));
      double dc = dx * p0[N].first + dy * p0[N].second - 1e-12;
      if(dc < dx * p0[N-1].first + dy * p0[N-1].second)
        break;
      unsigned cnt2 = 0;
      for(unsigned i = 0; i < p1.size(); ++i)
        if(p1[i].first * dx + p1[i].second * dy >= dc)
          cnt2++;
      if(cnt2 != N) break;
      double l = (dc - points[i].first * dx - points[i].second * dy) / (dx*dx + dy * dy);
      double px = points[i].first + l * dx;
      double py = points[i].second + l * dy;
      //cerr << nx << "  "<< ny << " " << nc << endl;
      //cerr << (px * nx + py * ny) << " " << nc << endl;
      //cerr << (px * dx + py * dy) << " " << dc << endl;
      cout << "Case #" << casenum << ": " << px << " " << py << " " << (px + dx) << "  "<< (py + dy) <<  endl;
      return;
    }
  }
  cout << "Case #" << casenum << ": IMPOSSIBLE" << endl;
}

int main(int argc, char **argv) {
    int T;
    cin >> T;
    for(int i = 1; i <= T; ++i)
      doit(i);
    return 0;
}
