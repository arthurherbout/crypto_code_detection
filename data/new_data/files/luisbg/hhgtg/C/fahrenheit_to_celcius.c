#include <stdio.h>

/* print Fahrenheit-Celsius table
 * for fahr = 0, 20, ..., 300 */

#define LOWER 0                 /* lower limit of the table */
#define UPPER 300               /* upper limit of the table */
#define STEP 20                 /* step size */


int
main ()
{
  float fahr, celsius;
  float lower, upper, step;
  lower = 0;
  upper = 300;
  step = 20;

  /* lower limit of temperatuire scale */
  /* upper limit */
  /* step size */
  fahr = lower;
  while (fahr <= upper) {
    celsius = (5.0 / 9.0) * (fahr - 32.0);
    printf ("%3.0f %6.1f\n", fahr, celsius);
    fahr = fahr + step;
  }

  printf ("\n\n");

  /* for loop and Symbolic Constants */
  int f;
  for (f = LOWER; f <= UPPER; f = f + STEP)
    printf ("%3d %6.1f\n", f, (5.0 / 9.0) * (f - 32));

  return 0;
}
