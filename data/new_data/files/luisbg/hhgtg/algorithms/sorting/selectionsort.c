/* Selection Sort
 *
 * Despite it's brute-force approach, selection sort outperforms more
 * sophisticated algorithms when sorting files with huge items and small
 * keys. The cost of moving data dominates the cost of making comparisons.
 * No algorithm can sort a file with substantially less data movement than
 * selection sort.
*/

#include <stdlib.h>
#include <stdio.h>


typedef int Item;
#define less(A, B) (A < B)
#define exch(A, B) { Item t = A; A = B; B = t; }
#define print(A) {printf ("%3d ", A); }
#define RANDMAX 100

void selection_sort (Item a[], int l, int r);
void print_list (Item a[], int len);


void
selection_sort (Item a[], int l, int r)
{
  int i, j;

  for (i = l; i < r; i++) {
    int min = i;

    for (j = i + 1; j <= r; j++)
      if (less (a[j], a[min]))
        min = j;
    if (i != min) {
      exch (a[i], a[min]);
      // print_list (a, r - l + 1);
    }
  }
}

void
print_list (Item a[], int len)
{
  int i;

  for (i = 0; i < len; i++)
    print (a[i]);
  printf ("\n");

}


int
main (int argc, char *argv[])
{
  int i;
  int N = atoi (argv[1]);

  int *a = malloc (N * sizeof (int));

  printf ("sorting %d random numbers from 0 to %d:\n", N, RANDMAX);
  for (i = 0; i < N; i++)
    a[i] = rand () % RANDMAX;

  print_list (a, N);

  selection_sort (a, 0, N - 1);

  print_list (a, N);

  return 0;
}
