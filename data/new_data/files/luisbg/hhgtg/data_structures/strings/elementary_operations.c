/* Simplified versions of the Elementary string-processing operations
 *
 * (without error checking)
 *
 * */

#include <stdlib.h>
#include <stdio.h>


// Compute string length
int _strlen (char *a)
{
  int i;
  for (i = 0; a[i] != 0; i++)
    ;

  return i;
}

// Copy string b into a
void _strcpy (char *a, char*b)
{
  int i;
  for (i = 0; (a[i] = b[i]) != 0; i++)
    ;
}

// Compare strings
int _strcmp (char *a, char *b)
{
  int i;
  for (i = 0; a[i] == b[i]; i++)
    if (a[i] == '\0')
      return 0;

  return a[i] - b [i];
}

// Append string b int a
void _strcat (char *a, char *b)
{
  _strcpy (a + _strlen(a), b);
}


int main ()
{
  char *hello = (char *) malloc (16 * sizeof (char));
  char *world = (char *) malloc (7 * sizeof (char));
  _strcpy (hello, "hello ");
  _strcpy (world, "world!");

  printf ("strings: a: %s\nb:%s\n\n", hello, world); 
  printf ("length: %d, %d\n", _strlen (hello), _strlen (world));
  printf ("compare: %s\n\n", (_strcmp (hello, world)) == 0? "true": "false");

  _strcat (hello, world);
  printf ("after strcat: strings: a: %s\nb:%s\n", hello, world); 

  return 0;
} 
