#include <iostream>
#include <vector>
#include <string>
#include <deque>
#include <queue>
#include <map>
#include <set>
using namespace std;

typedef pair<int,int> PII;

#define INF 1000000000
// 1e9 < INT_MAX/2

#define MP make_pair

int dx[4]={1,0,-1,0};
int dy[4]={0,1,0,-1};

void tos(){
  int N,M;
  cin>>N>>M;
  vector<string> DD(N);
  for(int i=0; i<N; i++){
    cin>>DD[i];
  }
  vector<vector<int> > D(N+2, M+2);
  vector<vector<int> > A(N+2, M+2);
  vector<vector<int> > F(N+2, M+2);
  priority_queue<pair<PII,PII> > pq;
  int Tcnt=0;
  for(int i=0; i<=N+1; i++){
    for(int j=0; j<=M+1; j++){
      F[i][j]=-1;
    }
  }
  for(int i=1; i<=N; i++){
    for(int j=1; j<=M; j++){
      if(DD[i-1][j-1]=='T'){
	pq.push(MP(MP(0,Tcnt++),MP(i,j)));
	D[i][j]=1;
      }else if(DD[i-1][j-1]=='#'){
	D[i][j]=1;
      }else{
	D[i][j]=0;
      }
    }
  }
  vector<int> uft(Tcnt);
  for(int i=0; i<Tcnt; i++){
    uft[i]=-1;
  }
  int res=0;
  while(!pq.empty()){
    PII data,pos;
    data=pq.top().first;
    pos=pq.top().second;
    pq.pop();
    int I,J;
    I=pos.first;
    J=pos.second;
    if(D[I][J]==0) continue;
    if(F[I][J]>=0){
      int aa=F[I][J];
      int bb=data.second;
      while(uft[aa]>=0) aa=uft[aa];
      while(uft[bb]>=0) bb=uft[bb];
      if(aa==bb) continue;
      else{
	if(uft[aa]<uft[bb]){
	  uft[aa]+=uft[bb];
	  uft[bb]=aa;
	} else{
	  uft[bb]+=uft[aa];
	  uft[aa]=bb;
	}
      }
      int d1=A[I][J]+1;
      int d2=-(data.first);
      res+=d1*d2;
      continue;
    }
    F[I][J]=data.second;
    res+= (A[I][J]= -(data.first));
    data.first--;
    for(int k=0; k<4; k++){
      pq.push(MP(data,MP(I+dx[k],J+dy[k])));
    }
  }
  /*
  cout<<endl;
  for(int i=1; i<=N; i++){
    for(int j=1; j<=M; j++){
      cout<<A[i][j];
    }
    cout<<endl;
  }
  */
  cout<<res<<endl;
}

int main(){
  int nCases;
  cin>>nCases;
  for(int c=1; c<=nCases; c++){
    cout<<"Case #"<<c<<": ";
    tos();
  }
  return 0;
}
