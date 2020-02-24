#include <iostream>
#include <vector>

using namespace std;

bool collinear(int dx, int dy, const vector<int>& X, const vector<int>& Y)
{
  for (size_t i = 0; i != X.size(); ++i) {
    if (dx*Y[i] == dy*X[i]) {
      // collinear ... just check in same direction from origin.  pick
      // a non-zero axis and compare sign
      if (dx != 0 && ((dx > 0) == (X[i] > 0))) {
	return true;
      }
      if (dy != 0 && ((dy > 0) == (Y[i] > 0))) {
	return true;
      }
    }
  }
  return false;
}

int main()
{
  int T;
  cin >> T;

  for (int t = 0; t != T; ++t) {
    int H, W, D;
    cin >> H >> W >> D;
    int twoX = -1;
    int twoY = -1;
    for (int h = 0; h != H; ++h) {
      string line;
      cin >> line;
      for (int w = 0; w != W; ++w) {
	if (line[w] == 'X') {
	  // x = w - 0.5
	  twoX = 2*w - 1;
	  twoY = 2*h - 1;
	}
      }
    }

    // loop over possible solutions
    int numW = (D / (W - 2)) + 1;
    int numH = (D / (H - 2)) + 1;

    vector<int> solnsX;
    vector<int> solnsY;

    for (int sW = -numW; sW <= numW; ++sW) {
      for (int sH = -numH; sH <= numH; ++sH) {
	int deltaX = sW * (W - 2);
	if (sW & 1) {
	  deltaX += (W - 2) - twoX;
	}

	int deltaY = sH * (H - 2);
	if (sH & 1) {
	  deltaY += (H - 2) - twoY;
	}

	if (deltaX == 0 && deltaY == 0) continue;

	// how far is this from original you
	int distSquared = deltaX*deltaX + deltaY*deltaY;
	if (distSquared <= D *D) {

	  // this is in range, so it's a valid series of reflection rays

	  // we just have to eliminate multiple collinear solutions - light doesn't go
	  // through you.  pretty inefficient but we're only attempting small case, just check
	  // against all solutions found so far.
	  if (!collinear(deltaX, deltaY, solnsX, solnsY)) {
	    solnsX.push_back(deltaX);
	    solnsY.push_back(deltaY);
	  }
	}
      }
    }

    cout << "Case #" << t + 1 << ": " << solnsX.size() << endl;
  }
  return 0;
}
