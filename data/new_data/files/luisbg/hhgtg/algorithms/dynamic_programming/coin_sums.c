/* Inspired by Project Euler problem 31
 *
 * In England the currency is made up of pound, £, and pence, p, and there are
 * eight coins in general circulation:
 *
 * 1p, 2p, 5p, 10p, 20p, 50p, £1 (100p) and £2 (200p).
 * It is possible to make £2 in the following way:
 *
 * 1×£1 + 1×50p + 2×20p + 1×5p + 1×2p + 3×1p
 * How many different ways can £2 be made using any number of coins?
 *
 */

#include <stdio.h>


int dynamic_programming (int target, int size, int coins[size])
{
  int ways[target+1];
  int i, j;

  ways[0] = 1;
  for (i = 1; i <= target; i++)
    ways[i] = 0;

  for (i = 0; i < size; i++) {
    for (j = coins[i]; j <= target; j++) {
      ways[j] += ways[j - coins[i]];
    }
  }

  return ways[target];
}

int main ()
{
  int coins[8] = {1, 2, 5, 10, 20, 50, 100, 200};
  int target = 200;

  printf ("ways to break %d with british coins: \t%d\n", target,
          dynamic_programming (200, 8, coins));

  return 0;
}
