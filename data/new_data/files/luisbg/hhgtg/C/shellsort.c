#include <stdio.h>

/* shellsort: sort v[0]...v[n-1] into increasing order */
void
shellsort (int v[], int n)
{
  int gap, i, j, temp;
  for (gap = n / 2; gap > 0; gap /= 2)
    for (i = gap; i < n; i++)
      for (j = i - gap; j >= 0 && v[j] > v[j + gap]; j -= gap) {
        temp = v[j];
        v[j] = v[j + gap];
        v[j + gap] = temp;
      }
}


int
main ()
{
  int v[10] = { 9, 7, 5, 3, 1, 2, 4, 6, 8, 0 };
  shellsort (v, 10);

  int c;
  for (c = 0; c < 10; c++) {
    printf ("%d ", v[c]);
  }

  printf ("\n");

  return 0;
}
