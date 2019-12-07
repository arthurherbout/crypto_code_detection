// Given a time in 12-hour AM/PM format, convert it to 24-hour time

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int
main ()
{
  char *time = (char *) malloc (11 * sizeof (char));
  char *time_24 = (char *) calloc (9, sizeof (char));
  scanf ("%s", time);

  if (time[8] == 'A') {
    strncpy (time_24, time, 8);

    // Midnight is "12:00:00AM" on a 12-hour clock,
    // and "00:00:00" on a 24-hour clock.
    if (strncmp (time, "12", 2) == 0) {
      strncpy (time_24, "00", 2);
    }
  } else {
    int hour, h1, h2;

    h1 = time[0] - '0';
    h2 = time[1] - '0';

    hour = (h1 * 10) + h2 + 12;
    strncpy (time_24, time, 8);

    // Noon is "12:00:00PM" on a 12-hour clock,
    // and "12:00:00" on a 24-hour clock.
    if (strncmp (time, "12", 2) == 0) {
      strncpy (time_24, "12", 2);
    } else {
      time_24[0] = (hour / 10) + '0';
      time_24[1] = (hour % 10) + '0';
    }
  }

  printf ("%s\n", time_24);

  return 0;
}
