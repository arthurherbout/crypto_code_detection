#include<iostream>
#include<cstring>
#include<cstdio>
#include<algorithm>
#include<vector>
using namespace std;
#define f first
#define s second
#define mp make_pair
char mat[105][105];
bool nwt[105][105];
int n,k;
bool row[105],col[105],dia1[305],dia2[305];
void write(){
    int i,j;
    for(i=1;i<=n;i++,cout<<endl)
        for(j=1;j<=n;j++)
            cout<<mat[i][j]<<" ";
    getchar();
    getchar();
    return;
}
void read(){
    cin>>n>>k;
    char c;
    int a,b;
    for(int i=1;i<=n;i++)
        for(int j=1;j<=n;j++)
            mat[i][j]='.';
    memset(row,0,sizeof row);
    memset(col,0,sizeof col);
    memset(dia1,0,sizeof dia1);
    memset(dia2,0,sizeof dia2);
    for(int i=1;i<=k;i++){
        cin>>c>>a>>b;
        mat[a][b]=c;
        if(c=='+' || c=='o') dia1[a-b+n]=dia2[a+b]=1;
        if(c=='x' || c=='o') row[a]=col[b]=1;
    }
}
vector < pair < char , pair < int , int > > > v;
void solve(){
    memset(nwt,0,sizeof nwt);
    for(int i=1;i<=n;i++){
        if(!row[i]){
            for(int j=1;j<=n;j++){
                if(!col[j]){
                    row[i]=col[j]=1;
                    if(mat[i][j]=='+') mat[i][j]='o';
                    if(mat[i][j]=='.') mat[i][j]='x';
                    nwt[i][j]=1;
                    break;
                }
            }
        }
    }
    int X,Y;
    for(int i=1;i<=n;i++){
        if(!dia1[i]){
            for(int j=2;j<=2*n;j++){
                if((i+j-n)&1) continue;
                X=(i+j-n)/2;
                Y=(j-i+n)/2;
                if(X>=1 && X<=n && Y>=1 && Y<=n && !dia2[j]){
                    dia1[i]=dia2[j]=1;
                    if(mat[X][Y]=='x') mat[X][Y]='o';
                    if(mat[X][Y]=='.') mat[X][Y]='+';
                    nwt[X][Y]=1;
                    break;
                }
            }
        }
    }
    for(int i=2*n-1;i>n;i--){
        if(!dia1[i]){
            for(int j=2;j<=2*n;j++){
                if((i+j)&1) continue;
                X=(i+j-n)/2;
                Y=(j-i+n)/2;
                if(X>=1 && X<=n && Y>=1 && Y<=n && !dia2[j]){
                    dia1[i]=dia2[j]=1;
                    if(mat[X][Y]=='x') mat[X][Y]='o';
                    if(mat[X][Y]=='.') mat[X][Y]='+';
                    nwt[X][Y]=1;
                    break;
                }
            }
        }
    }
    int res=0;
    v.clear();
    for(int i=1;i<=n;i++)
        for(int j=1;j<=n;j++){
            if(mat[i][j]=='+' || mat[i][j]=='x') res++;
            else if(mat[i][j]=='o') res+=2;
            if(nwt[i][j]){
                v.push_back(mp(mat[i][j],mp(i,j)));
            }
        }
    cout<<res<<" "<<v.size()<<endl;
    for(int i=0;i<v.size();i++)
        cout<<v[i].f<<" "<<v[i].s.f<<" "<<v[i].s.s<<endl;
}
int main(){
    int t,i;
    freopen("Ds.in","r",stdin);
    freopen("Ds.out","w",stdout);
    cin>>t;
    for(i=1;i<=t;i++){
        read();
        cout<<"Case #"<<i<<": ";
        solve();
    }
}
