#include<iostream>
#include<string>
#include<map>
#include<vector>
#include<set>
#include<cmath>

using namespace std;

int prime[1001];
int sum_p;
int a[1001];
int P;


int find_prime()
{
  memset(prime,0,sizeof(prime));
  for (int i = 2; i < sqrt(1001); i++)
    if (prime[i] == 0)
    {
      int k = i;
      while (k+i < 1000)
      {
        k+=i;
        prime[k] = 1;
      }
  
    }

  sum_p = 0;
  for (int i = 2; i < 1000; i++)
    if (prime[i] == 0)
    {
      sum_p++;
      prime[sum_p-1] = i;
    }

  return 0;
}


bool small_prime(int A, int B)
{
  for (int i = sum_p-1; i >= 0; i--)
  {
    if (prime[i] < P)  
      break;
    if ((A % prime[i] == 0) && (B % prime[i] == 0))
    {
//      cout << A << " " << B << " " << prime[i] << endl;
      return true;
      break;
    }  
  }
  return false;
}

int main()
{
  freopen("b.in","r", stdin);
  freopen("b.out","w",stdout);

  int tt;
  cin >> tt;
  find_prime();
  int A, B;
  for (int t = 0; t < tt; t++)
  {
    cin >> A >> B >> P;
    for (int i = A; i <= B; i++)
      a[i] = i;

    bool ok = true;
    while (ok)
    {
      ok = false;
      for (int i = A; i <= B; i++)
        for (int j = i+1; j <= B; j++)
          if ((a[i] != a[j]) && (small_prime(i,j)))
          {
//              cout << i << " " << j << endl;
              int mini = min(a[i],a[j]);
              a[i] = mini;
              a[j] = mini;
//              a[j] = a[i];
              ok = true;
          }
    }
    /*
      for (int i = A; i <= B; i++)
        for (int j = i+1; j <= B; j++)
          if ((a[i] != a[j]) && (small_prime(i,j)))
          {
              cout << i << " " << j << endl;
              a[j] = a[i];
              ok = true;
          }
    */

    set<int> rez;
    for (int i = A; i <= B; i++)
      rez.insert(a[i]);
    cout << "Case #" << t+1 << ": " << rez.size() << endl; 
  }
  return 0; 
}
