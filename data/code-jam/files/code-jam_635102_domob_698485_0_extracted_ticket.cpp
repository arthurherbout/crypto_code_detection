/* CodeJam solution ticket in C++ by domob.  */

//#define NDEBUG

#include <cassert>
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <algorithm>
#include <stdint.h>

static const int MAX_P = 10;

int p;
int miss[1 << MAX_P];


/* Build up a tree of matches.  */

class MatchTree
{
  public:

    int round;
    int price;

    int teamA, teamB;
    MatchTree* treeA;
    MatchTree* treeB;

    ~MatchTree ()
    {
      if (round != 1)
        {
          delete treeA;
          delete treeB;
        }
    }

    int costForThis ()
    {
      assert (price == 1);

      /* Each team in this subtree may participate here!  So we have to buy
         the ticket, if one of the teams has missing number == 0.  If we don't
         buy the ticket, recurse with missing number for all those teams
         decremented.  */

      int cost = 0;

      if (!hasMinMiss (round))
        {
          cost = price;
          if (round != 1)
            {
              cost += treeA->costForThis ();
              cost += treeB->costForThis ();
            }
        }
      /*
      else
        {
          incMissing (-1);
          if (round != 1)
            {
              cost += treeA->costForThis ();
              cost += treeB->costForThis ();
            }
          incMissing (1);
        }*/
          
      return cost;
    }

    bool hasMinMiss (int minMiss)
    {
      if (round == 1)
        return miss[teamA] >= minMiss && miss[teamB] >= minMiss;

      return treeA->hasMinMiss (minMiss) && treeB->hasMinMiss (minMiss);
    }

    void incMissing (int add)
    {
      if (round == 1)
        {
          miss[teamA] += add;
          miss[teamB] += add;
          assert (miss[teamA] >= 0);
          assert (miss[teamB] >= 0);
        }
      else
        {
          treeA->incMissing (add);
          treeB->incMissing (add);
        }
    }

};


/* Solve a single case.  */

void
solve_case ()
{
  scanf ("%d", &p);
  for (int i = 0; i < (1 << p); ++i)
    scanf ("%d", &miss[i]);

  MatchTree** trees;
  int numTrees = (1 << (p - 1));
  int round = 1;

  trees = new MatchTree*[numTrees];
  for (int i = 0; i < numTrees; ++i)
    {
      trees[i] = new MatchTree ();
      trees[i]->round = round;
      trees[i]->teamA = 2 * i;
      trees[i]->teamB = 2 * i + 1;
      scanf ("%d", &trees[i]->price);
    }

  while (numTrees > 1)
    {
      int newTrees = numTrees / 2;
      MatchTree** ntrees;
      ntrees = new MatchTree*[newTrees];
      ++round;
      
      for (int i = 0; i < newTrees; ++i)
        {
          ntrees[i] = new MatchTree ();
          ntrees[i]->round = round;
          ntrees[i]->treeA = trees[2 * i];
          ntrees[i]->treeB = trees[2 * i + 1];
          scanf ("%d", &ntrees[i]->price);
        }

      delete[] trees;
      trees = ntrees;

      numTrees = newTrees;
    }

  printf ("%d", trees[0]->costForThis ());
  delete[] trees;
}


/* Main routine, handling the different cases.  */

int
main ()
{
  int n;

  scanf ("%d\n", &n);
  for (int i = 1; i <= n; ++i)
    {
      printf ("Case #%d: ", i);
      solve_case ();
      printf ("\n");
    }

  return EXIT_SUCCESS;
}
