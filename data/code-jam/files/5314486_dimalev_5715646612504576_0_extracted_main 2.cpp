#include <iostream>
#include <cstring>
#include <vector>

#ifdef ALGO_DEBUG
#include "../test/debug.cpp"
#else

#define TRACE(message)
#define TRACE_LINE(message)
#define ASSERT(expr)
#define UNIT_TESTS()

#endif

void unit_tests() {
}

constexpr int max_rows = 5;
constexpr int max_cols = 50;

int rows, cols;
char home[max_rows + 1][max_cols + 1];

void solve(int t) {
  std::cin >> rows >> cols;
  std::vector<std::pair<int, int>> beams;
  int total_beams = 0;
  for(int r = 0; r < rows; ++r) {
    std::cin >> home[r];
    for(int c = 0; c < cols; ++c) {
      if(home[r][c] == '-' || home[r][c] == '|') {
        ++total_beams;
        beams.push_back(std::make_pair(r, c));
      }
    }
  }
  int max_var = 1 << total_beams;
  for(int i = 0; i < max_var; ++i) {
    bool good = true;
    int m = 1, id = 0;
    bool filled[rows][cols];
    std::memset(filled, 0, rows * cols);
    for(auto beam : beams) {
      int r = beam.first, c = beam.second;
      if(filled[r][c]) {
        good = false;
        break;
      }
      if(i & m) {
        home[r][c] = '-';
        bool left = true, right = true;
        for(int k = 1; k < cols && (left || right); ++k) {
          if(c - k >= 0 && left) {
            if(home[r][c - k] == '#') {
              left = false;
            } else {
              filled[r][c - k] = true;
            }
          } else left = false;
          if(c + k < cols && right) {
            if(home[r][c + k] == '#') {
              right = false;
            } else {
              filled[r][c + k] = true;
            }
          } else right = false;
        }
      } else {
        home[r][c] = '|';
        bool top = true, down = true;
        for(int k = 1; k < rows && (top || down); ++k) {
          if(r - k >= 0 && top) {
            if(home[r - k][c] == '#') {
              top = false;
            } else {
              filled[r - k][c] = true;
            }
          } else top = false;
          if(r + k < rows && down) {
            if(home[r + k][c] == '#') {
              down = false;
            } else {
              filled[r + k][c] = true;
            }
          } else down = false;
        }
      }
    }
    if(!good) continue;
    for(int r = 0; r < rows; ++r) {
      for(int c = 0; c < cols; ++c) {
        if(home[r][c] == '.' && !filled[r][c]) {
          good = false;
          break;
        }
      }
      if(!good) break;
    }
    if(good) {
      std::cout << "Case #" << t << ": POSSIBLE\n";
      for(int r = 0; r < rows; ++r) {
        std::cout << home[r] << "\n";
      }
      return;
    }
  }
  std::cout << "Case #" << t << ": IMPOSSIBLE\n";
}

int main() {
  UNIT_TESTS();
  int T;
  std::cin >> T;
  for(int t = 1; t <= T; ++t) solve(t);
  return 0;
}
