/*  The array can be viewed as a subset of the domain (e.g. {0, 1, 2, ..., n−1}),
 * where a 1 bit indicates a number in the set and a 0 bit a number not in the
 * set. This set data structure uses about n/w words of space, where w is the
 * number of bits in each machine word.
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define WORD_BITS 5    // for 32 bit
#define MOD (sizeof(uint) * 8 - 1)


/* initialize the bit vector */
uint *
vector_init (uint size)
{
  // array size is divided by 32 (since there are 32 bits per word)
  uint *vector;

  // size has to be at least 1 word
  if (size < 32)
    size = 32;

  // why would you need an empty set?
  if (!size)
    assert(!"helpful message");

  vector = (uint *) calloc (size >> WORD_BITS, sizeof (uint));

  return vector;
}

/* get value at position in vector. check if the set has the number */
uint
vector_get (uint *bitvector, uint pos)
{
  int word_num = (pos >> WORD_BITS);  // divide by 2 ^ WORD_BITS
  int bit_num = (pos & MOD);     // mod by (2 ^ WORD_BITS) - 1

  return bitvector[word_num] & (1 << bit_num);
}

/* set value at position in vector to True */
void
vector_set (uint *bitvector, uint pos)
{
  int word_num = (pos >> WORD_BITS);
  int bit_num = (pos & MOD);

  bitvector[word_num] |= (1 << bit_num);
}

/* set value at position in vector to False */
void
vector_unset (uint *bitvector, uint pos)
{
  int word_num = (pos >> WORD_BITS);
  int bit_num = (pos & MOD);

  bitvector[word_num] &= ~(1 << bit_num);
}


int main ()
{
  int i, r,
    set = 15,
    max = 30000;

  uint *vector = vector_init (max);

  // set positions
  for (i = 0; i < set; i++) {
    r = rand () % max;
    printf ("set number:  %5d\n", r);
    vector_set (vector, r);
  }

  // check what values the set has, print them all and unset some
  printf ("\n");
  for (i = 0; i < max; i++) {
    if (vector_get (vector, i)) {
      printf ("have in vector/set:  %5d\n", i);
      if (rand () % 3)
        vector_unset (vector, i);
    }
  }

  // print all values in set
  printf ("\nunset some\n\n");
  for (i = 0; i < max; i++) {
    if (vector_get (vector, i))
      printf ("have in vector/set:  %5d\n", i);
  }

  return 0;
}
