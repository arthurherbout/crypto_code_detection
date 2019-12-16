#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

int
main ()
{
  char c;
  FILE *fp;
  char filename[20];

  printf ("Enter the name of the file: ");      // Display prompt for input file
  scanf ("%s", filename);       // Read the user's response
  fp = fopen (filename, "rb");  // Open the file for input

  while (fread (&c, sizeof (char), 1, fp) != 0) // While there are chars to read
    fwrite (&c, 1, 1, stdout);  // Read a char from the file and print to screen

  fclose (fp);                  // Close the input file

  return 0;
}
