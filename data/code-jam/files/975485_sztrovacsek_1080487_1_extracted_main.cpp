#include <iostream>
#include <vector>
#include <fstream>

using namespace std;

int t;
int n;
fstream f;

struct Elem
{
    char c;
    int d;
};

vector<Elem> v;

void ReadInput()
{
  f >> n;
  v.clear();
  for (int i = 0; i < n; i++)
  {
      Elem e;
      f >> e.c;
      f >> e.d;
      v.push_back(e);
  }
}

int Process()
{
    int o_advantage = 0;
    int b_advantage = 0;
    int o_pos = 1;
    int b_pos = 1;
    int time = 0;
    for (vector<Elem>::iterator it = v.begin(); it != v.end(); it++)
    {
        Elem& e = *it;
        if (e.c == 'O')
        {
            int x = max(abs(e.d - o_pos) - o_advantage, 0) + 1;
            time += x;
            o_advantage = 0;
            b_advantage += x;
            o_pos = e.d;
        }
        else if (e.c == 'B')
        {
            int x = max(abs(e.d - b_pos) - b_advantage, 0) + 1;
            time += x;
            b_advantage = 0;
            o_advantage += x;
            b_pos = e.d;
        }
    }
    return time;
}

int main()
{
    f.open("input.txt");
    f >> t;
    for (int i = 0; i < t; i++)
    {
        cout << "Case #" << i+1 << ": ";
        ReadInput();
        int r = Process();
        cout << r << endl;
    }
    return 0;
}
