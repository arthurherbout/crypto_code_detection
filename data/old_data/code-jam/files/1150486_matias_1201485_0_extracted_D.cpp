#include <string>
#include <vector>
#include <algorithm>
#include <iostream>
#include <sstream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <queue>
#include <utility>

using namespace std;

typedef vector<int> VI;
typedef vector<VI> VVI;
typedef vector<string> VS;
typedef pair<int,int> PII;
#define FOR(i,a,b) for(int (i)=(a);(i)<(b);(i)++)
#define REP(i,n) FOR(i,0,n)
#define all(c) (c).begin(), (c).end()
#define tr(container,it) \
    for(typeof((container).begin()) it = (container).begin(); it!=(container).end(); it++)

int main(){
  int T,tt=1;
  cin>>T;
  while(T--){
    int V,E; //planets wormholes (vertices, edges)
    cin>>V>>E;
    vector<vector<int> > Graph(V,VI(0,0));
    int a,b;
    char ctp;
    REP(i,E){
      scanf("%d,%d",&a,&b);
      //      cin>>a>>ctp>>b;
      Graph[a].push_back(b);
      Graph[b].push_back(a);
    }
    REP(i,V){
      sort(all(Graph[i]));
    }
    VI dist(V,-1);
    dist[1]=0;
    queue<int> Q;
    Q.push(1);
    while(!Q.empty()){
      int cur=Q.front();
      if (cur==0) break;
      int d = dist[cur];
      Q.pop();
      REP(i,Graph[cur].size()){
	int b=Graph[cur][i];
	if (dist[b]==-1){
	  dist[b]=d+1;
	  Q.push(b);
	}
      }
    }
    while(!Q.empty()) Q.pop();
    VI threat(V,-1);
    threat[0]=Graph[0].size();
    REP(i,Graph[0].size()){
      if (dist[Graph[0][i]]==dist[0]-1){
	Q.push(Graph[0][i]);
      }
    }
    int glthreat=1;
    if (dist[0]==1)
      glthreat=threat[0]+2;
    while(!Q.empty()){
      int cur=Q.front();
      Q.pop();
      if (threat[cur]!=-1) continue;
      int maxthreat=0;
      REP(i,Graph[cur].size()){
	if ((dist[Graph[cur][i]]==(dist[cur]+1))){ //pred.
	  int candidate=Graph[cur][i];
	  if (threat[candidate]==-1) continue;
	  int curthreat=0;
	  REP(j,Graph[cur].size()){
	    if (!binary_search(all(Graph[candidate]),Graph[cur][j]))
	      curthreat++;
	  }
	  maxthreat=max(maxthreat,curthreat+threat[candidate]);
	}
	if (dist[Graph[cur][i]]==(dist[cur]-1))
	  Q.push(Graph[cur][i]);
      }
      threat[cur]=maxthreat-2;
      if (dist[cur]==1){
	glthreat=max(glthreat,maxthreat);
      }
    }
    /*
    REP(i,threat.size()){
      cout<<dist[i]<<' '<<threat[i]<<' ';
    }
    cout<<endl;
    */
    cout<<"Case #"<<tt++<<": "<<dist[0]-1<<' '<<glthreat-2<<endl;
  }
}
      

      
