/* Further information at:
 * https://developer.gnome.org/glib/stable/glib-Hash-Tables.html
 *
 */

#include <glib.h>
#include <stdio.h>

int
main ()
{
  char *tmp;
  int size;
  GHashTable *table = g_hash_table_new (g_str_hash, g_str_equal);

  // insert into the table
  printf ("inserting 'a'\n");
  g_hash_table_insert (table, "a", "is for apple");
  printf ("inserting 'b'\n");
  g_hash_table_insert (table, "b", "is for ball");
  printf ("inserting 'c'\n");
  g_hash_table_insert (table, "c", "is for carrot");
  printf ("inserting 'd'\n");
  g_hash_table_insert (table, "d", "is for duck");
  // returns true if key did not exist
  if (!g_hash_table_insert (table, "e", "is for egg"))
    printf ("this should not happen\n");

  // contains returns true if key exists
  if (g_hash_table_contains (table, "a"))
    printf ("'a' is in the table\n\n");
  else
    printf ("this should not happen\n");

  // lookup returns value attacked to key, or NULL if not in table
  tmp = g_hash_table_lookup (table, "d");
  printf ("lookup for 'd' got: %s\n\n", tmp);

  size = g_hash_table_size (table);
  printf ("there are %d elements in the table\n\n", size);

  printf ("remove 'd'\n");
  // returns true if key existed
  g_hash_table_remove (table, "d");

  size = g_hash_table_size (table);
  printf ("now there are %d elements in the table\n\n", size);

  tmp = g_hash_table_lookup (table, "d");
  if (!tmp)
    printf ("lookup for 'd' got nothing\n");
  else
    printf ("this should not happen\n");

  printf ("delete the table\n");
  g_hash_table_destroy (table);

  return 0;
}
