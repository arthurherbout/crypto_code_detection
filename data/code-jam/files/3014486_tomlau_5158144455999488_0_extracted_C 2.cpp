#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int T, t, i, N, W, H, B;
int NILE[500][100];

int main() {
  cin >> T;
  for (t=1; t<=T; t++) {
    cout << "Case #" << t << ": ";
    memset(NILE, 0, sizeof(NILE));
    cin >> W >> H >> B;
    for (i=0; i<B; i++) {
      int x0, y0, x1, y1;
      cin >> x0 >> y0 >> x1 >> y1;
      for (int x=x0; x<=x1; x++)
        for (int y=y0; y<=y1; y++)
          NILE[y][x] = -1;
    }

    int reached = 0;
    vector <pair<int, int> > wave, next;
    for (int x=0; x<W; x++)
      if (NILE[0][x] != -1) { 
        NILE[0][x] = 1; 
        next.push_back(make_pair(x, 0));
      }
    while (next.size() > 0) {
      wave = next;
      next.clear();
      for (auto &w : wave) {
        int x = w.first;
        int y = w.second;
        if (y < H-1 && NILE[y+1][x] == 0) { 
          NILE[y+1][x] = 1; 
          if (y+1 == H-1) reached++;
          else next.push_back(make_pair(x,y+1));
        }
        else if (x > 0 && NILE[y][x-1] == 0) { NILE[y][x-1] = 1; next.push_back(make_pair(x-1,y)); }
        else if (x < W-1 && NILE[y][x+1] == 0) { NILE[y][x+1] = 1; next.push_back(make_pair(x+1,y)); }
        else if (y > 0 && NILE[y-1][x] == 0) { NILE[y-1][x] = 1; next.push_back(make_pair(x,y-1)); }
        
      }
    }

    cout << reached << endl;
 }
}
