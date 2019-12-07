/* Further information at:
 * https://developer.gnome.org/glib/2.26/glib-Arrays.html
 *
 */

#include <glib.h>
#include <stdbool.h>

gint
compare_func (gconstpointer a, gconstpointer b)
{
  if (GPOINTER_TO_INT (a) < GPOINTER_TO_INT (b))
    return 1;
  else if (GPOINTER_TO_INT (a) == GPOINTER_TO_INT (b))
    return 0;
  else
    return -1;
}

int
main ()
{
  GArray *garray;
  gint i;

  // create a new array to store gint values
  // first parameter: if zero terminated
  // second: if elements should be cleared to 0 when allocated
  garray = g_array_new (FALSE, FALSE, sizeof (gint));

  g_print ("write 999 to 1 in the array\n");
  for (i = 999; i > 0; i--)
    g_array_append_val (garray, i);

  g_print ("first 9 values are: ");
  for (i = 0; i < 10; i++) {
    g_print ("%d ", g_array_index (garray, gint, i));
  }

  g_print ("\n\nSort array\n");
  g_array_sort (garray, compare_func);

  g_print ("Insert 23 in position 3\n");
  gint tmp = 23;
  g_array_insert_val (garray, 3, tmp);

  g_print ("\nRemove position 0\nNow the first 9 values are: ");
  g_array_remove_index (garray, 0);

  for (i = 0; i < 10; i++) {
    g_print ("%d ", g_array_index (garray, gint, i));
  }
  g_print ("\n");

  g_array_free (garray, TRUE);

  return 0;
}
