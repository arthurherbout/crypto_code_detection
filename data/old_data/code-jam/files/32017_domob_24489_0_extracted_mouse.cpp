#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>
#include <cassert>
#include <cstdlib>

typedef std::vector<unsigned> deck;

void solveCase ()
{
  unsigned k;
  unsigned cardsInDeck;
  std::cin >> k;

  // Construct perfect deck
  deck d;
  cardsInDeck = 0;
  for (unsigned c(k); c >= 1; --c)
    {
      unsigned modul (cardsInDeck + 1);
      unsigned pos;
      if (!cardsInDeck)
        pos = 0;
      else
        {
          pos = (c-1) % modul;
        }
      assert (c > 0);
      std::rotate (d.begin (), d.begin () + (d.size () - pos), d.end ());
      d.insert (d.begin () + pos, c);
      ++cardsInDeck;
    }
  assert (cardsInDeck == d.size ());

  // Output cards requested
  unsigned n;
  std::cin >> n;
  for (unsigned i(0); i != n; ++i)
    {
      unsigned x;
      std::cin >> x;
      --x;
      assert (x < d.size ());
      assert (d[x] > 0);
      std::cout << d[x] << " ";
    }
}

int main ()
{
  int n;
  std::cin >> n;

  for (int i(1); i <= n; ++i)
    {
      std::cout << "Case #" << i << ": ";
      solveCase ();
      std::cout << std::endl;
    }

  return EXIT_SUCCESS;
}
