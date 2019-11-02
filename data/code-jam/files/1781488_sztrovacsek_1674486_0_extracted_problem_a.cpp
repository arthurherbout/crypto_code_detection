#include <iostream>
#include <vector>
#include <list>

using namespace std;

typedef vector<list<int> > Graph;

bool Szelessegi(const vector<list<int> >& graph, int K, int N)
{
  bool ret = false;
  // szelessegi bejarast inditunk K-bol.
  vector<int> v = vector<int>(N);
  for (int i = 0; i < N; i++)
  {
    v[i] = -1;
  }
  v[K-1] = 0;
  //cout << "v[K-1] = 0, K-1 = " << K-1 << endl; 
  list<int> q;
  q.push_back(K);
  while (! ret && (q.size() != 0))
  {
	int X = *q.begin(); 
	q.pop_front();
	//cout << "graph[X-1], X-1= " << X-1 << endl;
	//cout << "num neighbours: " << graph[X-1].size() << endl;
    list<int>::const_iterator it = graph[X-1].begin();
    while (!ret && (it != graph[X-1].end()))
    {
      int Y = *it;
	  //cout << "Y=" << Y << endl;
	  //cout << "v[Y-1], Y-1= " << Y-1 << endl;
      if (v[Y-1] == -1)
      {
        v[Y-1] = v[X-1] + 1;
        q.push_back(Y);
      }
      else
      {
        ret = true;
      }
      it++;
    }
  }
  return ret;
}

int main()
{
  int T;
  cin >> T;
  for (int t = 1; t <= T; t++)
  {
    int N;
    cin >> N;
	//cout << "N=" << N << endl;
    Graph graph = vector<list<int> >(N);
    for (int i = 0; i < N; i++)
    {
      int Mi;
      cin >> Mi;
	  //cout << "Mi=" << Mi << endl;
      graph[i] = list<int>();
      for (int j = 0; j < Mi; j++)
      {
        int c;
        cin >> c;
		//cout << "graph[i].pushback(c), c = " << c << endl;
        graph[i].push_back(c);
      }
    }
	//cout << "Beolvasas vege" << endl;
    bool contains = false;
    int i = 0;
    while (!contains && (i < N))
	{
	  //cout << "Szelessegi from "  << i+1 << endl;
      contains = Szelessegi(graph, i+1, N);
      i++;
    }
    cout << "Case #" << t << ": " << (contains ? "Yes" : "No") << endl;
  }
}