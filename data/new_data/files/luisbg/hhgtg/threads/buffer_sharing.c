/* One thread writes to the buffer, the other reads from it */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

typedef struct Buffer
{
  pthread_mutex_t mutex;
  int index;
  int buffer[8];
} Buffer;

void * add_val (void *ptr) {
  Buffer *b = (Buffer *) ptr;
  int tmp;

  while (1) {
    pthread_mutex_lock (&b->mutex);
    if (b->index < 8) {
      tmp = random();
      b->buffer[b->index++] = tmp;
      printf ("add: %d\n", tmp);
    }
    pthread_mutex_unlock (&b->mutex);
    usleep (10000);
  }

  return NULL;
}

void * remove_val (void *ptr) {
  Buffer *b = (Buffer *) ptr;

  while (1) {
    pthread_mutex_lock (&b->mutex);
    while (b->index > 0) {
      printf ("remove: %d\n", b->buffer[--b->index]);
    }
    pthread_mutex_unlock (&b->mutex);
    sleep (1);
  }

  return NULL;
}

int main ()
{
  pthread_t read_thread, write_thread;
  Buffer *b = (Buffer *) malloc (sizeof (Buffer));
  b->index = 0;
  b->buffer[0] = 0;

  pthread_create (&read_thread, NULL, remove_val, (void *) b);
  pthread_create (&write_thread, NULL, add_val, (void *) b);

  pthread_join (read_thread, NULL);
  pthread_join (write_thread, NULL);

  return 0;
}
