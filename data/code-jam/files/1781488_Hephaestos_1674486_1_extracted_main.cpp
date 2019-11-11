#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include <cmath>
#include <queue>


using namespace std;



int main(void){
 int T;
 cin>>T;
  for(int i=0;i<T;++i){
   int N;
   cin>>N;
   vector<vector<int> > g(N);
   for(int j=0;j<N;++j){
    int m;
    cin>>m;
    g[j] = vector<int>(m);
    for (int k=0;k<m;++k){
     cin>>g[j][k];
     --(g[j][k]); 
    }
   }
   vector<int> start_nodes;
   for (int j=0;j<N;++j){
    bool is_start = true;
    for (int k=0;k<N;++k){
     if (find(g[k].begin(),g[k].end(),j)!=g[k].end()){
      is_start=false;
      break;
     }
    }
    if(is_start)
     start_nodes.push_back(j);
   }

   
   bool diamond_found=false;
   for(auto it = start_nodes.begin();it!=start_nodes.end();++it){
    
    vector<int> visited;
    queue<int> to_visit;
    to_visit.push(*it);
    while(!to_visit.empty() && !diamond_found){
     int k=to_visit.front();
     to_visit.pop();
     for (int l=0;l<g[k].size();++l){
      int found;
      if (found=find(visited.begin(),visited.end(),g[k][l])==visited.end()){
       visited.push_back(g[k][l]);
       to_visit.push(g[k][l]);
      }
      else {
       diamond_found=true;
       break;
      }
     }
    }
    if(diamond_found)
     break;
   }
   cout<<"Case #"<<i+1<<": ";
   if (diamond_found)
    cout<<"Yes\n";
   else
    cout<<"No\n";
  }
return 0;}
//typedef long long ll;
//
//ll diff[500][500];
//
//int find_subset(const vector<ll>& A,const vector<ll> ref, vector<ll>& ans1, vector<ll>& ans2, ll& ans3){
// if (A.size()==2){
//  ll found;
//  if (found = find(ref.begin(),ref.end(),abs(A[1]-A[0]))!=ref.end()){
//   ans1.push_back(min(A[0],A[1]));
//   ans1.push_back(found);
//   ans2.push_back(max(A[0],A[1]));
//   ans3 = found;
//   return 1;
//  }
//  else if (found = find(ref.begin(),ref.end(),A[1]+A[0])!=ref.end()){
//   ans1.push_back(A[0]);
//   ans1.push_back(A[1]);
//   ans2.push_back(found);
//   ans3 = found;
//   return 2;
//  }
//  else{
//   return 0;
//  }
// }
// else{
//  vector<ll> sub(A.size()-1);
//  for (int i=0;i<sub.size();++i)
//   sub[i] = A[i];
//  vector<ll> test;
//  test.push_back(0);
//  test.push_back(A.back());
//  vector<ll> a1,a2;
//  ll a3;
//  int r = find_subset(sub,test,a1,a2,a3);
//  if (r==0){
//   return 0;
//  }
//  else if (a3==0)
//  else if (r==1) { //diff of subsets = a3, first is smallest
//   a3 = 0;
//  }
// }
//
//}
//
//int main3(void){
// int T;
// cin>>T;
// for (int i=0;i<T;++i){
//  int N;
//  cin>>N;
//  vector<ll> A(N);
//  for(int j=0;j<N;++j){
//   cin>>A[j];
//  }
//  for (int j=0;j<N;++j){
//   for(int k=j+1;k<N;++k) {
//    diff[j][k] = abs(A[j]-A[k]);
//   }
//  }
//
//  find_subset(
//
// }
//
//}
//
//
//int main2(void){
//
// int T;
// cin>>T;
// for (int i=0;i<T;++i){
//  int N;
//  cin>>N;
//
//  vector<long double> result(N);
//  vector<pair<long double,int>> s1(N);
//  double X=0;
//  for (int j=0;j<N;++j){
//   long double a;
//   cin>>a;
//   X+=a;
//   s1[j] = pair<long double,int>(a,j);
//  }
//  int X0 = X;
//  vector<long double> r(N);
//  sort(s1.begin(),s1.end());
//  vector<long double> s(N);
//  for (int j=0;j<N;++j){
//   s[j] = s1[j].first;
//  }
//  int n=1;
//  bool stop = false;
//  while (!stop){
//   n=count(s.begin(),s.end(),s[0]);
//   long double diff;
//   if (n<N){
//    diff = s[n]-s[0];
//   }
//   else {
//    diff = long double(X)/long double(n);
//    stop=true;
//   }
//   if (n*diff > X){    
//    for (int k=0;k<n;++k) {
//     X-=diff;
//     s[k]+=diff;
//     r[s1[k].second]+=diff;
//    }
//    break;
//   }
//   else {
//    for (int k=0;k<n;++k) {
//     X-=diff;
//     s[k]+=diff;
//     r[s1[k].second]+=diff;
//    }
//   }
//  }
//  
//
//  cout.precision(9);
//  cout<<fixed;
//  cout<<"Case #"<<i+1<<':';
//  for (int j=0;j<N;++j){
//   cout<< ' '<<100.*r[j]/long double(X0);
//   }
//  cout<<'\n';
// }
//
//return 0;}