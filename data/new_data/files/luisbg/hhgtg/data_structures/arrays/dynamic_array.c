#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

typedef struct
{
  int *array;
  int size;
  int len;
} dyn_arr;


/* naive push using malloc and copying data over when doubling size */
void
push (dyn_arr * da, int i)
{
  printf ("push: %d\n", i);

  if (da->len == da->size) {
    int new_size = da->size * 2;
    int *new_s = (int *) malloc (new_size * sizeof (int));
    int c;
    for (c = 0; c < new_size; c++) {
      if (c < da->len)
        new_s[c] = da->array[c];
      else
        new_s[c] = 0;
    }

    free (da->array);
    da->array = new_s;
    da->size = new_size;
  }

  da->array[da->len] = i;
  da->len++;

/*    printf("current state ");
    int a;
    for (a = 0; a < da->size; a++) {
        printf("%d ", da->array[a]);
    }
    printf("\n"); */
}

/* proper push using realloc */
void
push_realloc (dyn_arr * da, int i)
{
  printf ("push: %d\n", i);

  if (da->len == da->size) {
    da->size *= 2;
    da->array = (int *) realloc (da->array, da->size * sizeof (int));
  }
  da->array[da->len++] = i;
}

/* get last element pushed to the array */
bool
stack_pop (dyn_arr * da, int *ret)
{
  if (da->len == 0)
    return false;

  *ret = da->array[da->len - 1];
  da->array[da->len - 1] = 0;
  da->len = da->len - 1;

  // trim array if half empty
  if (da->size >= (da->len * 2)) {
    da->size /= 2;
    da->array = (int *) realloc (da->array, da->size * sizeof (int));
  }

  return true;
}

/* free the memory used by the array */
void
free_array (dyn_arr * da)
{
  free (da->array);
  da->array = NULL;
  da->size = da->len = 0;
}

/* initialize dynamic array */
void
init_dyn_arr (dyn_arr * new, int i)
{
  new->array = (int *) malloc (sizeof (int));
  new->array[0] = i;
  new->size = 1;
  new->len = 1;
}


int
main ()
{
  int c, t;
  dyn_arr a;

  printf ("initialize with 1\n");
  init_dyn_arr (&a, 1);

  for (c = 2; c <= 37; c++) {
    if (c % 2 == 0)
      push (&a, c);
    else
      push_realloc (&a, c * 10);
  }

  while (stack_pop (&a, &t))
      printf ("pop: %d\n", t);

  printf ("stack empty\n");

  free_array (&a);
  return 0;
}
