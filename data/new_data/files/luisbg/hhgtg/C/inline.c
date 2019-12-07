/* "Firstly, it serves as a compiler directive that suggests (but does not
 * require) that the compiler substitute the body of the function inline by
 * performing inline expansion, i.e. by inserting the function code at the
 * address of each function call, thereby saving the overhead of a function
 * call."
 *
 * https://en.wikipedia.org/wiki/Inline_function
 */

#include <stdio.h>
#include <stdbool.h>

static inline bool
is_leap_year (int y)
{
  return !(y % 4) && ((y % 100) || !(y % 400));
}

int
main ()
{
  int year;

  printf ("Enter a year (last 1582): ");
  scanf ("%d", &year);

  printf ("%d is%s a leap year\n", year, is_leap_year (year) ? "" : " not ");

  return 0;
}
