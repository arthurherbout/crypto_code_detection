/* Given a matrix MxN in which each row and each column is sorted in */
/* ascending order, write a method to find the element */

#include <stdio.h>
#include <stdlib.h>

void print (int m, int n, int l[m][n]);

int search (int key, int m, int n, int l[m][n])
{
  int c, r;
  r = n - 1;
  c = 0;

  while (r >= 0 && c < n) {
    if (l[c][r] == key)
      return (c * 4) + r + 1;

    if (l[c][r] > key)
      --r;
    else if (l[c][r] < key)
      ++c;
  }

  return -1;
}

void print (int m, int n, int l[m][n])
{
  int c, r;
  for (c = 0; c < n; c++) {
    for (r = 0; r < m; r++) {
      printf ("%d ", l[c][r]);
    }
    printf ("\n");
  }
}


int main ()
{
  int m = 4;
  int n = 4;
  int l[4][4] = {{15, 20, 40, 85},
                 {20, 35, 80, 95},
                 {30, 55, 95, 105},
                 {40, 80, 100, 120}};

  print (m, n, l);

  printf ("\n");
  printf ("pos of %d is %d\n", 15, search (15, m, n, l));
  printf ("pos of %d is %d\n", 55, search (55, m, n, l));
  printf ("pos of %d is %d\n", 60, search (60, m, n, l));
  printf ("pos of %d is %d\n", 100, search (100, m, n, l));
  printf ("pos of %d is %d\n", 120, search (120, m, n, l));

  return 0;
}
