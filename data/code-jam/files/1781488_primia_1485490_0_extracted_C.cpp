#include<iostream>
#include<vector>
using namespace std;

typedef vector<pair<long long,int> > Line;

long long res;

void concat(Line &line)
{
  while(true){
    bool erased = false;
    for(int i = 0; i < (int)line.size()-1; ++i){
      if( line[i].second == line[i+1].second ){
        line[i].first += line[i+1].first;
        line.erase(line.begin()+i+1);
        erased = true;
        break;
      }
    }
    if( !erased ) break;
  }
}

void cur(int cur1, int cur2, Line &line1, Line &line2, long long sum){

  /*
  long long tsum = 0;
  for(int c = 1+cur1; c < (int)line1.size(); ++c){
    tsum += line1[c].first;
  }
  */
  // if( sum+tsum<=res ) return ;

  if( cur1 == (int)line1.size() || cur2 == (int)line2.size() ){
    res = max(res,sum);
    return ;
  }
  
  if( line1[cur1].second == line2[cur2].second ){
    long long make = min(line1[cur1].first,line2[cur2].first);
    if( line1[cur1].first > line2[cur2].first ){
      line1[cur1].first -= line2[cur2].first;
      cur(cur1,cur2+1,line1,line2,sum+make);
      line1[cur1].first += line2[cur2].first;
    }else if( line1[cur1].first < line2[cur2].first ){
      line2[cur2].first -= line1[cur1].first;
      cur(cur1+1,cur2,line1,line2,sum+make);
      line2[cur2].first += line1[cur1].first;
    }else{
      cur(cur1+1,cur2+1,line1,line2,sum+make);
    }
  }else{
    cur(cur1,cur2+1,line1,line2,sum);
    cur(cur1+1,cur2,line1,line2,sum);
  }
}

int main()
{
  int T;
  cin >> T;
  for(int tc=1;tc<=T;++tc){
    int N,M;
    cin >> N >> M;
    vector<pair<long long, int> > line1, line2;

    for(int i = 0; i < N; ++i){
      long long a;
      int A;
      cin >> a >> A;
      line1.push_back(make_pair(a,A));
    }
    for(int i = 0; i < M; ++i){
      long long b;
      int B;
      cin >> b >> B;
      line2.push_back(make_pair(b,B));
    }
    concat(line1);
    concat(line2);

    res = 0;
    cur(0,0,line1,line2,0);

    cout <<"Case #"<<tc<<": " << res << endl;
    
  }
  return 0;
}

