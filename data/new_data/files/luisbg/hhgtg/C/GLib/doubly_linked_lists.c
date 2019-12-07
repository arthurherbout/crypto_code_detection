/* Further information at:
 * https://developer.gnome.org/glib/stable/glib-Doubly-Linked-Lists.html
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
  GList *list = NULL;
  GList *second = NULL;
  GList *runner = NULL;

  printf ("append 4\n");
  list = g_list_append (list, GINT_TO_POINTER (4));
  printf ("append 5\n");
  list = g_list_append (list, GINT_TO_POINTER (5));
  printf ("prepend 2\n");
  list = g_list_prepend (list, GINT_TO_POINTER (2));
  printf ("prepend 1\n");
  list = g_list_prepend (list, GINT_TO_POINTER (1));
  printf ("insert 3 at pos 3\n");
  list = g_list_insert (list, GINT_TO_POINTER (3), 3);
  printf ("insert sorted 6\n\n");
  list = g_list_insert_sorted (list, GINT_TO_POINTER (6), compare_func);

  printf ("the list contains: ");
  runner = list;
  while (runner) {
    printf ("%d ", GPOINTER_TO_INT (runner->data));
    runner = g_list_next (runner);
  }

  printf ("\n\nremove 1\n");
  list = g_list_remove (list, GINT_TO_POINTER (1));
  printf ("remove 3\n");
  list = g_list_remove (list, GINT_TO_POINTER (3));

  printf ("now the list contains (in reverse): ");
  runner = g_list_last (list);
  while (runner) {
    printf ("%d ", GPOINTER_TO_INT (runner->data));
    runner = g_list_previous (runner);
  }

  printf ("\n\nappend 10 7 9 8 in a second list\n");
  second = g_list_append (second, GINT_TO_POINTER (10));
  second = g_list_append (second, GINT_TO_POINTER (7));
  second = g_list_append (second, GINT_TO_POINTER (9));
  second = g_list_append (second, GINT_TO_POINTER (8));

  printf ("sort second list\n");
  second = g_list_sort (second, compare_func);

  printf ("the second list contains: ");
  runner = second;
  while (runner) {
    printf ("%d ", GPOINTER_TO_INT (runner->data));
    runner = g_list_next (runner);
  }
  printf ("\n\n");

  printf ("concatenate the two lists\n");
  list = g_list_concat (list, second);

  printf ("the concat list now contains: ");
  for (int i = 0; i < 8; i++) {
    gpointer tmp = g_list_nth_data (list, i);
    printf ("%d ", GPOINTER_TO_INT (tmp));
  }
  printf ("\n");

  g_list_free (list);
  g_list_free (second);

  return 0;
}
