/* CodeJam solution crossing in C++ by domob.  */

//#define NDEBUG

#include <cassert>
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <algorithm>
#include <queue>
#include <vector>

typedef int intT;

struct DijkstraEdge
{
  intT targetTime;
  intT target;

  inline DijkstraEdge (intT time, intT id)
    : targetTime (time), target (id)
  {}

};

/* PriorityQueue returns largest, so see if a should come after b.  */
static inline bool operator< (const DijkstraEdge& a, const DijkstraEdge& b)
{
  return a.targetTime > b.targetTime;
}

typedef std::priority_queue<DijkstraEdge> edgeQueue;
typedef std::vector<bool> knownArr;

static int rows, cols;

static const int MAX_NM = 21;
static int s[MAX_NM][MAX_NM], w[MAX_NM][MAX_NM], t[MAX_NM][MAX_NM];

static edgeQueue edges;
static knownArr alreadyKnown;


/* See when a crossing turns green in north-south or east-west.  Return
   either duration to wait or 0 if it already is.  All from a given
   time onwards.  */

static int
turnsGreen (int time, const int myS, const int myW, const int myT)
{
  // Bring to starting at t = 0.
  time -= myT;

  const int fullCycle = myS + myW;
  time %= fullCycle;
  if (time < 0)
    time += fullCycle;
  assert (time >= 0 && time < fullCycle);

  if (time < myS)
    return 0;
  return fullCycle - time;
}

static int
turnsGreenNS (int time, int r, int c)
{
  return turnsGreen (time, s[r][c], w[r][c], t[r][c]);
}

static int
turnsGreenEW (int time, int r, int c)
{
  return turnsGreen (time, w[r][c], s[r][c], t[r][c] + s[r][c]);
}


/* Encode/Decode into a vertex ID.
   Corners are 0..3 as north-east..north-west, clockwise.  */

static intT
vertexID (int r, int c, int corner, intT time)
{
  assert (corner >= 0 && corner < 4);
  assert (r >= 0 && r < rows);
  assert (c >= 0 && c < cols);
  assert (time >= 0);
  return corner + 4 * (c + cols * (r + rows * time));
}

static int
vertexCorner (intT id)
{
  return id % 4;
}

static int
vertexCol (intT id)
{
  return (id / 4) % cols;
}

static int
vertexRow (intT id)
{
  return (id / (4 * cols)) % rows;
}

static int
vertexTime (intT id)
{
  return id / (4 * cols * rows);
}


/* Push the edges reachable from a given vertex onto the queue.  */

static void
pushEdges (const int r, const int c, const int corner, const int time)
{
  #define PUSH_IT(ro, co, corn, tim) \
    edges.push (DijkstraEdge (tim, vertexID ((ro), (co), (corn), (tim))));

  // Going along the streets.
  #define GO_STREET(drow, dcol, targetCorner) \
    if (r + (drow) >= 0 && r + (drow) < rows \
        && c + (dcol) >= 0 && c + (dcol) < cols) \
      PUSH_IT (r + (drow), c + (dcol), (targetCorner), time + 2)
  switch (corner)
    {
      case 0:
        GO_STREET (0, 1, 3)
        GO_STREET (-1, 0, 1)
        break;
      case 1:
        GO_STREET (0, 1, 2)
        GO_STREET (1, 0, 0)
        break;
      case 2:
        GO_STREET (0, -1, 1)
        GO_STREET (1, 0, 3)
        break;
      case 3:
        GO_STREET (0, -1, 0)
        GO_STREET (-1, 0, 2)
        break;
      default:
        assert (false);
    }

  // Going NS or EW over the street.
  #define CROSS_STREET(targetCorner, dir) \
    PUSH_IT (r, c, (targetCorner), \
             time + 1 + turnsGreen##dir (time, r, c));
  switch (corner)
    {
      case 0:
        CROSS_STREET (1, NS)
        CROSS_STREET (3, EW)
        break;
      case 1:
        CROSS_STREET (0, NS)
        CROSS_STREET (2, EW)
        break;
      case 2:
        CROSS_STREET (3, NS)
        CROSS_STREET (1, EW)
        break;
      case 3:
        CROSS_STREET (2, NS)
        CROSS_STREET (0, EW)
        break;
      default:
        assert (false);
    }
}


/* Solve a single case.  */

static intT
solve_case ()
{
  std::cin >> rows >> cols;
  for (int r = 0; r < rows; ++r)
    for (int c = 0; c < cols; ++c)
      std::cin >> s[r][c] >> w[r][c] >> t[r][c];

  while (!edges.empty ())
    edges.pop ();
  alreadyKnown.clear ();

  pushEdges (rows - 1, 0, 2, 0);
  while (true)
    {
      assert (!edges.empty ());
      DijkstraEdge cur = edges.top ();
      edges.pop ();

      if (cur.target < alreadyKnown.size () && alreadyKnown[cur.target])
        continue;

      while (cur.target >= alreadyKnown.size ())
        alreadyKnown.push_back (false);

      assert (cur.target < alreadyKnown.size ());
      assert (!alreadyKnown[cur.target]);
      alreadyKnown[cur.target] = true;

      const int r = vertexRow (cur.target);
      const int c = vertexCol (cur.target);
      const int corner = vertexCorner (cur.target);
      const int time = cur.targetTime;
      assert (time == vertexTime (cur.target));

      if (r == 0 && c == cols - 1 && corner == 0)
        return cur.targetTime;

      pushEdges (r, c, corner, time);
    }
}


/* Main routine, handling the different cases.  */

int
main ()
{
  int n;

  scanf ("%d\n", &n);
  for (int i = 1; i <= n; ++i)
    printf ("Case #%d: %d\n", i, solve_case ());

  return EXIT_SUCCESS;
}
