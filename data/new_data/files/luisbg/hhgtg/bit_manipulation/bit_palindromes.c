/* Inspired by Project Euler problem 36
 *
 * Find the sum of all numbers less than one million, which are palindromic in
 * base 10 and base 2 */

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#define BIN_SIZE 32


bool * in_binary(int n, bool * binary) {
  int i;
  for (i = 0; i < BIN_SIZE; i++) {
    binary[BIN_SIZE -1 -i] = (n & (1 << i)) != 0;
  }

  return binary;
}

bool is_palindrome_bin (bool * binary) {
  int i, j;

  i = 0;
  while (!binary[i])
    i++;

  j = BIN_SIZE - 1;
  while (i < j) {
    if (binary[i] != binary[j]) {
      return false;
    }

    i++;
    j--;
  }

  return true;
}

void print_binary (bool binary[BIN_SIZE]) {
  int i;
  for (i = 0; i < BIN_SIZE; i++)
    printf ("%d", (binary[i])? 1: 0);

  printf ("\n");
}

bool is_palindrome_n (int n) {
  int * digits = (int *) calloc (6, sizeof(int));
  int num_digits;
  int i, j;

  num_digits = 0;
  j = 0;
  i = n;
  while (i > 0) {
    digits[j++] = i % 10;
    num_digits++;

    i /= 10;
  }

  if (num_digits == 1) {
    free(digits);
    return true;
  }

  i = 0;
  j = num_digits-1;
  while (i < j) {
    if (digits[i] != digits[j]) {
      free(digits);
      return false;
    }

    i++;
    j--;
  }

  free(digits);
  return true;
}


int main ()
{
  int n, sum = 0;
  bool * binary = (bool *) malloc (BIN_SIZE * sizeof(bool));

  for (n = 1; n < 1000000; n++) {
    if (is_palindrome_n (n)) {
      binary = in_binary (n, binary);
      if (is_palindrome_bin (binary)) {
        printf ("%d: ", n);
        print_binary (binary);
        sum += n;
      }
    }
  }

  printf ("\nsum: %d\n", sum);

  free (binary);
  return 0;
}
