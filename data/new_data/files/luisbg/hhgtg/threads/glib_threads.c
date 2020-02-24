/* Further information at:
 * https://developer.gnome.org/glib/stable/glib-Threads.html
 *
 */


#include <glib.h>
#include <glib/gprintf.h>
#include <stdio.h>


void *sensitive_function ();
void *concurrent_function (gpointer data);


typedef struct _Example
{
  GMutex mutex;
  gint counter;
} Example;

void *
sensitive_function (gpointer data)
{
  Example *e = data;
  g_mutex_lock (&e->mutex);

  e->counter++;
  g_printf ("Counter value: %d\n", e->counter);

  g_usleep (1000000);

  g_mutex_unlock (&e->mutex);

  return NULL;
}

void *
concurrent_function (gpointer data)
{
  while (1) {
    g_printf (".\n");
    g_usleep (250000);
  }

  return NULL;
}


int
main (int argc, char *argv[])
{
  GThread *thread1, *thread2, *thread3;
  Example *data = g_new (Example, 1);;

  data->counter = 0;
  g_mutex_init (&data->mutex);

  thread1 = g_thread_new ("1", sensitive_function, data);
  thread2 = g_thread_new ("2", sensitive_function, data);
  thread3 = g_thread_new ("3", concurrent_function, NULL);

  g_thread_join (thread1);
  g_thread_join (thread2);
  g_thread_join (thread3);

  return 0;
}
