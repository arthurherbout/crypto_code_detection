#include <stdio.h>


int
check_player_wins (int player, int size, int board[size][size], int vic_num)
{
  int count;
  int max_count;

  // try horizontal lines
  for (int r = 0; r < size; r++) {
    count = 0;
    max_count = 0;
    for (int c = 0; c < size; c++) {
      if (board[r][c] == player)
        count++;
      else
        count = 0;

      if (count > max_count)
        max_count = count;
    }

    if (max_count == vic_num)
      return 1;
  }

  // try vertical lines
  for (int c = 0; c < size; c++) {
    count = 0;
    max_count = 0;
    for (int r = 0; r < size; r++) {
      if (board[r][c] == player)
        count++;
      else
        count = 0;

      if (count > max_count)
        max_count = count;
    }

    if (max_count == vic_num)
      return 1;
  }

  // try down-right diagonals
  for (int r = size - 1; r >= 0; r--) {
    count = 0;
    max_count = 0;
    int c = 0;
    int tmp_r = r;
    while (tmp_r != size) {
      // printf ("%d %d, ", tmp_r, c);
      if (board[tmp_r][c] == player)
        count++;
      else
        count = 0;

      if (count > max_count)
        max_count = count;

      tmp_r++;
      c++;
    }
    // printf ("\n");
    if (max_count == vic_num)
      return 1;
  }
  for (int c = 1; c < size; c++) {
    count = 0;
    max_count = 0;
    int r = 0;
    int tmp_c = c;
    while (tmp_c != size) {
      // printf ("%d %d, ", r, tmp_c);
      if (board[r][tmp_c] == player)
        count++;
      else
        count = 0;

      if (count > max_count)
        max_count = count;

      tmp_c++;
      r++;
    }
    // printf ("\n");
    if (max_count == vic_num)
      return 1;
  }

  // try up-right diagonals
  for (int r = 0; r < size; r++) {
    count = 0;
    max_count = 0;
    int c = 0;
    int tmp_r = r;
    while (tmp_r >= 0) {
      // printf ("%d %d, ", tmp_r, c);
      if (board[tmp_r][c] == player)
        count++;
      else
        count = 0;

      if (count > max_count)
        max_count = count;

      tmp_r--;
      c++;
    }
    // printf ("\n");
    if (max_count == vic_num)
      return 1;
  }
  for (int c = 1; c < size; c++) {
    count = 0;
    max_count = 0;
    int r = size - 1;
    int tmp_c = c;
    while (tmp_c < size) {
      // printf ("%d %d, ", r, tmp_c);
      if (board[r][tmp_c] == player)
        count++;
      else
        count = 0;

      if (count > max_count)
        max_count = count;

      tmp_c++;
      r--;
    }
    // printf ("\n");
    if (max_count == vic_num)
      return 1;
  }

  return 0;
}

// In this game, unlike classic Connect4, you can position anywhere in the
// board, instead of slide down each column.
int
try_all_empty (int player, int size, int board[size][size], int vic_num)
{
  for (int r = 0; r < size; r++) {
    for (int c = 0; c < size; c++) {
      if (board[r][c] == 0) {
        board[r][c] = player;

        if (check_player_wins (player, size, board, vic_num))
          return 1;

        board[r][c] = 0;
      }
    }
  }

  return 0;
}

int
main ()
{
  int test_case, T;
  int board_size, victory_num;

  setbuf (stdout, NULL);
  scanf ("%d", &T);

  for (test_case = 1; test_case <= T; test_case++) {
    // victory_num is the amount of pieces a player needs to have in a row
    // to win the game. Number has to be exact, == num, not > num
    scanf ("%d %d", &board_size, &victory_num);

    int board[board_size][board_size];
    for (int r = 0; r < board_size; r++) {
      for (int c = 0; c < board_size; c++) {
        // 1 = black piece (player A)
        // 2 = white piece (player B)
        // 0 = empty position
        scanf ("%d", &board[r][c]);
      }
    }

#if 0
    for (int r = 0; r < board_size; r++) {
      for (int c = 0; c < board_size; c++) {
        printf ("%d ", board[r][c]);
      }
      printf ("\n");
    }
#endif

    // 1 = player A wins
    // 2 = player B wins
    // -1 = tie
    if (check_player_wins (1, board_size, board, victory_num))
      printf ("#%d %d\n", test_case, 1);
    else if (try_all_empty (2, board_size, board, victory_num))
      printf ("#%d %d\n", test_case, 2);
    else
      printf ("#%d %d\n", test_case, -1);
  }

  return 0;
}
