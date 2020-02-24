#include<iostream>
#include<vector>
#include<algorithm>

using namespace std;

int test(int start,int end,vector<vector<int> > edges){
  //  cout<<"test "<<start<<' '<<end<<endl;
  int N = edges.size();
  int total = N;
  int sofar = 0;
  for (int i=start;i<=end;i++){
    if (edges[i].size()>0)
      for (int j=edges[i].size()-1;j>=0;j--){
	if (edges[i][j]<=end && edges[i][j]>i)
	  if (i!=start || edges[i][j] !=end){
	    total = min(test(i,edges[i][j],edges),total);
	    i=edges[i][j]-1;
	    break;
	  }
      }
    sofar++;
  }
  //  cout<<"total: "<<sofar<<' '<<total<<' '<<min(sofar,total)<<endl;
  return min(sofar,total);
}

vector<int> write1(int start,int end,int sval,int eval,int tot,vector<vector<int> > edges){
  int N = edges.size();
  int p=end-start+1;
  vector<int> res(p,-1);
  res[0]=sval;
  res[p-1]=eval;
  vector<int> taken(tot+1,0);
  taken[sval]=1;
  taken[eval]=1;
  for (int i=start;i<=end;i++){
    int a=res[i-start];
    if (a==-1){
      a=1;
      for (int k=1;k<=tot;k++){
	if (!taken[k]){
	  a=k;
	  taken[k]=1;
	  break;
	}
      }
      res[i-start]=a;
    }
    if (edges[i].size()>0)
      for (int j=edges[i].size()-1;j>=0;j--){
	if (edges[i][j]<=end && edges[i][j]>i)
	  if (i!=start || edges[i][j] !=end){
	    int b=res[edges[i][j]-start];
	    if (b==-1){
	      b=1;
	      for (int k=1;k<=tot;k++){
		if (!taken[k]){
		  b=k;
		  taken[k]=1;
		  break;
		}
	      }
	      res[edges[i][j]-start]=b;
	    }
	    vector<int> between = write1(i,edges[i][j],a,b,tot,edges);
	    for (int k=0;k<between.size();k++){
	      res[i+k-start]=between[k];
	    }
	    i=edges[i][j]-1;
	    break;
	  }
      }
  }
  return res;
}

int main(){
  int T,tt=1;
  cin>>T;
  while(T--){
    int N,M;
    cin>>N>>M;
    vector<vector<int> > edges(N);
    vector<pair<int,int> > edge(M);
    for (int i=0;i<M;i++){
      // starting point
      cin>>edge[i].second;
      edge[i].second--;
    }
    for (int i=0;i<M;i++){
      cin>>edge[i].first;
      // end point
      edge[i].first--;
      edges[edge[i].second].push_back(edge[i].first);
      edges[edge[i].first].push_back(edge[i].second);
    }
    for (int i=0;i<N;i++){
      sort(edges[i].begin(),edges[i].end());
    }
    sort(edge.begin(),edge.end());
    int total = test(0,N-1,edges);
    vector<int> res = write1(0,N-1,1,2,total,edges);
    cout<<"Case #"<<tt++<<": "<<total<<endl;
    for (int i=0;i<res.size();i++){
      cout<<res[i]<<' ';
    }
    cout<<endl;
  }
}
      

      
