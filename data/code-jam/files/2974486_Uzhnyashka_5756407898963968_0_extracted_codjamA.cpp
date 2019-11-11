#include <iostream>
#include <cstdio>

using namespace std;


int t, t1, t2, x, a[10], b[10], kol, ans, qq[1000];


int main()

{
    freopen("input.txt","r",stdin);
    freopen("output.txt","w",stdout);
    cin>>t;
    for (int tt = 0; tt < t; tt++)
    {
        cin>>t1;
        for (int i = 1; i <= 16; i++)
            qq[i] = 0;
        for (int i = 1; i <= 4; i++)
            for (int j = 1; j <= 4; j++)
            {
                cin>>x;
                if (i == t1) {
                    qq[x]++;
                }
            }
        cin>>t2;
        for (int i = 1; i <= 4; i++)
            for (int j = 1; j <= 4; j++)
            {
                cin>>x;
                if (i == t2) {
                    qq[x]++;
                }
            }
            kol = 0;
            ans = 0;
      for (int i = 1; i <= 16; i++)
      {
          if (qq[i] == 2) {kol++; ans = i;}
      }
      cout<<"Case #"<<tt+1<<": ";
      if (kol == 1) cout<<ans<<endl;
      else if (kol > 1) cout<<"Bad magician!"<<endl;
      else if (kol == 0) cout<<"Volunteer cheated!"<<endl;
    }
}
