#include <iostream>
#include <list>

using namespace std;

int
main ()
{
  list<int> odd;

  if (odd.empty ())
    cout << "odds is empty. adding numbers to it" << endl;

  for (int c = 3; c < 20; c += 2) {
    cout << "add " << c << " to odd" << endl;
    odd.push_back (c);
  }

  cout << endl << "odd has size: " << odd.size () << endl;
  cout << "first element is: " << *(odd.begin ()) << endl;
  cout << "last element is: " << *(--odd.end ()) << endl;

  cout << "full list is: \n";
  auto it = odd.begin ();
  auto end = odd.end ();
  while (it != end) {
    cout << *it << " ";
    it++;
  }
  cout << endl << endl;

  cout << "remove front and back" << endl;
  odd.pop_front ();
  odd.pop_back ();

  it = odd.begin ();
  while (it != end) {
    cout << *it << " ";
    it++;
  }
  cout << endl << endl;

  cout << "remove third element" << endl;
  auto third = odd.begin ();
  third++;
  third++;
  odd.erase (third);

  for (auto i:odd)
    cout << i << " ";
  cout << endl;

  odd.clear ();

  return 0;
}
