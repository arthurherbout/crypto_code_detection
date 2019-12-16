// Store a sorted list containing the ages of costumers

#include <stdio.h>

#define LIST_SIZE 128           // reasonable age range is 0-127

// Since the range of ages is short we can use an array in which each
// item represents how many people have a certain age
int test_result[LIST_SIZE] = {
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 00
  0, 0, 0, 0, 0, 1, 0, 0, 0, 0, // 10
  0, 0, 0, 1, 1, 0, 0, 0, 0, 0, // 20
  0, 0, 0, 1, 0, 0, 0, 0, 0, 0, // 30
  0, 0, 0, 2, 0, 2, 0, 0, 0, 0, // 40
  0, 1, 0, 0, 0, 0, 0, 0, 0, 0, // 50
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 60
  0, 0, 0, 0, 0, 0, 1, 0, 0, 0, // 70
  0, 0, 1, 0, 0, 0, 0, 0, 0, 0, // 80
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 90
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 100
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 110
  0, 0, 0, 0, 0, 0, 0, 0        // 120
};

//0  1  2  3  4  5  6  7  8  9


int
add_person (int list[LIST_SIZE], int age)
{
  // increment the count of that age
  if (age >= 0 && age < 128) {
    list[age]++;

    return 0;
  }

  return -1;
}

void
print_list (int list[LIST_SIZE])
{
  printf ("\nAges list is: ");

  for (int i = 0; i < LIST_SIZE; i++) {
    for (int r = 0; r < list[i]; r++) {
      printf (" %d", i);
    }
  }

  printf ("\n");
}

int
test (int list[LIST_SIZE])
{
  for (int i = 0; i < LIST_SIZE; i++) {
    if (list[i] != test_result[i]) {
      printf ("FAIL: Different result at age %d\n", i);

      return -1;
    }
  }

  printf ("Test succeeded\n");
  return 0;
}

int
main ()
{
  int ages_list[LIST_SIZE] = { };
  int sample_input[12] = { 23, 33, 45, 24, 15, 76, 82, 43, 51, 43, 45, -12 };

  printf ("Original list: ");
  for (int i = 0; i < 12; i++) {
    printf (" %d", sample_input[i]);
  }
  printf ("\n");

  for (int i = 0; i < 12; i++) {
    if (add_person (ages_list, sample_input[i]) != 0) {
      printf ("Ignoring invalid input: %d\n", sample_input[i]);
    }
  }
  print_list (ages_list);

  test (ages_list);

  return 0;
}
