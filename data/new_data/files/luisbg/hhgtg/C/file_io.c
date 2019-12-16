#include <stdio.h>
#include <string.h>
#include <stdlib.h>


typedef struct
{
  char txt[32];
  int m;
  int n;
} example_struct;


void
write (char *file_path)
{
  FILE *fp;
  example_struct data[10];
  int i;
  // int w;

  fp = fopen (file_path, "w+b");
  if (fp == NULL) {
    perror ("Failed to open file to write");
    return;
  }

  for (i = 0; i < 10; i++) {
    /* fill data */
    sprintf (data[i].txt, "example %c", 'A' + i);
    data[i].m = i * 10;
    data[i].n = i * 20;

    /* write data */
    fwrite (&data[i], sizeof (example_struct), 1, fp);
    // printf ("wrote %d\n", w);  // w is the return of fwrite(), amount written
    printf ("position %ld\n", ftell (fp));
  }

  fclose (fp);
}

void
read (char *file_path)
{
  FILE *fp;
  example_struct data;
  int i;

  fp = fopen (file_path, "rb");
  if (fp == NULL) {
    perror ("Failed to open file to read");
    return;
  }

  for (i = 0; i < 5; i++) {
    /* read data */
    fread (&data, sizeof (example_struct), 1, fp);

    printf ("%d: %s :: %d\n", i, data.txt, data.m);
    printf ("position %ld\n", ftell (fp));
  }

  fclose (fp);
}

void
read_backwards (char *file_path)
{
  FILE *fp;
  example_struct data;
  int i;

  fp = fopen (file_path, "rb");
  if (!fp) {
    perror ("Unable to open file!");
    return;
  }

  for (i = 9; i >= 5; i--) {
    fseek (fp, i * sizeof (example_struct), SEEK_SET);
    fread (&data, sizeof (example_struct), 1, fp);

    printf ("%d: %s :: %d\n", i, data.txt, data.m);
    printf ("position %ld\n", ftell (fp));
  }

  fclose (fp);
}

int
main ()
{
  printf ("write data:\n");
  write ("sample_file");

  printf ("\n\nread data:\n");
  read ("sample_file");

  printf ("\n\nread data seeking backwards:\n");
  read_backwards ("sample_file");

  return 0;
}
