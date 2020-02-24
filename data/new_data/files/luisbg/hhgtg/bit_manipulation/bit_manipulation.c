#include <stdio.h>
#include <stdbool.h>


// get the bit in position i
bool
getbit (int num, int i)
{
  return ((num & (1 << i)) != 0);
}

// set bit i to 1
int
setbit (int num, int i)
{
  return num | (1 << i);
}

// clear bit i to 0
int
clearbit (int num, int i)
{
  // return num ^ (1 << i);
  int mask = ~(1 << i);
  return num & mask;
}

// clear most significant bits after (and including) i
int
clearbits_msb (int num, int i)
{
  int mask = (1 << i) - 1;
  return num & mask;
}

// clear least significant bits before (and including) i
int
clearbits_lsb (int num, int i)
{
  int mask = ~((1 << (i + 1)) - 1);
  return num & mask;
}

int
updatebit (int num, int i, int v)
{
  int mask = ~(1 << i);
  return (num & mask) | (v << i);
}

int
updatebit_c (int num, int i, int v)
{
  if (v)
    return num |= (1 << i);     // set
  else
    return num &= ~(1 << i);    // clear
}

int
togglebit (int num, int i)
{
  return num ^= (1 << i);
}

int
main ()
{
  int c;
  unsigned int x = 15;          // 1111

  printf ("x = %d (1111)\n\n", 15);

  printf ("XOR\n");
  printf ("x ^ 0 = %d   (x)\n", x ^ 0);
  printf ("x ^ 1 = %d   ~(x)\n", x ^ 15);
  printf ("x ^ x = %d    (0)\n\n", x ^ x);

  printf ("AND\n");
  printf ("x & 0 = %d    (0)\n", x & 0);
  printf ("x & 1 = %d   (x)\n", x & 15);
  printf ("x & x = %d   (x)\n\n", x & x);

  printf ("OR\n");
  printf ("x | 0 = %d   (x)\n", x | 0);
  printf ("x | 1 = %d   (1)\n", x | 15);
  printf ("x | x = %d   (x)\n\n", x | x);

  printf ("NOT\n");
  printf ("~x = %d     ~(x)\n", ~x);
  for (c = 3; c >= 0; --c)
    printf ("get %d bit: %d\n", c, getbit (~x, c));

  printf ("\n\n");

  printf ("x = %d (1010)\n", x = 10);
  for (c = 3; c >= 0; --c)
    printf ("get %d bit: %d\n", c, getbit (x, c));

  printf ("\n");
  printf ("set bit %d to 1: %d\n", 2, x = setbit (x, 2));
  for (c = 3; c >= 0; --c)
    printf ("get %d bit: %d\n", c, getbit (x, c));

  printf ("\n");
  printf ("clear bit %d to 0: %d\n", 3, x = clearbit (x, 3));
  for (c = 3; c >= 0; --c)
    printf ("get %d bit: %d\n", c, getbit (x, c));

  printf ("\n");
  printf ("x = %d (1111)\n", x = 15);
  printf ("clear most significant bits through bit %d: %d\n", 2,
      x = clearbits_msb (x, 2));
  for (c = 3; c >= 0; --c)
    printf ("get %d bit: %d\n", c, getbit (x, c));

  printf ("\n");
  printf ("x = %d (1111)\n", x = 15);
  printf ("clear least significant bits through bit %d: %d\n", 1,
      x = clearbits_lsb (x, 1));
  for (c = 3; c >= 0; --c)
    printf ("get %d bit: %d\n", c, getbit (x, c));

  printf ("\n");
  printf ("x = %d (1111)\n", x = 15);
  printf ("update bit %d to 0: %d\n", 2, x = updatebit (x, 2, 0));
  printf ("update bit %d to 1: %d\n", 3, x = updatebit (x, 3, 1));
  for (c = 3; c >= 0; --c)
    printf ("get %d bit: %d\n", c, getbit (x, c));

  printf ("\n");
  printf ("x = %d (1111)\n", x = 15);
  printf ("update bit %d to 0 (C support): %d\n", 2, x = updatebit_c (x, 2, 0));
  printf ("update bit %d to 1 (C support): %d\n", 3, x = updatebit_c (x, 3, 1));
  for (c = 3; c >= 0; --c)
    printf ("get %d bit: %d\n", c, getbit (x, c));

  printf ("\n");
  printf ("toggle bit %d: %d\n", 1, x = togglebit (x, 1));
  printf ("toggle bit %d: %d\n", 3, x = togglebit (x, 3));
  for (c = 3; c >= 0; --c)
    printf ("get %d bit: %d\n", c, getbit (x, c));
}
