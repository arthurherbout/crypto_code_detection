/* Bubble Sort */

#include <stdlib.h>
#include <stdio.h>


typedef int Item;
#define less(A, B) (A < B)
#define exch(A, B) { Item t = A; A = B; B = t; }
#define print(A) {printf ("%3d ", A); }
#define RANDMAX 100

void bubble_sort (Item a[], int l, int r);
void print_list (Item a[], int len);


void
bubble_sort (Item a[], int l, int r)
{
  int i, j;

  for (i = l; i < r; i++)
    for (j = r; j > i; j--)
      if (less (a[j], a[j - 1])) {
        exch (a[j-1], a[j]);
        // print_list (a, r - l + 1);
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

  bubble_sort (a, 0, N - 1);

  print_list (a, N);

  return 0;
}
