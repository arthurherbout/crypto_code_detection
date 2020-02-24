#include <stdio.h>

/* Loop recursively until it segfaults due to too many unended calls */
void
recurse (int count)
{                               /* Each call gets its own copy of count */
  printf ("%d\n", count);
  /* It is not necessary to increment count since each function's */
  /* variables are separate (so each count will be initialized one greater) */
  recurse (count + 1);
}

/* Loop counting to 100 */
void
count_to_hundred (int count)
{
  printf ("%d ", count);

  /* we only keep counting if we have a value under 100 */
  if (count < 100)
    count_to_hundred (count + 1);
}

/* Print numbers up sequentially */
void
printnum (int begin)
{
  printf ("%d", begin);
  if (begin < 9) {              /* The base case is when begin is no longer *//* less than 9 */
    printnum (begin + 1);
  }
  /* display begin again after we've already printed everything from 1 to 9 */
  /* and from 9 to begin + 1 */
  printf ("%d", begin);
}

/* Print numbers by order in which function calls end */
void
print_num_reverse (int begin)
{

  if (begin < 10) {
    begin++;
    print_num_reverse (begin);
    printf ("%d ", begin);
  }
}

int
main ()
{
  count_to_hundred (1);         /* First function call, so it starts at one */
  printf ("\n");

  print_num_reverse (0);
  printf ("\n");

  recurse (0);

  return 0;
}
