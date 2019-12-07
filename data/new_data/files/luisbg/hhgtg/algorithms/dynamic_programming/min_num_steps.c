/* On a positive integer, you can perform any one of the following 3 steps.
 * 1: Subtract 1 from it. ( n = n - 1 )
 * 2: If its divisible by 2, divide by 2. ( if n % 2 == 0 , then n = n / 2  )
 * 3: If its divisible by 3, divide by 3. ( if n % 3 == 0 , then n = n / 3  )
 *
 * Given a positive integer n, find the minimum number of steps that takes
 * n to 1
 *
 * examples:
 * For n = 1, output: 0
 * For n = 4, output: 2  (4 / 2 = 2; /2 = 1 )
 * For n = 7, output: 3  (7 - 1 = 6; /3 = 2; /2 = 1 )
 */

#include <stdio.h>
#include <stdlib.h>


int get_min_steps (int n, int *memo)        // Memoization
{
  int res;

  if (n == 1)
    return 0;                   // base case

  if (memo[n] != -1)
    return memo[n];             // we have solved it already

  res = 1 + get_min_steps (n - 1, memo);        // '-1' step.  'r' will contain
                                                //the optimal answer later

  if (n % 2 == 0) {
    if (res > (1 + get_min_steps (n / 2, memo)))
      res = 1 + get_min_steps (n / 2, memo);
  }

  if (n % 3 == 0) {
    if (res > (1 + get_min_steps (n / 3, memo)))
      res = 1 + get_min_steps (n / 3, memo);
  }

  memo[n] = res;                // save the result. avoid duplicating calculations

  return res;
}

int get_min_steps_bu (int n, int *dp)       // Bottom Up
{
  int c;

  dp[1] = 0;                    // bottom case

  for (c = 2; c <= n; c++) {
    dp[c] = 1 + dp[c - 1];

    if (c % 2 == 0) {
      if (dp[c] > (1 + dp[c / 2]))
        dp[c] = 1 + dp[c / 2];
    }

    if (c % 3 == 0) {
      if (dp[c] > (1 + dp[c / 3]))
        dp[c] = 1 + dp[c / 3];
    }
  }

  return dp[n];
}


int
main ()
{
  int num = 52;
  int *memo = (int *) malloc ((num + 1) * sizeof (int));
  int c;

  for (c = 0; c <= num; c++) {
    memo[c] = -1;               // initialize all elements to -1,
  }                             // which means not solved yet

  printf ("%d : %d\n", num, get_min_steps_bu (num, memo));

  return 0;
}
