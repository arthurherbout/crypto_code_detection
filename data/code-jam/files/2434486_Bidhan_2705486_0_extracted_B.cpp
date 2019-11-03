/*
 * Bidhan Roy
 * University of Dhaka
 */

using namespace std;

#include <algorithm>
#include <bitset>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <list>
#include <ctime>
#include <map>
#include <queue>
#include <set>
#include <sstream>
#include <stack>
#include <string>
#include <vector>

#define rep(i,n) for(__typeof(n) i=0; i<(n); i++)
#define foreach(i,n) for(__typeof((n).begin())i =(n).begin();i!=(n).end();i++)
#define inf (1<<28)
#define eps 1e-9
#define pb push_back
#define ins insert
#define mp make_pair
#define sz(x) ((int)x.size())
#define clr clear()
#define all(x) x.begin(),x.end()
#define xx first
#define yy second
#define sqr(x) ((x)*(x))
#define mem(x,val) memset((x),(val),sizeof(x));
#define read(x) freopen(x,"r",stdin);
#define rite(x) freopen(x,"w",stdout);

template <class T> inline T __in() { T v; cin>>v; return v; }

#define Q __in<int>()
#define QQ __in<i64>()
#define QS __in<st>()

typedef long long i64;
typedef unsigned long long ui64;
typedef string st;
typedef vector<int> vi;
typedef vector<st> vs;
typedef map<int,int> mii;
typedef map<st,int> msi;
typedef map<int,st> mis;
typedef set<int> si;
typedef set<st> ss;
typedef pair<int,int> pii;
typedef vector<pii> vpii;

#define mx 60

/*i64 hash(char *inp){
    i64 ret=0;
    for(int i=0; inp[i]; i++){
        ret*= 27;
        ret+= inp[i];
    }
    return ret;
}|*/
#define MAX 26
struct trie {
    trie *next[MAX+1];
    trie() { for(int i=0; i<=MAX; i++) next[i] = NULL; }
};

void insert(trie *root, char *seq) {
    trie *curr = root;
    for(int i = 0; i < seq[i]; i++) {
        if(!curr->next[seq[i]-'a']) curr->next[seq[i]-'a'] = new trie;
        curr = curr->next[seq[i]-'a'];
    }
    if(!curr->next[MAX]) curr->next[MAX] = new trie;
}
trie *root;
char inp[60];
//int dp[mx][mx][mx][28];
map<trie *,int> dp[60][60];
int len;
int call(int p,int lst,trie *curr){
    if(p==len){
        if(curr->next[MAX]) {
            //cout<<"ok"<<endl;
            return 0;
        }
        return inf;
    }
    if(dp[p][lst].find(curr)!=dp[p][lst].end()) return dp[p][lst][curr];

    int ret=inf;
    char letter=inp[p];
    //cout<<"entered "<<p<<" "<<letter<<endl;
    if(curr->next[MAX]) {
        //cout<<p<<" ending"<<endl;
        ret=min(ret,call(p,len,root));
        //cout<<"<------"<<endl;
    }
    if(curr->next[letter-'a']){
        //if(p==len-1) cout<<"here"<<endl;
        //cout<<len<<","<<p<<" Taking "<<char(letter)<<endl;
        ret=min(ret,call(p+1,lst,curr->next[letter-'a']));
        //cout<<"<------"<<endl;
    }
    if(lst==len || p-lst>4)
    for(int i=0; i<26; i++){
        //if(i==2) if(!curr->next[i]) cout<<"here"<<endl;
        if(!curr->next[i]) continue;
        if(i==letter-'a') continue;
        //cout<<len<<",,"<<p<<" Taking +1 "<<char(i+'a')<<endl;
        ret=min(ret,call(p+1,p,curr->next[i])+1);
        //cout<<"<<---------"<<ret<<endl;
    }
    //cout<<p<<" "<<lst<<"="<<ret<<endl;
    return dp[p][lst][curr]=ret;
}



int main(){
    root= new trie;
    FILE *dic=fopen("Dic.txt","r");
    FILE *input=fopen("in.txt","r");
    FILE *output=fopen("out.txt","w");
    double cl = clock();
    while( fscanf(dic,"%s",inp)==1 ){
        //if(strcmp("b",inp)==0) cout<<"1"<<endl;;
        //if(strcmp("ibb",inp)==0) cout<<"2"<<endl;;
        //cout<<inp<<endl;
        insert(root,inp);
    }
    //cout<<"here"<<endl;
    int test,kas=0;
    //cin>>test;
    fscanf(input,"%d",&test);
    //cout<<test<<endl;
    while(test--){
        //cin>>inp;
        fscanf(input,"%s",inp);
        len=strlen(inp);
        rep(ii,mx) rep(j,mx) dp[ii][j].clr;
        //cout<<call(0,len,root)<<endl;
        //cout<<"here"<<endl;
        fprintf(output,"Case #%d: %d\n",++kas,call(0,len,root));
    }
    cl = clock() - cl;
    cerr<<endl<<endl<<endl;
    cerr<<"-----------------------------------"<<endl;
	cerr<<"Total Execution Time = "<<cl / CLOCKS_PER_SEC<<" s"<<endl;
	cerr<<"-----------------------------------"<<endl;
	fclose(dic);
	fclose(input);
	fclose(output);
	return 0;
}
