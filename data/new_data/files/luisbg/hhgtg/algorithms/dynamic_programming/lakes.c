/* We have an array of elevations and it rains. */
/* We want to know the volume of water that gets trapped in. */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>


void
get_all_max (int *elevations, int *max_direction, int size, int direction)
{
  int i;
  int max = -1;

  if (direction) {              // get all max left
    for (i = 0; i < size; i++) {
      if (elevations[i] > max)
        max = elevations[i];
      max_direction[i] = max;
    }
  } else {                      // get all max right
    for (i = size - 1; i >= 0; i--) {
      if (elevations[i] > max)
        max = elevations[i];
      max_direction[i] = max;
    }
  }

  return;
}

int
volume_at_point (int *elevations, int *volumes, int *max_left,
    int *max_right, int size, int location)
{
  if (max_right[location] >= max_left[location])
    volumes[location] = max_left[location] - elevations[location];
  else
    volumes[location] = max_right[location] - elevations[location];

  return volumes[location];
}

int
array_volume (int *elevations, int *volumes, int size)
{
  int i;
  int sum = 0;

  int *max_left = (int *) malloc (size * sizeof (int));
  int *max_right = (int *) malloc (size * sizeof (int));
  get_all_max (elevations, max_left, size, 1);
  get_all_max (elevations, max_right, size, 0);

  for (i = 0; i < size; i++) {
    sum += volume_at_point (elevations, volumes, max_left, max_right, size, i);
  }

  free (max_left);
  free (max_right);

  return sum;
}


int
main ()
{
  int size = 20;
  int *elevations = (int *) malloc (size * sizeof (int));
  int *volumes = (int *) malloc (size * sizeof (int));

  int i, sum_volumes;

  srand(time(NULL));

  for (i = 0; i < size; i++) {  // populate elevations and init volumes
    elevations[i] = rand () % 10;
    volumes[i] = -1;
  }

  printf ("elevations: \t");    // iterate over elevations
  for (i = 0; i < size; i++) {
    printf ("%d ", elevations[i]);
  }
  printf ("\n");

  printf ("volumes: \t");       // iterate over volumes
  sum_volumes = array_volume (elevations, volumes, size);
  for (i = 0; i < size; i++) {
    printf ("%d ", volumes[i]);
  }
  printf ("\n");

  free (elevations);
  free (volumes);

  printf ("total volume: \t%d\n", sum_volumes);
}
