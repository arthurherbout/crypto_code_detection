#include <set>
#include <string>
#include <iostream>
#include <queue>

using namespace std;

queue<string> q;
set<string> S;

void gen(string s, string t="", int i=0, int jj=0){
  if (t.size()==0)
    t = string(s.size()-1, '.');
  if (i==s.size()){
    if (S.find(t)==S.end()){
      S.insert(t);
      q.push(t);
    }
    return;
  }
  if (s[i]=='0'){
    gen(s, t, i+1, 0);
  }
  int cnt=s[i]-'0';
  int num=0;
  for (int j=jj;j<t.size();++j)
    if (t[j]=='.')
      ++num;
  s[i] -= 1;
  for (int j=jj;j<t.size();++j){
    if (num<cnt)
      break;
    if (t[j]=='.'){
      t[j]='0'+i;
      gen(s, t, i, j+1);
      t[j]='.';
      --num;
    }
  }
}

int main(){
  int tnum;cin>>tnum;
  for (int ti=0;ti<tnum;++ti){
    string s;cin>>s;
    S.clear();
    q.push(s);S.insert(s);
    while (!q.empty()){
      s = q.front();q.pop();
      string t="0"+s;
      int sum=s.size();
      for (int i=0;i<s.size();++i)
        sum -= (s[i]-'0');
      if (sum<0)
        continue;
      t[0]='0'+sum;
      gen(t);
    }
    cout<<"Case #"<<ti+1<<": "<<S.size()<<endl;
  }
  return 0;
}
