#include <bits/stdc++.h>
using namespace std;

char grid[103][103];
char grid2[103][103];
int mp1[250];
int mp2[250];
vector<pair<char,pair<int,int> > > v;
vector<pair<int,int> > points;
int n,m;
int ans =0 ;
int re,ce;



void check(int p,int x,bool pr){
    memset(mp1,0,sizeof(mp1));
    memset(mp2,0,sizeof(mp2));
    points.clear();
    int allsum = 0 ;
    memset(grid2,'.',sizeof(grid2));
    if(p < n){
        for(int i=0;i<n;i++){
            grid2[p][i] = '+';
            allsum++;
            mp1[p+i+105]++;
            mp2[p-i+105]++;
        }
    }
    else{
        p-=n;
        for(int i=0;i<n;i++){
            grid2[i][p] = '+';
            allsum++;
             mp1[p+i+105]++;
            mp2[p-i+105]++;
        }
    }
    if(x < n){
        int r = 0;
        int c = x;
        while(r >= 0 && r < n && c >=0 && c < n){
                if(grid2[r][c] == '+'){
                    allsum--;
                    mp1[r+c+105]--;
                    mp2[r-c+105]--;
                }
                grid2[r][c] = 'x';
                allsum++;
                points.push_back(make_pair(r,c));
            r++;
            c++;
        }
        r = n-1;
        c = x-1;
        while(r >= 0 && r < n && c >=0 && c < n){
                if(grid2[r][c] == '+'){
                    allsum--;
                    mp1[r+c+105]--;
                    mp2[r-c+105]--;
                }
                grid2[r][c] = 'x';
                allsum++;
                points.push_back(make_pair(r,c));
            r--;
            c--;
        }

    }
    else{
        x-=n;
        int r = 0;
        int c = x;
        while(r >= 0 && r < n && c >=0 && c < n){
                if(grid2[r][c] == '+'){
                    allsum--;
                    mp1[r+c+105]--;
                    mp2[r-c+105]--;
                }
                grid2[r][c] = 'x';
                allsum++;
                points.push_back(make_pair(r,c));
            r++;
            c--;
        }
        r = n-1;
        c = x+1;
        while(r >= 0 && r < n && c >=0 && c < n){
            if(grid2[r][c] == '+'){
                    allsum--;
                    mp1[r+c+105]--;
                    mp2[r-c+105]--;
                }
                grid2[r][c] = 'x';
                allsum++;
                points.push_back(make_pair(r,c));
            r--;
            c++;
        }
    }
    int si = points.size();
    for(int i=0;i<si;i++){
        int r = points[i].first;
        int c = points[i].second;
        if(mp1[r+c+105] == 0 && mp2[r-c+105] == 0){
            mp1[r+c+105] = 1;
            mp2[r-c+105] = 1;
            grid2[r][c] = 'o';
            allsum++;
        }
    }
    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            if(mp1[i+j+105] == 0 && mp2[i-j+105] == 0){
                if(grid2[i][j] != 'x')
                    allsum++;
                grid2[i][j] = '+';
                mp1[i+j+105]++;
                mp2[i-j+105]++;
            }
        }
    }
    if(allsum > ans){
        ans = allsum;
        v.clear();
        for(int i=0;i<n;i++){
            for(int j=0;j<n;j++){
                if(grid[i][j] != grid2[i][j]){
                    v.push_back(make_pair(grid2[i][j],make_pair(i,j)));
                }
            }
        }
    }
}

void gen(){
    for(int i=0;i<2*n;i++){
        for(int j=0;j<2*n;j++){
            check(i,j,0);
        }
    }
}

int main() {
    freopen("in.txt","r",stdin);
    freopen("out.txt","w",stdout);
    int t;
    scanf("%d",&t);
    int cs = 1;
    while(t--){
        printf("Case #%d: ",cs++);
        scanf("%d%d",&n,&m);
        memset(grid,'.',sizeof(grid));
        ans = 0 ;
        while(m--){
            char x;
            scanf(" %c",&x);
            int r,c;
            scanf("%d%d",&r,&c);
            r--;
            c--;
            grid[r][c] = x;
        }
        gen();
        check(re,ce,1);
        int si = v.size();
        printf("%d %d\n",ans,si);
        for(int i=0;i<si;i++){
            printf("%c %d %d\n",v[i].first,v[i].second.first+1,v[i].second.second+1);
        }
    }
    return 0;
}
