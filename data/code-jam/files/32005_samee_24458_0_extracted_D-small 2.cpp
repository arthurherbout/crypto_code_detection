#include<iostream>
#include<algorithm>
using namespace std;

int main()
{
  int ci,cn;
  cin>>cn;
  for(ci=1;ci<=cn;++ci)
  {
    int n,i,a,b,m,j;
    bool mat[10][10]={},sub[10][10]={};
    cin>>n;
    for(i=0;i<n-1;++i)
    { cin>>a>>b;
      --a; --b;
      mat[a][b]=mat[b][a]=true;
    }
    cin>>m;
    for(i=0;i<m-1;++i)
    { cin>>a>>b;
      --a; --b;
      sub[a][b]=sub[b][a]=true;
    }
    int ord[10];
    bool can=false;
    for(i=0;i<n;++i) ord[i]=i;
    do
    {
      for(i=0;i<m;++i) for(j=0;j<m;++j) 
        if(sub[i][j]&&!mat[ord[i]][ord[j]]) goto notthis;
      can=true;
      break;
notthis:;
    }while(next_permutation(ord,ord+n));
    cout<<"Case #"<<ci<<": ";
    if(can) cout<<"YES"<<endl;
    else cout<<"NO"<<endl;
  }
}
