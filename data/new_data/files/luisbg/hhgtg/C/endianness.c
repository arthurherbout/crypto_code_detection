/* Returns true if the machine is little-endian, false if the
 * machine is big-endian
 */

#include <stdio.h>
#include <stdbool.h>


bool
endianness ()
{
  union {
    int num;
    char single_byte;
  } test;

  test.num = 1;
  return test.single_byte;                /* Returns the byte at the lowest address */
}


int
main ()
{
  printf ("Machine is %s Endian\n", endianness ()? "Little" : "Big");

  return 0;
}
