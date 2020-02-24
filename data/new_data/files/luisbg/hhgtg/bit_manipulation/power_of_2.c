/* Is a number zero or a power two?
 *
 * As seen in fontconfig
 */

#include <stdio.h>

#define FC_IS_ZERO_OR_POWER_OF_TWO(x) (!((x) & ((x)-1)))


int
main ()
{
  int list_size = 20, i;

  for (i = 0; i < list_size; i++)
    printf ("%d %s zero or power of two\n", i,
            FC_IS_ZERO_OR_POWER_OF_TWO (i)? "*IS*": "isn't");

  return 0;
}
