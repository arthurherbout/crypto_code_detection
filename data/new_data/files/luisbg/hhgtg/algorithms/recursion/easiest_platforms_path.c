// Find the path with the easiest vertical jumps or drops
// Start at position with value 2 and finish at value 3
// Complexity is the size of the biggest jump required in the easiest path

#include <stdlib.h>
#include <stdio.h>


typedef struct map
{
  int foot_hold;
  int up_jump;
  int down_jump;
  int visited;
} map;


/* global variables */
int height, width;


void
set_up_jump (map level[height][width])
{
  int jump, h, w;

  // top of level can't go up
  for (w = 0; w < width; w++) {
    level[0][w].up_jump = 0;
  }


  for (h = 1; h < height; h++) {
    for (w = 0; w < width; w++) {
      // rest of the level
      for (jump = 1; jump <= h; jump++) {
        level[h][w].up_jump = 0;

        if (level[h - jump][w].foot_hold != 0) {
          level[h][w].up_jump = jump;
          break;
        }
      }
    }
  }
}

void
set_down_jump (map level[height][width])
{
  int jump, h, w;

  // bottom of level can't go down
  for (w = 0; w < width; w++) {
    level[height - 1][w].down_jump = 0;
  }

  // rest of the level
  for (h = height - 2; h >= 0; h--) {
    for (w = 0; w < width; w++) {
      level[h][w].down_jump = 0;

      for (jump = 1; jump < height; jump++) {
        if (level[h + jump][w].foot_hold != 0) {
          level[h][w].down_jump = jump;
          break;
        }
      }
    }
  }
}

int
try_level (int complexity, int cur_height, int cur_width,
    map level[height][width])
{
  int jump;
  map *cur_step = &level[cur_height][cur_width];

  if (cur_step->foot_hold == 3)
    return complexity;

  cur_step->visited = 1;
  jump = cur_step->up_jump;

  // try jump up
  if (jump != 0 && jump <= complexity &&
      !level[cur_height - jump][cur_width].visited)
    return try_level (complexity, cur_height - jump, cur_width, level);

  // try jump down
  jump = cur_step->down_jump;
  if (jump != 0 && jump <= complexity &&
      !level[cur_height + jump][cur_width].visited)
    return try_level (complexity, cur_height + jump, cur_width, level);

  // try neighbor left
  if (cur_width >= 0 && !level[cur_height][cur_width + 1].visited)
    return try_level (complexity, cur_height, cur_width + 1, level);

  // try neighbor right
  if (cur_width < width && !level[cur_height][cur_width - 1].visited)
    return try_level (complexity, cur_height, cur_width - 1, level);

  return 0;
}

void
print_map (int height, int width, map level[height][width])
{
  int h, w;

  for (h = 0; h < height; h++) {
    for (w = 0; w < width; w++) {
      printf ("%d ", level[h][w].foot_hold);
    }

    printf ("\n");
  }
}

int
main ()
{
  int test_case, T;
  int h, w;

  scanf ("%d", &T);

  for (test_case = 1; test_case <= T; test_case++) {
    int complexity = 0;

    scanf ("%d %d", &height, &width);

    map level[height][width];
    int cur_height = height - 1, cur_width = 0;

    for (h = 0; h < height; h++) {
      for (w = 0; w < width; w++) {
        scanf ("%d", &level[h][w].foot_hold);
      }
    }

    print_map (height, width, level);

    set_up_jump (level);
    set_down_jump (level);

    while (1) {
      complexity++;

      for (h = 0; h < height; h++) {
        for (w = 0; w < width; w++) {
          level[h][w].visited = 0;
        }
      }

      if (try_level (complexity, cur_height, cur_width, level))
        break;
    }

    printf ("Platform level: #%d: %d\n\n", test_case, complexity);
  }

  return 0;
}
