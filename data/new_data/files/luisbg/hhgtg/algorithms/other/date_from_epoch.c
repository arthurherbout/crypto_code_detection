/* Calendar date from the seconds elapsed since epoch */

#include <stdio.h>
#include <stdbool.h>

#define DAYSLEAPYEAR 366
#define DAYSNORMYEAR 365

typedef struct Date_T
{
  unsigned int sec;
  unsigned int min;
  unsigned int hour;
  unsigned int mday;            // day of the month [0-30]
  unsigned int month;           // month [0-11]
  unsigned int year;
  unsigned int wday;            // week day [0-6]
  unsigned int yday;            // year day [0-365]
} Date_T;


bool
is_leap_year (int y)
{
  return !(y % 4) && ((y % 100) || !(y % 400));
}

int
year_size (int y)
{
  return (is_leap_year (y)) ? DAYSLEAPYEAR : DAYSNORMYEAR;
}

Date_T
date_from_epoch (int since_epoch)
{
  int seconds_day = 86400;
  int dayclock;
  int dayno;
  bool is_leap;

  Date_T date;
  date.year = 1970;

  int days_before_month[2][13] = {
    /* Normal years */
    {0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365},
    /* Leap years */
    {0, 31, 60, 91, 121, 152, 182, 213, 243, 273, 304, 334, 365}
  };

  dayno = since_epoch / seconds_day;    // number of days since epoch
  dayclock = since_epoch % seconds_day; // number of seconds in the day

  date.hour = dayclock / 3600;  // use number of seconds in day
  dayclock = dayclock - (date.hour * 3600);     // to get hour, minute and second
  date.min = dayclock / 60;
  dayclock = dayclock - (date.min * 60);
  date.sec = dayclock;

  date.wday = (dayno + 4) % 7;  // work day is based on epoch being a Thursday

  while (dayno >= year_size (date.year)) {      // get year and month from days
    dayno -= year_size (date.year);
    date.year++;
  }
  date.yday = dayno;

  is_leap = (is_leap_year (date.year)) ? 1 : 0;
  date.month = 11;
  while (dayno < days_before_month[is_leap][date.month]) {
    date.month--;
  }

  dayno -= days_before_month[is_leap][date.month];
  date.mday = dayno;

  return date;
}


int
main ()
{
  Date_T date;

  int epoch_time[2] = { 433167300, 1386110494 };
  int c;

  for (c = 0; c < 2; c++) {
    date = date_from_epoch (epoch_time[c]);

    printf ("%10d -> %d-%02d-%02d  %02d:%02d\n", epoch_time[c], date.year,
        date.month + 1, date.mday + 1, date.hour, date.min);
  }

  return 0;
}
