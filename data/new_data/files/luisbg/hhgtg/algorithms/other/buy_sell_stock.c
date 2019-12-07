/* Given an array for which the ith element is the price of a given stock on
 * day i. Design an algorithm to find the maximum profit if you wereo only
 * permitted to complete at most one transaction (buy once and sell once).
 */

#include <stdio.h>

#define INT_MAX 2147483647

int
max_profit_one_transaction (int *prices, int pricesSize)
{
  int min_price_seen = prices[0];
  int max_profit = 0;

  for (int p = 1; p < pricesSize; p++) {
    if (prices[p] < min_price_seen)
      min_price_seen = prices[p];
    else if (prices[p] - min_price_seen > max_profit)
      max_profit = prices[p] - min_price_seen;
  }

  return max_profit;
}

int
max_profit_multiple_transactions (int *prices, int pricesSize)
{
  int sum = 0;
  int bought_at = prices[0];
  for (int p = 0; p < (pricesSize - 1); p++) {
    if (prices[p] < bought_at)  // if price lower than what we bought, we should buy here
      bought_at = prices[p];
    else {                      // meanwhile price goes up, keep and do nothing
      if (prices[p] > prices[p + 1]) {  // if price going down next, time to sell
        sum += prices[p] - bought_at;
        bought_at = prices[p + 1];      // buy at next price (which is lower than current)
      }
    }
  }

  // check if it there is a gain selling in last price
  if (prices[pricesSize - 1] > bought_at) {
    sum += prices[pricesSize - 1] - bought_at;
  }

  return sum;
}

int
main ()
{
  int stock_a[6] = { 7, 1, 5, 3, 6, 4 };
  int stock_b[5] = { 7, 6, 4, 3, 1 };
  int stock_c[7] = { 1, 3, 4, 6, 7, 9 };

  printf ("Max profit of the following stock day prices windows:\n[");
  for (int i = 0; i < 6; i++)
    printf ("%d, ", stock_a[i]);
  printf ("]\n   one transaction %d\n", max_profit_one_transaction (stock_a,
          6));
  printf ("   multiple transactions %d\n[",
      max_profit_multiple_transactions (stock_a, 6));

  for (int i = 0; i < 5; i++)
    printf ("%d, ", stock_b[i]);
  printf ("]\n   one transaction %d\n", max_profit_one_transaction (stock_b,
          5));
  printf ("   multiple transactions %d\n[",
      max_profit_multiple_transactions (stock_b, 6));

  for (int i = 0; i < 6; i++)
    printf ("%d, ", stock_c[i]);
  printf ("]\n   one transaction %d\n", max_profit_one_transaction (stock_c,
          7));
  printf ("   multiple transactions %d\n",
      max_profit_multiple_transactions (stock_c, 6));

  return 0;
}
