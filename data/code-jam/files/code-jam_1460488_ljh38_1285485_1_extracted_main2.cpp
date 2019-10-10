#include <iostream>
#include <vector>
#include <fstream>

using namespace std;

char map[31][31];

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

int sign(int dir)
{
  return (dir > 0) ? 1 : -1;
}

void getNextSquare(int startX, int startY, int curX, int curY, int dX, int dY, int* nextX, int* nextY)
{
  if (dX == 0) {
    *nextX = curX;
    *nextY = curY + sign(dY);
  } else if (dY == 0) {
    *nextX = curX + sign(dX);
    *nextY = curY;
  } else {
    // diagonal.  does it go through corner?
    int det = (2*curX + sign(dX) - 2*startX)*dY - (2*curY + sign(dY) - 2*startY)*dX;
    // cout << "det = " << det << endl;
    if (det == 0) {
      // right through corner
      *nextX = curX + sign(dX);
      *nextY = curY + sign(dY);
    } else if (det > 0) {
      // corner is below ray, if dx > 0
      if (dY > 0 && dX > 0) {
	*nextX = curX;
	*nextY = curY + 1;
      } else if (dY > 0 && dX < 0) {
	*nextX = curX - 1;
	*nextY = curY;
      } else if (dY < 0 && dX > 0) {
	*nextX = curX + 1;
	*nextY = curY;
      } else {
	*nextX = curX;
	*nextY = curY + sign(dY);
      }      
    } else {
      // corner is above ray, if dx > 0
      if (dY > 0 && dX > 0) {
	*nextX = curX + 1;
	*nextY = curY;
      } else if (dY > 0 && dX < 0) {
	*nextX = curX;
	*nextY = curY + sign(dY);
      } else if (dY < 0 && dX > 0) {
	*nextX = curX;
	*nextY = curY + sign(dY);
      } else {
	*nextX = curX + sign(dX);
	*nextY = curY;
      }
    }
  }
}

bool traceRay(int startX, int startY, int curX, int curY, int dX, int dY, int D)
{
  // cout << "Trace " << startX << ", " << startY << " -> " << curX << ", " << curY << " [ " << dX << ", " << dY << " ]" << endl;
  
  // stop if gone too far
  if (((startX - curX)*(startX - curX)) + ((startY - curY)*(startY - curY)) > D*D) {
    // cout << "Gone too far\n";
    return false;
  }

  // are we in an X here?
  if (map[curX][curY] == 'X') {
    // this is where we are!  does the ray go through here perfectly?
    if ((curX - startX)*dY == (curY - startY)*dX) {
      // yes - is the distance ok?
      int distSquared = ((curX - startX)*(curX - startX) + (curY - startY)*(curY - startY));
      if (distSquared > 0) {
	// cout << "Got it\n";
	return true;
      }
    }
  }
  
  int nextX, nextY;
  getNextSquare(startX, startY, curX, curY, dX, dY, &nextX, &nextY);

  if (map[nextX][nextY] == '#') {
    // headed into a mirror
    if (nextX != curX && nextY != curY) {
      // corner case.  there are ? cases here:
      // (a) corner of 3 adjacent mirrors, return same way
      // (b) corner of 1 (nothing adjacent), absorbed
      // (c) corner of 2, flat edge to reflect
      if (map[nextX][curY] == '#' && map[curX][nextY] == '#') {
	// 3 adjacent, reflect back the way we came
	return traceRay(nextX + curX - startX, nextY + curY - startY, curX, curY, -dX, -dY, D);
      } else if (map[nextX][curY] == '#') {
	// 2 adjacent, reflect in X
	return traceRay(nextX + curX - startX, startY, curX, curY, -dX, dY, D);
      } else if (map[curX][nextY] == '#') {
	// 2 adjacent, reflect in Y
	return traceRay(startX, nextY + curY - startY, curX, curY, dX, -dY, D);
      } else {
	// none adjacent, absorb
	return false;
      }
    } else if (nextX != curX) {
      // reflect in X dir
      return traceRay(nextX + curX - startX, startY, curX, curY, -dX, dY, D);
    } else {
      // reflect in Y dir
      return traceRay(startX, nextY + curY - startY, curX, curY, dX, -dY, D);
    }
  }

  return traceRay(startX, startY, nextX, nextY, dX, dY, D);
}

int main()
{
  // ifstream input("D-small-attempt0.in");
  istream& input = cin;
  int T;
  input >> T;

  for (int t = 0; t != T; ++t) {
    int H, W, D;
    input >> H >> W >> D;

    int startX = -1;
    int startY = -1;
    
    for (int h = 0; h != H; ++h) {
      string line;
      input >> line;
      for (int w = 0; w != W; ++w) {
	map[w][h] = line[w];
	if (line[w] == 'X') {
	  startX = w;
	  startY = h;
	}
      }
    }
    
    // generate list of all possible ray directions
    vector<int> dirsX;
    vector<int> dirsY;
    for (int dx = -D; dx <= D; ++dx) {
      for (int dy = -D; dy <= D; ++dy) {
	if (dx == 0 && dy == 0) continue;
	if (!collinear(dx, dy, dirsX, dirsY)) {
	  dirsX.push_back(dx);
	  dirsY.push_back(dy);
	}
      }
    }

    // now try them all
    int count = 0;
    for (size_t i = 0; i != dirsX.size(); ++i) {
      int dX = dirsX[i];
      int dY = dirsY[i];
      // cout << "Trying " << dX << ", " << dY << endl;
      if (traceRay(startX, startY, startX, startY, dX, dY, D)) {
	++count;
      }
    }
    
    cout << "Case #" << t + 1 << ": " << count << endl;
  }
  return 0;
}
