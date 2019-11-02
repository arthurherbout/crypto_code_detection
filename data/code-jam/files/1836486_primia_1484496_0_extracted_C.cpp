#include<iostream>
#include<vector>
using namespace std;
int main()
{
  int T;
  cin >> T;
  for(int tc=1;tc<=T;++tc){
    int N;
    cin >> N;
    int S[N];
    bool out = false;
    
    for(int i = 0; i < N; ++i){
      cin >> S[i];
    }

    cout <<"Case #"<<tc<<":"<<endl;
    for(int b = 0; b < (1<<N); ++b){
      vector<int> v;
      for(int i = 0; i < N; ++i){
        if( b & (1<<i) ){
          v.push_back(S[i]);
        }
      }

      for(int bb = 0; bb < (1<<v.size()); ++bb){
        int sa = 0, sb = 0;
        vector<int> va,vb;
        for(int i = 0; i < v.size(); ++i){
          if( bb & (1<<i) ){
            sa += v[i];
            va.push_back(v[i]);
          }else{
            sb += v[i];
            vb.push_back(v[i]);
          }
          if( sa == sb && va.size() > 0 && vb.size() > 0 ){
            out = true;
            for(int i = 0; i < va.size(); ++i){
              if( i ) cout << ' ';
              cout << va[i];
            }
            cout << endl;
            for(int i = 0; i < vb.size(); ++i){
              if( i ) cout << ' ';
              cout << vb[i];
            }
            cout << endl;
          }
        }
        if( out ) break;
      }
      if(out)break;
    }
    if( !out ){
      cout << "Impossible" << endl;
    }
  }
  return 0;
}
