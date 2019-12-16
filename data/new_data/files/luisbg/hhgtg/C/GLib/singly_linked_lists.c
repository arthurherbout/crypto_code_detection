/* Further information at:
 * https://developer.gnome.org/glib/stable/glib-Singly-Linked-Lists.html
 *
 */

#include <glib.h>
#include <stdio.h>

gint
compare_func (gconstpointer a, gconstpointer b)
{
  if (GPOINTER_TO_INT (a) < GPOINTER_TO_INT (b))
    return -1;
  else if (GPOINTER_TO_INT (a) == GPOINTER_TO_INT (b))
    return 0;
  else
    return 1;
}

int
main ()
{
  GSList *list = NULL;
  GSList *second = NULL;
  GSList *runner = NULL;

  printf ("append 4\n");
  list = g_slist_append (list, GINT_TO_POINTER (4));
  printf ("append 5\n");
  list = g_slist_append (list, GINT_TO_POINTER (5));
  printf ("prepend 2\n");
  list = g_slist_prepend (list, GINT_TO_POINTER (2));
  printf ("prepend 1\n");
  list = g_slist_prepend (list, GINT_TO_POINTER (1));
  printf ("insert 3 at pos 3\n");
  list = g_slist_insert (list, GINT_TO_POINTER (3), 3);
  printf ("insert sorted 6\n\n");
  list = g_slist_insert_sorted (list, GINT_TO_POINTER (6), compare_func);

  printf ("the list contains: ");
  runner = list;
  while (runner) {
    printf ("%d ", GPOINTER_TO_INT (runner->data));
    runner = g_slist_next (runner);
  }

  printf ("\n\nremove 1\n");
  list = g_slist_remove (list, GINT_TO_POINTER (1));

  printf ("now the list contains: ");
  for (int i = 0; i < 5; i++) {
    gpointer tmp = g_slist_nth_data (list, i);
    printf ("%d ", GPOINTER_TO_INT (tmp));
  }

  printf ("\n\nappend 10 7 9 8 in a second list\n");
  second = g_slist_append (second, GINT_TO_POINTER (10));
  second = g_slist_append (second, GINT_TO_POINTER (7));
  second = g_slist_append (second, GINT_TO_POINTER (9));
  second = g_slist_append (second, GINT_TO_POINTER (8));

  printf ("sort second list\n");
  second = g_slist_sort (second, compare_func);

  printf ("the second list contains: ");
  runner = second;
  while (runner) {
    printf ("%d ", GPOINTER_TO_INT (runner->data));
    runner = g_slist_next (runner);
  }
  printf ("\n\n");

  printf ("concatenate the two lists\n");
  list = g_slist_concat (list, second);

  printf ("the concat list now contains: ");
  runner = list;
  while (runner) {
    printf ("%d ", GPOINTER_TO_INT (runner->data));
    runner = g_slist_next (runner);
  }
  printf ("\n");

  g_slist_free (list);
  g_slist_free (second);

  return 0;
}
