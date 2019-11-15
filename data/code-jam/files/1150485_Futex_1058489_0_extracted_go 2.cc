#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;

vector<int> vens;
int dist;

double Go() {
  if (vens.size() <= 1) {
    return 0.0;
  }

  int left_idx = 0;
  int right_idx = vens.size() - 1;
  double time = 0.0;

  while (left_idx < right_idx) {
    if (left_idx + 1 == right_idx) {
      // special
      int tot_dist = vens[right_idx] - vens[left_idx];
      if (tot_dist < dist) {
	time += (dist - tot_dist) / 2.0;
      }
      break;

    } else {
      // normal
      int dist_left = vens[left_idx + 1] - vens[left_idx];
      int dist_right = vens[right_idx] - vens[right_idx - 1];
      int max_dist = max(dist_left, dist_right);
      if (max_dist >= dist) {
	++left_idx;
	--right_idx;
	continue;
      }
      time += (dist - max_dist);
      if (dist_left == max_dist) {
	++left_idx;
      } else {
	vens[left_idx] -= (dist - max_dist);
      }
      if (dist_right == max_dist) {
	--right_idx;
      } else {
	vens[right_idx] += (dist - max_dist);
      }
    }
  }

  return time;
}

int main(int argc, char **argv) {
  fstream fin("in");

  int cases;
  fin >> cases;

  for (int C = 0; C < cases; ++C) {
    int num_pts;
    fin >> num_pts >> dist;

    vens.clear();
    for (int idx = 0; idx < num_pts; ++idx) {
      int num_ven, pt;
      fin >> pt >> num_ven;
      
      for (int v = 0; v < num_ven; ++v) {
	vens.push_back(pt);
      }
    }

    sort(vens.begin(), vens.end());

    cout << "Case #" << (C+1) << ": ";
    cout << Go() << endl;
  }

  return 0;
}
