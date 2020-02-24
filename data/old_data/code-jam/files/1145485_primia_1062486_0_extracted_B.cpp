#include<iostream>
#include<string>
#include<vector>
#include<list>
#include<algorithm>

using namespace std;

int main()
{
  int T;
  cin >> T;
  for(int tc=1;tc<=T;++tc){
    int N, M;
    vector<string> D;
    cin >> N >> M;

    int lose[N];
    for(int i = 0; i < N; ++i){
      string word;
      cin >> word;
      D.push_back( word );
      lose[i] = 0;
    }

    sort(D.begin(),D.end());
    cout << "Case #" << tc << ":";
    for(int i = 0; i < M; ++i){
      string L;
      cin >> L;
      for(int j = 0; j < N; ++j){
        string W = D[j];
        bool reveal[W.length()];
        bool creveal[128];
        fill(reveal,reveal+W.length(),false);
        fill(creveal,creveal+128,false);

        list<string> cand;
        for(int k = 0; k < N; ++k){
          if( D[k].length() == W.length() ){
            cand.push_back(D[k]);
          }
        }
        
        for(int k = 0; k < (int)L.length(); ++k){
          bool found = false;
          if( cand.size() == 1 ){
            break;
          }
          for(int l = 0; l < (int)W.length(); ++l){
            if( W[l] == L[k] ){
              reveal[l]=true;
              found=true;
            }
          }
          if(!found){
            lose[j]++;
          }
          creveal[(int)L[k]]=true;
          for(list<string>::iterator itl = cand.begin(); itl != cand.end(); ++itl){
            string word = *itl;
            for(int l = 0; l < (int)word.length(); ++l){
              if( reveal[l] && W[l] != word[l] ||
                  cand.size()==2 && creveal[(int)word[l]] && !reveal[l] ){
                itl = cand.erase( itl );
                if(itl!=cand.begin())--itl;
                break;
              }
            }
          }
        }
      }
      //cout << L << endl;
      int maxlose = 0;
      for(int j = 0; j < N; ++j){
        maxlose = max( maxlose, lose[j] );
      }
      for(int j = 0; j < N; ++j){
        if( maxlose == lose[j] ){
          cout << ' ' << D[j];
          break;
        }
      }
    }
    cout << endl;
  }
  return 0;
}
