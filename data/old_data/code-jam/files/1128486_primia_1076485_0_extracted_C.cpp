#include<iostream>
#include<vector>
#include<bitset>
#include<set>
#include<map>
using namespace std;

#define MAX 100000013
 
vector<int> vprimes;
bool *isPrime = new bool[MAX];
 
/* ëfêîê∂ê¨ O(n^2) */
void MakePrime(){
  for(int i=0;i<MAX;++i)
    isPrime[i]=true;
  isPrime[0]=false;
  isPrime[1]=false;
  for(long long int i=2;i<MAX;++i)
    if(isPrime[i])
      for(long long int j=2;i*j<MAX;++j)
	isPrime[i*j]=false;
  for(int i = 0; i < MAX; ++i)
    if( isPrime[i] )
      vprimes.push_back( i );
}
 
// óÕãZÇ…ÇÊÇÈëfàˆêîï™â(10^12íˆìxÇ»ÇÁÇŸÇ⁄àÍèuÇ≈èoóàÇÈ)
template<typename Integer>
void Factorization(Integer n, vector<Integer> &vp){
  if(n==1){vp.push_back(1);return ;}
  for(int i = 0; i < (int)vprimes.size(); ++i){
    while(n%vprimes[i]==0){
      n/=vprimes[i];
      vp.push_back( vprimes[i] );
    }
    if(n==1)break;
  }
  if(n!=1)vp.push_back(n);
}

int main()
{
  int T;
  cin >> T;

  MakePrime();
  map<long long,int> ind;
  for(int i = 0; i < (int)vprimes.size(); ++i){
    ind[vprimes[i]]=i;
  }

  for(int tc=1;tc<=T;++tc){
    int N;
    long long L,H;
    cin >> N >> L >> H;

    vector< set<int> > ps(vprimes.size());
    set<long long> ap;

    int onecnt = 0;
    for(int i = 0; i < N; ++i){
      long long f;
      cin >> f;
      if(f==1){
        onecnt++;
        continue;
      }
      vector<long long> fact;
      Factorization(f,fact);
      for(int j = 0; j < (int)fact.size(); ++j){
        ap.insert( fact[j] );
        ps[ind[fact[j]]].insert(i);
      }
    }

    long long res = 0;
    bool overflow = false;

    bool onlythis = false;
    
    vector<long long> olv;
    for(int i = 0; i < (int)ps.size(); ++i){
      if( (int)ps[i].size() == N-onecnt ){
        olv.push_back( vprimes[i] );
      }
    }

    if( olv.size() > 0 ){
      res = 1;
      for(int i = 0; i < (int)olv.size(); ++i){
        if( res < L ){
          if( res * olv[i] < res ) overflow = true;
          res *= olv[i];
        }else break;
      }
      onlythis = true;
    }

    if( res == 0 || res < L && onlythis ){
      res = 1;
      onlythis = false;
      for(set<long long>::iterator it = ap.begin(); it != ap.end(); ++it){
        if( res * (*it) < res ) overflow = true;
        res *= *it;
      }
    }
    
    if( res < L && !overflow && !onlythis ){
      long long mul;
      if(( L-res )% res == 0 ){
        mul = ((L-res)/res);
      }else{
        mul = ((L-res)/res+1);
      }
      if( res * mul < res || res * mul + res < res ) overflow = true;
      res += res * mul;
    }

    if(L==1){
      overflow=false;
      res=1;
    }
    
    if( overflow || res<L || res > H ){
      cout << "Case #" << tc << ": NO" << endl;
    }else{
      cout << "Case #" << tc << ": " << res << endl;
    }
  }
  return 0;
}
