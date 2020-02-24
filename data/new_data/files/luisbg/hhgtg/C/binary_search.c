#include <stdio.h>

int
binsearch (int x, int v[], int n)
{
  int low, high, mid;
  low = 0;
  high = n - 1;

  while (low <= high) {
    mid = (low + high) / 2;
    if (v[mid] < x)
      low = mid + 1;
    else if (v[mid] > x)
      high = mid - 1;
    else
      /* found match */
      return mid;
  }

  /* no match */
  return -1;
}

int
main ()
{
  int n, c, search, pos, array[100];

  printf ("Enter number of elements\n");
  scanf ("%d", &n);

  printf ("Enter %d integers\n", n);

  for (c = 0; c < n; c++)
    scanf ("%d", &array[c]);

  printf ("Enter value to find\n");
  scanf ("%d", &search);

  pos = binsearch (search, array, n);
  printf ("%d found at location %d.\n", search, pos + 1);

  return 0;
}
