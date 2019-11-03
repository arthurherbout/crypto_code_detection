#include<iostream>
#include<cstdio>
#include<vector>
#include<set>
#include<algorithm>
using namespace std;


int main()
{
    int test_count;
    cin>>test_count;
    for(int test_num=0;test_num<test_count;test_num++)
    {
            int N;
            scanf("%d",&N);
            vector<int> q;
            for(int i=0;i<N;i++)
            {
                    int x;
                    scanf("%d",&x);
                    q.push_back(x);
            }
            sort(q.begin(),q.end());
            int res;
            if (N==0)
              res=0;
            else
            {
                res = 10000;
                multiset<int> mas[2];
                int p = 0;
                for(int x=1;x<=10001;x++)
                {
                        for(;p<N && q[p]==x;p++)
                        {
                                  if (mas[1-(x&1)].empty())
                                    mas[x&1].insert(1);
                                  else
                                  {
                                      multiset<int>::iterator it = mas[1-(x&1)].begin();
                                      mas[x&1].insert(1+*it);
                                      mas[1-(x&1)].erase(it);
                                  }
                        }
                        for(multiset<int>::iterator it = mas[1-(x&1)].begin(); it!=mas[1-(x&1)].end();it++)
                          if (*it < res)
                            res = *it;
                        mas[1-(x&1)].clear();              
                }
            }
            printf("Case #%d: %d\n",test_num+1,res);
    }
    
    
    return 0;
}
