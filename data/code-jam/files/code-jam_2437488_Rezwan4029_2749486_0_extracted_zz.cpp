/*
    Rezwan_4029
    AUST , CSE-25
*/

#include<stdio.h>
#include<iostream>
#include<cstring>
#include<string>
#include<cctype>
#include<vector>
#include<algorithm>
#include<cmath>
#include<map>
#include<sstream>
#include<set>
#include<queue>
#define pb push_back
#define INF 1<<30
#define CS printf("Case #%d: ",cs++)
#define ms(a,v) memset(a,v,sizeof a)
#define II ({int a; scanf("%d", &a); a;})
#define LL ({ll  a; scanf("%lld", &a); a;})
#define FRD freopen("in.txt", "r", stdin)
#define FWR freopen("out.txt", "w", stdout)
using namespace std;
typedef  long long ll;

#define rep(i,n) for( __typeof(n)i = 0 ; i < n ; i++)
#define For(i,n) for( __typeof(n)i = 1 ; i <= n ; i++)
#define forstl(i,n) for(__typeof(n.begin())i = n.begin();i!=n.end();i++)
#define forab(i,a,b)for(__typeof(b)i = a ; i <= (b) ; i++)

int cs = 1 ;

string toString(ll number){ stringstream ss;ss << number; return ss.str(); }
bool isVowel(char s){s=tolower(s); if(s=='a'||s=='e'||s=='i'||s=='o'||s=='u')return true;return false;}
ll  GCD(ll  a,ll  b){ while(b){b ^= a ^=b ^= a %=b;}  return a;}
ll  LCM(ll  a,ll  b) { return (a / GCD(a,b)*b); }
ll  Pow(ll  B,ll  P){ll  R=1; while(P>0)  {if(P&1) R=(R*B);P/=2;B=(B*B);}return R;} //compute b^p
ll  BigMod(ll  B,ll  P,ll  M){ ll  R=1; while(P>0)  {if(P&1){R=(R*B)%M;}P/=2;B=(B*B)%M;} return (ll )R;} //compute b^p%m
ll  modInv(ll x , ll m ){ return BigMod(x,m-2,m);}

int main(){
    FRD ; FWR;
    int t = II , xprev,yprev;
    vector<char> coord;
    
    while(t--){
            
        int counter=1;
        coord.clear();
        
        int x = II , y = II ;
        xprev=0;
        yprev=0;
        while(x!=xprev)
        {
           if(x-xprev >0)
           {
            xprev+=counter;
            counter++; 
            coord.push_back('E');
          } 
          else
          {
             xprev+=counter;
             coord.push_back('E');
             counter++;
             xprev-=counter;
             coord.push_back('W');
             counter++;
           }
        }
        while(y!=yprev)
        {
            if(y-yprev >0){
                yprev-=counter;
                coord.push_back('S');
                counter++;    
                        yprev+=counter;
                coord.push_back('N');
                counter++;           
 
            } else{
                yprev+=counter;
                coord.push_back('N');
                counter++;   
                                yprev-=counter;
                coord.push_back('S');
                counter++; 
            }        
        }
        CS;
        //printf("Case #%d: %d",cs++,coord.size());
        for(int j=0;j<coord.size();j++){
            printf("%c",coord[j] );
        }
        printf("\n");
                coord.clear();
        
    }
    
    return 0;
}
