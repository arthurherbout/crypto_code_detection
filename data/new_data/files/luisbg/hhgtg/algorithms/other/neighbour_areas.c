/* Convert all cells containing 0 into their biggest neighboring area value */

#include <stdlib.h>
#include <stdio.h>

typedef struct section
{
  int number;
  int size;
  int product;
} section;


section
biggest_neighbor_area_step (int x, int y, int map_size,
    int map[map_size][map_size], section areas[map_size][map_size])
{
  section big;
  big.size = -1;

  if (x != 0 && map[x - 1][y] != 0 && areas[x - 1][y].size > big.size)
    big = areas[x - 1][y];

  if (x != (map_size - 1) && map[x + 1][y] != 0
      && areas[x + 1][y].size > big.size)
    big = areas[x + 1][y];

  if (y != 0 && map[x][y - 1] != 0 && areas[x][y - 1].size > big.size)
    big = areas[x][y - 1];

  if (y != (map_size - 1) && map[x][y + 1] != 0
      && areas[x][y + 1].size > big.size)
    big = areas[x][y + 1];

  return big;
}

void
biggest_neighbor_area (int x, int y, int map_size,
    int map[map_size][map_size],
    section areas[map_size][map_size], section * biggest)
{
  int i, j, area_to_change;
  section tmp_section;

  area_to_change = areas[x][y].number;
  tmp_section.size = -1;

  for (i = 0; i < map_size; i++)
    for (j = 0; j < map_size; j++)
      if (areas[i][j].number == area_to_change) {
        tmp_section = biggest_neighbor_area_step (i, j, map_size, map, areas);
        if (tmp_section.size > biggest->size)
          *biggest = tmp_section;
      }
}

void
convert_to_biggest_neighbor (int x, int y, int map_size,
    int map[map_size][map_size], section areas[map_size][map_size])
{
  int i, j, area_num;
  section *biggest = (section *) malloc (sizeof (section));

  biggest->size = -1;

  biggest_neighbor_area (x, y, map_size, map, areas, biggest);
  free (biggest);

  area_num = areas[x][y].number;
  for (i = 0; i < map_size; i++)
    for (j = 0; j < map_size; j++)
      if (areas[i][j].number == area_num)
        map[i][j] = biggest->product;
}

void
generate_area (int x, int y, int map_size, int map[map_size][map_size],
    int visited[map_size][map_size], section areas[map_size][map_size],
    int area_num)
{
  // check we are inside the boundaries
  if (x < 0 || x >= map_size || y < 0 || y >= map_size)
    return;

  // if not visited already, add neighbors with same value
  if (!visited[x][y]) {
    visited[x][y] = 1;
    areas[x][y].number = area_num;
    areas[x][y].product = map[x][y];

    if (map[x - 1][y] == map[x][y])
      generate_area (x - 1, y, map_size, map, visited, areas, area_num);

    if (map[x + 1][y] == map[x][y])
      generate_area (x + 1, y, map_size, map, visited, areas, area_num);

    if (map[x][y - 1] == map[x][y])
      generate_area (x, y - 1, map_size, map, visited, areas, area_num);

    if (map[x][y + 1] == map[x][y])
      generate_area (x, y + 1, map_size, map, visited, areas, area_num);
  }
}

int
create_and_count_all_areas (int map_size, int map[map_size][map_size],
    int visited[map_size][map_size], section areas[map_size][map_size])
{
  int x, y;
  int area_counter = 0;

  for (y = 0; y < map_size; y++) {
    for (x = 0; x < map_size; x++) {
      if (!visited[x][y])
        generate_area (x, y, map_size, map, visited, areas, area_counter++);
    }
  }

  int *area_sizes = (int *) malloc (area_counter * sizeof (int));
  for (y = 0; y < map_size; y++) {
    for (x = 0; x < map_size; x++) {
      area_sizes[areas[x][y].number]++;
    }
  }

  for (y = 0; y < map_size; y++) {
    for (x = 0; x < map_size; x++) {
      areas[x][y].size = area_sizes[areas[x][y].number];
    }
  }

  free (area_sizes);

  return area_counter;
}

void
print_areas (int map_size, int map[map_size][map_size])
{
  int x, y;

  for (y = 0; y < map_size; y++) {
    for (x = 0; x < map_size; x++) {
      printf ("%d ", map[x][y]);
    }

    printf ("\n");
  }
}

// Run with: $ cat sample_input_neighbour_areas | neighbour_areas
int
main ()
{
  int test_case, T;

  scanf ("%d", &T);

  for (test_case = 0; test_case < T; test_case++) {
    int map_size;
    int x, y;
    int num_areas;

    scanf ("%d", &map_size);

    int map[map_size][map_size];
    int visited[map_size][map_size];
    section all_areas[map_size][map_size];

    for (y = 0; y < map_size; y++) {
      for (x = 0; x < map_size; x++) {
        scanf ("%d", &map[x][y]);
        visited[x][y] = 0;
      }
    }

    printf ("Case %d\nOriginal areas:\n", test_case);
    print_areas (map_size, map);

    create_and_count_all_areas (map_size, map, visited, all_areas);

    // reset visited array to 0 and set zeros to biggest neighboring area
    for (y = 0; y < map_size; y++) {
      for (x = 0; x < map_size; x++) {
        if (map[x][y] == 0) {
          convert_to_biggest_neighbor (x, y, map_size, map, all_areas);
        }
      }
    }

    // count final number of areas
    for (y = 0; y < map_size; y++) {
      for (x = 0; x < map_size; x++) {
        visited[x][y] = 0;
      }
    }

    printf ("\nAfter converting 0's to biggest neighbour:\n");
    print_areas (map_size, map);

    num_areas = create_and_count_all_areas (map_size, map, visited, all_areas);
    printf ("\nTotal number of areas: %d\n\n", num_areas);
  }

  return 0;
}
