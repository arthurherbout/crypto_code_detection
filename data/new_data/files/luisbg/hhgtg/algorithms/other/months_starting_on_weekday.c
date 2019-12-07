/* How many Sundays fell on the first of the month during the twentieth century
 * (1 Jan 1901 to 31 Dec 2000)?
 */

#include <stdio.h>
#include <stdbool.h>

typedef enum
{
  MONDAY, TUESDAY, WEDNESDAY, THURSDAY, FRIDAY, SATURDAY, SUNDAY
} dow;

// Months starting in given @day of the week. From year @low to year @high.
int months_starting_in_day (int low, int top, dow day);
static bool is_leap_year (int year);
static int days_in_month (int y, int m);


static bool
is_leap_year (int year)
{
  if (!(year % 4) && ((year % 100) || !(year % 400)))
    return true;
  else
    return false;
}

static int
days_in_month (int y, int m)
{
  if (m == 0 || m == 2 || m == 4 || m == 6 || m == 7 || m == 9 || m == 11)
    return 31;
  else if (m == 8 || m == 5 || m == 3 || m == 10)
    return 30;
  else if (m == 1) {
    if (is_leap_year (y))
      return 29;
    else
      return 28;
  }

  return -1;                    // if arrive here @m was out of range
}

int
months_starting_in_day (int low, int top, dow day)
{
  int year;
  int month;                    // month [0-11]
  int dim;                      // days in month
  dow fdm = MONDAY;             // first day of the month
  int c = 0;                    // counter of amount of months that start with the given week day

  if (low > top)
    return -1;

  for (year = 1900; year <= top; year++) {
    if (year >= low) {
      for (month = 0; month < 12; month++) {
        dim = days_in_month (year, month);

        if (fdm == day) {
          printf (": %d. %d\n", year, month + 1);
          c++;
        }

        fdm = (fdm + dim) % 7;
      }
    } else {
       if (is_leap_year (year))
         fdm = (fdm + 366) % 7;
       else
         fdm = (fdm + 365) % 7;
    }
  }

  return c;
}


int
main ()
{
  int tmsd = months_starting_in_day (1960, 2000, 6);
  printf ("\n");
  printf ("total: %d\n", tmsd);

  return 0;
}
