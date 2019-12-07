#include <stdio.h>
#include <pthread.h>

#define ARRAYSIZE 17
#define NUMTHREADS 4

struct ThreadData
{
  int start, stop;
  int *array;
};

/* puts i^2 into array positions i=start to stop-1 */
void *
squarer (void *td)
{
  struct ThreadData *data = (struct ThreadData *) td;
  int start = data->start;
  int stop = data->stop;
  int *array = data->array;
  int i;

  for (i = start; i < stop; i++) {
    array[i] = i * i;
  }

  return NULL;
}

int
main (void)
{
  pthread_t thread[NUMTHREADS];
  int array[ARRAYSIZE];
  struct ThreadData data[NUMTHREADS];
  int i, error;
  /* this has the effect of rounding up the number of tasks
   * per thread, which is useful in case ARRAYSIZE does not
   * divide evenly by NUMTHREADS. */
  int tasksPerThread = (ARRAYSIZE + NUMTHREADS - 1) / NUMTHREADS;

  /* Divide work for threads, prepare parameters */
  for (i = 0; i < NUMTHREADS; i++) {
    data[i].start = i * tasksPerThread;
    data[i].stop = (i + 1) * tasksPerThread;
    data[i].array = array;
  }
  /* the last calculation must not go past the end of the array */
  data[NUMTHREADS - 1].stop = ARRAYSIZE;

  /* Launch threads */
  for (i = 0; i < NUMTHREADS; i++) {
    error = pthread_create (&thread[i], NULL, squarer, &data[i]);
    if (error)
      fprintf (stderr, "Unable to create thread\n");
  }

  /* Wait for threads to finish */
  for (i = 0; i < NUMTHREADS; i++) {
    error = pthread_join (thread[i], NULL);
    if (error)
      fprintf (stderr, "Unable to join thread\n");
  }

  /* Display Result */
  for (i = 0; i < ARRAYSIZE; i++) {
    printf ("%d ", array[i]);
  }
  printf ("\n");

  return 0;
}
