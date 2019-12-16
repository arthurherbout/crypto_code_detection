#include <stdio.h>
//#include <stdint.h>
#include <stdbool.h>


bool
getbit (int num, int i)
{
  return ((num & (1 << i)) != 0);
}

int
update_bit (int num, int i, int v)
{
  if (v)
    num |= (1 << i);            // set
  else
    num &= ~(1 << i);           // clear

  return num;
}

int
contain_m_in_n (int m, int n, int j, int i)
{
  int c;
  int s = j - i;

  for (c = j; c >= i; c--) {
    n = update_bit (n, c, getbit (m, s));
    --s;
  }

  return n;
}

int
update_bits_directly (int m, int n, int j, int i)
{
  // Create a mask to clear bits i through j in n
  int all_ones = ~0;

  // 1's before position j, then 0's. 11100000
  int left = all_ones << (j + 1);

  // 1's after position i. 00000011
  int right = ((1 << i) - 1);

  // All 1's, except for 0's between i and j. 11100011
  int mask = left | right;

  // Clear bits j through i then put m in there
  int n_cleared = n & mask;
  int m_shifted = m << i;

  return n_cleared | m_shifted; // OR them and return
}

int
main ()
{
  int c;
  unsigned int n = 1024;
  unsigned int m = 16 + 2 + 1;
  int j = 6;
  int i = 2;

  printf ("n: ");
  for (c = 31; c >= 0; c--) {
    printf ("%d", getbit (n, c));
  }
  printf ("\nm: ");
  for (c = 31; c >= 0; c--) {
    printf ("%d", getbit (m, c));
  }
  printf ("\nj: %d", j);
  printf ("\ni: %d", i);

  n = contain_m_in_n (m, n, j, i);

  printf ("\nresult: ");
  for (c = 31; c >= 0; c--) {
    printf ("%d", getbit (n, c));
  }

  printf ("\n\n");

  n = 1024 + 512 + 256 + 128 + 64;
  m = 16 + 1;
  j = 5;
  i = 1;

  printf ("n: ");
  for (c = 31; c >= 0; c--) {
    printf ("%d", getbit (n, c));
  }
  printf ("\nm: ");
  for (c = 31; c >= 0; c--) {
    printf ("%d", getbit (m, c));
  }
  printf ("\nj: %d", j);
  printf ("\ni: %d", i);

  n = update_bits_directly (m, n, j, i);

  printf ("\nresult: ");
  for (c = 31; c >= 0; c--) {
    printf ("%d", getbit (n, c));
  }

  printf ("\n");
}
