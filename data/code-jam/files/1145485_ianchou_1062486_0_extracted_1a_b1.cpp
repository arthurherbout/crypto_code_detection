#include<iostream>
#include<vector>
using namespace std;
int main(void){
  int t=0,order[26];
  vector<string> d;
  cin>>t;
  for(int c=1;c<=t;++c){
    int n,m;
    cin>>n>>m;
    d.resize(n);
    for(int i=0;i<n;++i)
      cin>>d[i];
    cout<<"Case #"<<c<<":";
    for(;m>0;--m){
      char c;
      for(int i=0;i<26;++i){
        cin>>c;
        order[c-'a']=i;
      }
      int ans, score=-1;
      for(int i=0;i<n;++i){
        bool used[26]={0}, trial[26]={0};
        for(int j=0;j<d[i].size();++j)
          used[d[i][j]-'a']=1;
        for(int j=0;j<n;++j){
          if(i==j) continue;
          if(d[i].size()!=d[j].size()) continue;
          int broken=99, word;
          for(int k=0;k<d[i].size();++k){
            if(d[i][k]==d[j][k])
              continue;
            if(order[d[i][k]-'a']<broken){
              broken = order[d[i][k]-'a'];
              word = d[i][k]-'a';
            }
            if(order[d[j][k]-'a']<broken){
              broken = order[d[j][k]-'a'];
              word = d[j][k]-'a';
            }            
          }
          trial[word]=1;
        }
        int sum=0;
        for(int j=0;j<26;++j)
          if(trial[j] && !used[j])
            ++sum;
        if(sum>score){
          score=sum;
          ans=i;
        }
      }
      cout<<" "<<d[ans];
    }
    cout<<endl;
  }
  return 0;
}
