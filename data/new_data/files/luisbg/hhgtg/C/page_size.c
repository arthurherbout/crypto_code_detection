// Ask the system what is the Page size

#include <stdio.h>
#include <unistd.h>             /* sysconf(3) */

int
main (void)
{
  /* _SC_PAGE_SIZE is OK too */
  printf ("The page size for this system is %ld bytes.\n",
      sysconf (_SC_PAGESIZE));

  return 0;
}
