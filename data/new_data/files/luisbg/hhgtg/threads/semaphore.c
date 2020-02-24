#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

/* prototypes for functions executed by threads */
void fill_buffer (int *);
void empty_buffer (int *);
void show_buffer ();

/* mutex to protect access to buffer */
pthread_mutex_t buffer_mutex;

/* semaphores for synchronizing fill_buffer and empty_buffer threads */
unsigned int threads_fill_done; /* count threads done filling buffer */
sem_t done_filling_buffer;        /* barrier to sync fill_buffer threads and empty_buffer threads */
sem_t filling_buffer;             /* to protect threads_fill_done */

/* Buffer to store values */
int buffer[4];

int
main ()
{
  int i;
  buffer[0] = 0;

  /* initialize mutex */
  pthread_mutex_init (&buffer_mutex, NULL);

  /* initialize semaphores */
  int res = sem_init (&done_filling_buffer,       /* pointer to semaphore */
      0,                        /* 0 if shared between threads, 1 if shared between processes */
      0);                       /* initial value for semaphore (0 is locked) */
  if (res < 0) {
    perror ("Semaphore initialization failed");
    exit (0);
  }
  if (sem_init (&filling_buffer, 0, 1)) { /* initially unlocked */
    perror ("Semaphore initialization failed");
    exit (0);
  }
  threads_fill_done = 0;


  pthread_t threads[11];        /* used by pthread_create to return thread id */
  int param[5] = { 0, 1, 2, 3, 4 };     /* array used to pass parameter to thread functions */

  for (i = 0; i < 5; i++) {
    /* creating 5 threads. Each thread enters one number (0-4) in the buffer */
    pthread_create (&threads[i],        /* pointer to buffer used by pthread_create to return thread id */
        NULL,                   /* pointer to thread attribute object */
        (void *) fill_buffer,     /* pointer to function to execute */
        (void *) &param[i]);    /* pointer to parameter to pass to function */
  }

  for (i = 5; i < 10; i++) {
    /* creating 5 threads. Each thread removes one number (0-4) from the buffer */
    pthread_create (&threads[i],
        NULL, (void *) empty_buffer, (void *) &param[i - 5]);
  }

  for (i = 0; i < 10; i++)
    pthread_join (threads[i], NULL);


  pthread_mutex_destroy (&buffer_mutex);
  sem_destroy (&filling_buffer);
  sem_destroy (&done_filling_buffer);

  return 0;
}

void
fill_buffer (int *value)
{
  int i;
  int num;

  /* using mutex before entering critical section */
  pthread_mutex_lock (&buffer_mutex);
  num = random ();
  printf ("Thread is inserting number %d in buffer: %d\n", *value, num);
  buffer[*value - 1] = num;
  pthread_mutex_unlock (&buffer_mutex);

  /* entering critical section with semaphore (could use mutex too) */
  sem_wait (&filling_buffer);     // blocks is semaphore 0. If semaphore nonzero,
  // it decrements semaphore and proceeds
  if (threads_fill_done == 4) {
    printf ("Done filling buffer. Lifting barrier for 5 empty_buffer threads.\n");
    for (i = 0; i < 5; i++)
      sem_post (&done_filling_buffer);    // sem_post increments semaphore. Incrementing it to 5
  } else {
    threads_fill_done++;
    sem_post (&filling_buffer);
  }
}

void
empty_buffer (int *value)
{
  int num;

  /* waiting for buffer to be filled up */
  printf
      ("Thread is waiting for semaphore to be released to remove %d from buffer.\n",
      *value);
  sem_wait (&done_filling_buffer);

  /* buffer filled. Removing elements */
  pthread_mutex_lock (&buffer_mutex);
  num = buffer[*value - 1];
  buffer[*value - 1] = 0;
  printf ("Thread is removing number %d from buffer: %d\n", *value, num);
  pthread_mutex_unlock (&buffer_mutex);
}


void
show_buffer (int *thread_num)
{
  pthread_mutex_lock (&buffer_mutex);
  printf ("Current buffer from thread %i:\n", *thread_num);
  pthread_mutex_unlock (&buffer_mutex);
}
