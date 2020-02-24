#include <stdio.h>

void
subsets (int ls[], int size)
{
  int bit, element;

  // The number of subsets is 1 << n
  for (bit = 0; bit < (1 << (size)); bit++) {
    // Compare bits to the number of elements
    // and output jth element when jth bit is 1
    for (element = 0; element < size; element++) {
      if (bit & (1 << element))
        printf ("%d, ", ls[element]);
    }
    printf ("\n");
  }
}

int
main ()
{
  int ls[] = { 0, 1, 2, 3, 4 };

  subsets (ls, sizeof (ls) / sizeof (int));

  return 0;
}
