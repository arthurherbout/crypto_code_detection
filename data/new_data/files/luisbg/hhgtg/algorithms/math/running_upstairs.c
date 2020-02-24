/* A child is running up a staircase with s steps, and can hop either 1, 2,  */
/* or 3 steps at a time. Implement a method to count how many possible ways  */
/* the child can run up the stairs.                                          */

#include <stdio.h>

long
running_stairs (int s)
{                               // O(3^n)
  if (s < 0) {
    return 0;
  } else if (s == 0) {
    return 1;
  } else {
    return running_stairs (s - 1) + running_stairs (s - 2) +
        running_stairs (s - 3);
  }
}

unsigned long
running_dynamic (int s, unsigned long *map)
{                               // O(n)
  if (s < 0)
    return 0;
  else if (s == 0)
    return 1;
  else if (map[s] > 0) {
    return map[s];

  } else {
    map[s] =
        running_dynamic (s - 1, map) +
        running_dynamic (s - 2, map) + running_dynamic (s - 3, map);
    return map[s];
  }
}


int
main ()
{
  int steps[10] = { 1, 2, 3, 4, 8, 10, 20, 25, 32, 36 };
  unsigned long map[37] = { 0 };
  int n;

  for (n = 0; n < 10; n++) {
    // printf ("%d: %d\n", steps[n], running_stairs (steps[n]));
    printf ("%d: %ld\n", steps[n], running_dynamic (steps[n], map));
  }
}
