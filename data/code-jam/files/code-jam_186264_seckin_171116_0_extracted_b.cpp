#include <vector>
#include <list>
#include <map>
#include <set>
#include <deque>
#include <queue>
#include <stack>
#include <bitset>
#include <algorithm>
#include <functional>
#include <numeric>
#include <utility>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <cstdio>
#include <cmath>
#include <cstdlib>
#include <cctype>
#include <string>
#include <cstring>
#include <cstdio>
#include <cmath>
#include <cstdlib>
#include <ctime> 
#define all(c) (c).begin(),(c).end()
#define tr(c,i) for(typeof((c).begin()) i = (c).begin(); i != (c).end(); i++)
#define ll long long 
#define sz(a) int((a).size())
#define pb push_back
#define SORT(x) sort((x).begin(),(x).end()); 
#define VI vector<int>
#define VII vector < vector <int> >
#define MP make_pair
#define SET(x,byte) memset(x,byte,sizeof(x));
#define I(x) ((x) - 'A')
#define REP(i,mx) for(int i=0;(i)<(mx);i++)
#define RFOR(i,mx,mn) for(int i=(mx);(i)>=(mn);i--)
#define FORD(i,mn,mx,k) for(int i=(mn);(i)<=(mx);i+=(k))
#define FOR(i,mn,mx) for(int i=mn;(i)<=(mx);i++)
#define U(x) (1<<(x))
#define INF 1000000000
#define DEBUG 0

using namespace std;
struct node{
    public:
    int leaf;
    double prob;
    struct node *yes,*no;
};
map<node *,string> mp;
node *root;
double query(double , node*, set<string>& );


node *readroot(){
    node *ptr;
    char ch;
    string str;
    double num;
    
    ptr = (node *)malloc(sizeof(node));
    ptr->leaf=1;
    
    scanf("%lf",&num); ptr->prob = num;
    scanf(" %c",&ch);

    //cout<<"root prob "<<num<<endl;

    if(isalpha(ch)){ 
        while(ch!='\n' && ch!=' '){ str+=ch; scanf("%c",&ch);}

        mp[ptr] = str;
        ptr->leaf=0;

        scanf(" %c",&ch);
   // cout<<"root name saga "<<str<<endl;
        ptr->yes = readroot();
        scanf(" %c",&ch);
   // cout<<"root name sola"<<str<<endl;
        ptr->no = readroot();
   // cout<<"root name cik "<<str<<endl;
        scanf(" %c",&ch);
        return ptr;
    }
    else{ 
        
    //cout<<"root prob cik "<<num<<endl;
        return ptr;
    }
}
void doit(){
    string name,attr;
    int num,l;

    scanf("%d",&l);
    char ch;
    scanf(" %c",&ch);
    root = readroot();
    
    int q;
    scanf("%d",&q);
    //cout<<"tane q "<<q<<endl;
    REP(i,q){
        cin>>name>>num;
        set<string> s;
        REP(j,num){ 
            cin>>attr;
            s.insert(attr);
        }
        double res = query(1.0,root,s);
        printf("%lf\n",res);
    }
}

double query(double prob, node* ptr, set<string>& attr){
    double p2 = prob * ptr->prob;
    if(ptr->leaf) return p2;
    else{
        if(attr.count(mp[ptr])) return query(p2, ptr->yes, attr);
        else return query(p2, ptr->no, attr);
    }
}

int main(){
    int t;
    scanf("%d",&t);
    REP(cs,t){
        printf("Case #%d:\n", cs+1);
        doit();
    }
    return 0;
}
