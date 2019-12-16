#include <stdlib.h>
#include <stdio.h>

typedef struct stack
{
  int n;
  struct stack *next;
} stack;

typedef struct tower
{
  stack *disks;
  int index;
} tower;


void add (tower * t, int n);
int pop (tower * t);
void move_disks (int n, tower * origin, tower * destination, tower * buffer);
void travel_towers (tower t[]);


void
add (tower * t, int n)
{
  stack *new = (stack *) malloc (sizeof (stack));
  new->n = n;
  new->next = t->disks;
  t->disks = new;
}

int
pop (tower * t)
{
  int value = -1;
  stack *tmp = NULL;

  if (t->disks) {
    value = t->disks->n;
    tmp = t->disks;
    t->disks = tmp->next;
    free (tmp);
  }

  return value;
}

void
move_disks (int n, tower * origin, tower * destination, tower * buffer)
{
  /* Base case */
  if (n <= 0)
    return;

  /* move top n-1 disks from origin to buffer, using destination as a buffer */
  move_disks (n - 1, origin, buffer, destination);

  /* move top from origin to destination */
  add (destination, pop (origin));
  tower tmp[3];
  tmp[0] = *origin, tmp[1] = *buffer, tmp[2] = *destination;
  travel_towers (tmp);

  /* move top n -1 disks from buffer to destinatoin, using origin as a */
  /* buffer */
  move_disks (n - 1, buffer, destination, origin);
}

void
travel_towers (tower t[])
{
  int c = 0;
  stack *head;
  for (; c < 3; c++) {
    head = t[c].disks;
    printf ("%d: ", c);
    while (head) {
      printf ("%d ", head->n);
      head = head->next;
    }
    printf ("\n");
  }

  printf ("\n");
}


int
main ()
{
  int disk_num = 5;
  int c;
  tower towers[3];

  for (c = 0; c < 3; c++) {
    towers[c].disks = NULL;
    towers[c].index = c;
  }

  for (c = disk_num; c > 0; c--) {
    add (&towers[0], c);
  }
  travel_towers (towers);

  move_disks (disk_num, &towers[0], &towers[2], &towers[1]);

  return 0;
}
