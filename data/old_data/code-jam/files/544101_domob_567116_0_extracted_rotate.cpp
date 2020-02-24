/* CodeJam solution rotate in C++ by domob.  */

//#define NDEBUG

#include <cassert>
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <algorithm>
#include <stdint.h>

static const int empty = 0;
static const int red = 1;
static const int blue = 2;

int board[100][100];


/* Check for a single row win.  */

bool
hasInRow (int i, int j, int di, int dj, int n, int k, int p)
{
  for (int m = 0; m < k; ++m)
    {
      const int newI = i + m * di;
      const int newJ = j + m * dj;
      if (newI < 0 || newI >= n || newJ < 0 || newJ >= n)
        return false;

      if (board[newI][newJ] != p)
        return false;
    }

  return true;
}


/* Solve a single case.  */

void
solve_case ()
{
  int n, k;
  scanf ("%d %d\n", &n, &k);

  for (int i = 0; i < n; ++i)
    for (int j = 0; j < n; ++j)
      {
        while (true)
          {
            char c = getc (stdin);
            if (c == ' ' || c == '\n' || c == '\r')
              continue;

            switch (c)
              {
                case '.':
                  board[i][j] = empty;
                  break;
                case 'R':
                  board[i][j] = red;
                  break;
                case 'B':
                  board[i][j] = blue;
                  break;
                default:
                  printf ("X: %c\n", c);
                  assert (false);
              }

            break;
          }
      }

  /* Rotation has the effect, that all pieces will "fall" to the
     right in the original board.  Do this.  */

  for (int i = 0; i < n; ++i)
    for (int j = n - 1; j >= 0; --j)
      if (board[i][j] != empty)
        {
          int nj;
          for (nj = j; nj < n - 1; ++nj)
            {
              if (board[i][nj + 1] != empty)
                break;
            }

          if (nj == j)
            continue;

          assert (nj < n && nj >= j && board[i][nj] == empty);
          board[i][nj] = board[i][j];
          board[i][j] = empty;
        }

  // Check k in a row.
  bool wins[3];
  for (int p = 1; p <= 2; ++p)
    {
      wins[p] = false;
      for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j)
          {
            if (hasInRow (i, j, 0, 1, n, k, p)
                || hasInRow (i, j, 1, 0, n, k, p)
                || hasInRow (i, j, 1, 1, n, k, p)
                || hasInRow (i, j, 1, -1, n, k, p))
              wins[p] = true;
          }
    }

  if (wins[red] && wins[blue])
    printf ("Both");
  else if (wins[red])
    printf ("Red");
  else if (wins[blue])
    printf ("Blue");
  else
    printf ("Neither");
}


/* Main routine, handling the different cases.  */

int
main ()
{
  int n;

  scanf ("%d\n", &n);
  for (int i = 1; i <= n; ++i)
    {
      printf ("Case #%d: ", i);
      solve_case ();
      printf ("\n");
    }

  return EXIT_SUCCESS;
}
