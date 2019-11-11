#include<iostream>
using namespace std;

int main()
{
  int T;
  cin >> T;
  for(int tc=1;tc<=T;++tc){
    int R,C;
    cin >> R >> C;
    char M[R][C];
    for(int i = 0; i < R; ++i){
      for(int j = 0; j < C; ++j){
        cin >> M[i][j];
      }
    }
    for(int i = 0; i < R-1; ++i){
      for(int j = 0; j < C-1; ++j){
        if( M[i][j] == '#' && M[i+1][j] == '#' &&
            M[i][j+1] =='#' &&M[i+1][j+1]=='#'){
          M[i][j] = '1';
          M[i+1][j+1]='1';
          M[i+1][j] ='2';
          M[i][j+1]='2';
        }
      }
    }
    bool impossible=false;
    for(int i = 0; i < R;++i){
      for(int j = 0; j < C; ++j){
        if( M[i][j] == '#' ) impossible =true;
      }
    }

    cout << "Case #" << tc << ":" <<endl;
    if(impossible)cout<<"Impossible"<<endl;
    else{
      for(int i = 0; i < R;++i){
        for(int j = 0; j < C; ++j){
          if( M[i][j] == '1'){
            putchar('/');
          }else if(M[i][j]=='2'){
            putchar('\\');
          }else{
            putchar(M[i][j]);
          }
        }
        putchar('\n');
      }
    }
  }
}
