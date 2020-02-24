#include<bits/stdc++.h>
using namespace std;
typedef long long ll;


char a[55][55];
int main() {
    /* Enter your code here. Read input from STDIN. Print output to STDOUT */   
    int t,ca,n;
    scanf("%d",&t);
    for(ca=1;ca<=t;ca++)
        {
     
       scanf("%d",&n);
        int i,j;
        int total = 0;
        for(i=1;i<=n;i++)
            {
            scanf("%s",&a[i]);
            for(j=1;j<=n;j++)
                {
                if(a[i][j-1]=='1')
                    {
                    total++;
                }
            }
        }
        int found ;
        while(1){
            found = 0;
            int k;
        for(i=1;i<=n;i++)
            {
           int co = 0;
            vector<int> v;
            for(j=1;j<=n;j++)if(a[j][i-1]=='1')v.push_back(j);
              //  printf("%d is 1 in %d\n",i,v.size());
                for(j=0;j<v.size();j++)
                for(k=j+1;k<v.size();k++)
                {
                int x = v[j],y=v[k];
                   // printf("checking %d and %d which are %s and %s\n",x,y,a[x],a[y]);
                if(strcmp(a[x],a[y]))
                    {
                    found = 1;
                   // printf("now copycat\n");
                    int l;
                    for(l=0;l<n;l++)
                        {
                        if(a[x][l]!=a[y][l])a[x][l] = a[y][l] = '1';
                    }
                   // printf("becomes %s %s\n",a[x],a[y]);
                }
            }
                
        }
            if(found==0)break;
        }
        map< pair<int,string>,int> co;
        int cur = 0;
        int none = 0;
        for(i=1;i<=n;i++){
            int vv = 1;
            int l = 0;
            for(j=1;j<=n;j++){
                if(a[i][j-1]=='1'){
             vv=0;       cur++;
                    l++;
                }
            }
            if(l)
            co[make_pair(l,a[i])]++;
            none+= vv;
        }
            cur = cur-total;
              map< pair<int,string>,int>::iterator I;
        for(I=co.begin();I!=co.end();I++)
            {
            string s = I->first.second;
            int xx = I->first.first;
            int yy = I->second;
           int f = max(xx,yy);
            cur += f*f - xx*yy;
            int di = f-min(xx,yy);
            none-= di;
        }
        cur+= none;
        printf("Case #%d: %d\n",ca,cur);
       
    }
    return 0;
}

