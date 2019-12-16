/* Inspired by FireCode problem 11 */

#include <stdlib.h>
#include <stdio.h>
#include <time.h>


void print_array (int arrayList[], int sz);

void rotate_left(int arraylist[], int sz, int k){
    int rc, r, save_first;

    // iterate the amount of rotations needed
    for (rc = 0; rc < k; rc++) {
        // move all once to the left
        save_first = arraylist[0];
        for (r = 0; r < (sz - 1); r++)
            arraylist[r] = arraylist[r + 1];
        arraylist[sz - 1] = save_first;
    }
}

int find_min_element_rotated (int arrayList[], int sz) {
  int beg = 0;
  int end = sz - 1;
  int mid;

  while (beg < end - 1) {
    mid = (beg + end) / 2;

    if (arrayList[mid] > arrayList[end]) {
      beg = mid;
    } else {
      end = mid;
    }
  }

  return end;
}

void print_array (int arrayList[], int sz)
{
  int i;

  for (i = 0; i < sz; i++)
      printf ("%3d ", arrayList[i]);

  printf ("\n");
}

void generate_list (int arrayList[], int sz)
{
  int i;
  int val = rand () % sz;

  for (i = 0; i < sz; i++) {
    arrayList[i] = val;
    val += rand () % sz;
  }
}

int main ()
{
  int sz = 10;
  int array[10];
  int k;

  /* use time as seed for pseud-randomness */
  srand(time(NULL));

  printf ("original list:\n");
  generate_list (array, sz);
  print_array (array, sz);

  k = rand () % (sz * 2);
  printf ("list rotated by %d:\n", k);
  rotate_left (array, sz, k);
  print_array (array, sz);

  k = find_min_element_rotated (array, sz);
  printf ("\nsmallest item is: %d\nat position %d\n", array[k], k);

  return 0;
}
