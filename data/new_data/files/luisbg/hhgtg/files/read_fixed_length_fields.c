/*
 * Reader counterpart for write_fixed_length_fields.c
 */

#include <stdio.h>

typedef struct
{
  char last[20];
  char first[16];
  char address[32];
  char city[16];
  char zip[8];
} set_of_fields;


int
main ()
{
  char filename[20];
  FILE *fp;
  set_of_fields person;
  int c = 0;

  printf ("Enter the name of the file: ");
  scanf ("%s", filename);
  fp = fopen (filename, "rb");

  while (fread (&person, sizeof (set_of_fields), 1, fp) != 0) {
    printf ("%d| %s %s | %s\n", c, person.first, person.last, person.city);
    c++;
  }

  return 0;
}
