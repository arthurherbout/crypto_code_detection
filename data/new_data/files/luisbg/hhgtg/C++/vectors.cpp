#include <iostream>
#include <vector>

using namespace std;

int main()
{
  vector<int> v;
  for (int c = 0; c < 5; c++)
    v.push_back(c + 1);

  cout << "vector has size: " << v.size() << endl;
  cout << "first element is: " << v[0] << endl;

  cout << "squaring all elements" << endl;
  for (auto &i : v)
      i *= i;

  cout << "full list is: \n";
  auto it = v.begin();
  auto end = v.end();
  while (it != end) {
    cout << *it << " ";
    it++;
  }
  cout << endl;

  cout << "remove third element" << endl;
  v.erase(v.begin() + 2);

  for (auto i: v)
    cout << i << " ";
  cout << endl;

  return 0;
}
