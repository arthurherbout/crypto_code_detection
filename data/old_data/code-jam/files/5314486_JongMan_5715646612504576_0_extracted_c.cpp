#include<cstring>
#include<iostream>
#include<algorithm>
#include<sstream>
#include<string>
#include<vector>
#include<cmath>
#include<cstdio>
#include<cstdlib>
#include<fstream>
#include<cassert>
#include<numeric>
#include<set>
#include<map>
#include<queue>
#include<list>
#include<deque>
using namespace std;

int width, height;
char maze[5][51];
int laser_count[51];
int laser_position[51];
int laser_index[51][5];
int walls[51];

bool checked[51][1<<5][1<<5][1<<5];

int possible(int x, int incoming, int needToCover, int exposed) {
  // printf("possible(x=%d, incoming=%d, needToCover=%d, exposed=%d) laserCount %d\n", x, incoming, needToCover, exposed, laser_count[x]);
  if(x == width) return (needToCover == 0);
  if(checked[x][incoming][needToCover][exposed]) return false;
  checked[x][incoming][needToCover][exposed] = true;

  if(incoming & laser_position[x]) return false;
  if(needToCover & walls[x]) return false;

  // if(x == 42) {
  //   printf("\n");
  // }
  for(int horz = 0; horz < (1<<laser_count[x]); ++horz) {
    int horizontal = 0;
    for(int i = 0; i < laser_count[x]; ++i) {
      if(horz & (1<<i)) 
        horizontal += (1<<laser_index[x][i]);
    }
    if (horizontal & exposed) continue;
    if ((needToCover & laser_position[x]) & ~horizontal) continue;

    bool ok = true;
    int columnCovered = laser_position[x] | incoming | walls[x];

    for(int i = 0; i < laser_count[x]; ++i) {
      int y = laser_index[x][i];
      if (horz & (1<<i)) {
        maze[y][x] = '-';
      }
      else {
        maze[y][x] = '|';
        for(int yy = y-1; yy >= 0; --yy) {
          if(maze[yy][x] == '#') break;
          else if(maze[yy][x] == '.') columnCovered |= (1<<yy);
          else {
            ok = false; // hit a laser
            break;
          }
        }
        for(int yy = y+1; yy < height; ++yy) {
          if(maze[yy][x] == '#') break;
          else if(maze[yy][x] == '.') columnCovered |= (1<<yy);
          else {
            ok = false; // hit a laser
            break;
          }
        }
      }
    }


    int all = (1<<height)-1;
    int leftOut = (1<<height) - 1 - columnCovered;
    // printf("x %d horz %d ok %d leftOut %d\n", x, horz, ok, leftOut);
    if (!ok) continue;
    int newIncoming = (incoming & ~walls[x]) | horizontal;
    int newNeedToCover = (needToCover & ~horizontal) | leftOut;
    int newExposed = (exposed & ~walls[x]) | laser_position[x];
    if(possible(x+1, newIncoming, newNeedToCover, newExposed)) {
      return true;
    }
  }
  return false;
}

int main() {
  int cases;
  cin >> cases;
  for(int cc = 0; cc < cases; ++cc) {
    cin >> height >> width;
    for(int i = 0; i < height; ++i) cin >> maze[i];
    for(int x = 0; x < width; ++x) {
      int c = 0;
      laser_position[x] = 0;
      walls[x] = 0;
      for(int y = 0; y < height; ++y) {
        if(maze[y][x] == '-' || maze[y][x] == '|') {
          laser_position[x] += (1<<y);
          laser_index[x][c++] = y;
        }
        if(maze[y][x] == '#') {
          walls[x] += (1<<y);
        }
      }
      laser_count[x] = c;
    }
    memset(checked, 0, sizeof checked);
    if (possible(0, 0, 0, 0)) {
      printf("Case #%d: POSSIBLE\n", cc+1);
      for(int i = 0; i < height; ++i) 
        printf("%s\n", maze[i]);
    }
    else {
      printf("Case #%d: IMPOSSIBLE\n", cc+1);
    }
  }
}
