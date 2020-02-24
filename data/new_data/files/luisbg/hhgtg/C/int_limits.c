#include <stdio.h>
#include <limits.h>

int main () {
  printf ("INT_MAX: \t%d\n", INT_MAX);
  printf ("UINT_MAX: \t%u\n", UINT_MAX);
  printf ("LONG_MAX: \t%ld\n", LONG_MAX);
  printf ("LLONG_MAX: \t%lld\n", LLONG_MAX);
  printf ("ULONG_MAX: \t%lu\n", ULONG_MAX);
  printf ("ULLONG_MAX: \t%llu\n\n", ULLONG_MAX);
  printf ("INT_MIN: \t%d\n", INT_MIN);
  printf ("LONG_MIN: \t%ld\n", LONG_MIN);
  printf ("LLONG_MIN: \t%lld\n", LLONG_MIN);
}
