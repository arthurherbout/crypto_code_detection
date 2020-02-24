#include <bits/stdc++.h>

using namespace std;
typedef long long int ll;

#define MAX_M 10
#define MAX_N 10

map<int,int> vals;
struct Trie
{
  map<char,Trie> c;
  int size;
  int cCnt;
  Trie()
  {
    size = cCnt = 0;
    c.clear();
  }
  void insert(const string & s,int i)
  {
    if((int)s.size() <= i)
      return;
    if(c.count(s[i]) == 0)
      size++;
    cCnt++;
    c[s[i]].insert(s,i+1);
  }

  void remove(const string & s,int i)
  {
    if((int)s.size() <= i)
      return;
    cCnt--;
    c[s[i]].remove(s,i+1);
    if(c[s[i]].cCnt <= 0)
      {
	c.erase(s[i]);
	size--;
      }
  }

  int fullSize()
  {
    int ret = size;
    for(auto it = c.begin(); it != c.end();it++)
      ret += it->second.fullSize();
    return ret;
  }
};

Trie T[MAX_N];
int M,N;
string S[MAX_M];
int ans;
void gen(int cur,int used)
{
  if( cur >= M)
    {
      int tans = 0;
      for(int i = 0; i < N;i++)
	tans += T[i].fullSize()+1;
      vals[tans]++;

      return;
    }
  for(int i = 0; i < N;i++)
    {
      T[i].insert(S[cur],0);
      gen(cur+1,used | (1<<i));
      T[i].remove(S[cur],0);
    }
}
int main()
{
  int _T;
  scanf("%d",&_T);
  for(int _t = 1; _t <= _T;_t++)
    {
      scanf("%d %d",&M,&N);
      
      for(int i = 0; i < M;i++)
	cin >> S[i];
      for(int i = 0; i< N;i++)
	T[i] = Trie();
      vals.clear();
      gen(0,0);
      auto last = vals.end();
      last--;
      printf("Case #%d: %d %d",_t,last->first,last->second);
      printf("\n");
    }
  return 0;
}
