#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
ll po[65];
char str[55];
char ans[55][55];
int main() {
    int t,ca,i;
    scanf("%d",&t);
    po[0] = 1;
     int x,y,z;
      
    for(ca=1;ca<=55;ca++)
        {
        po[ca] = 2* po[ca-1];
    }
    for(ca=1;ca<=t;ca++)
        {
   int n;
       // scanf("%d",&n);
        int j,p,s,k;
        
        scanf("%d%d%d%d",&j,&p,&s,&k);
        if(k==1)
            {
            printf("Case #%d: %d\n",ca,j*p);
            for(i=1;i<=j;i++){
                z=i;
                for(x=1;x<=p;x++){
                printf("%d %d %d\n",i,x,z);
                    z++;
                    if(z>s)z =1;
             
            }
            }
        }
        else if(k>=3)
            {
            printf("Case #%d: %d\n",ca,j*p*s);
        for(x=1;x<=j;x++)
            {
            for(y=1;y<=p;y++)
                {
                for(z=1;z<=s;z++)
                    {
                    printf("%d %d %d\n",x,y,z);
                }
            }
        }
        }
        else {
    // printf("%d %d %d\n",j,p,s);
            if(j==3)
                {
            printf("Case #%d: %d\n",ca,12);
                printf("1 1 1\n");
                printf("1 1 2\n");
                printf("1 2 2\n");
                printf("1 2 3\n");
                printf("1 3 1\n");
                printf("1 3 3\n");
                printf("2 1 1\n");
                printf("2 1 2\n");
                printf("2 2 2\n");
                printf("2 2 3\n");
                printf("2 3 1\n");
                printf("2 3 3\n");
                continue;
            }
            
       vector< pair<int,pair<int,int> > > v;
      
        for(x=1;x<=j;x++)
            {
            for(y=1;y<=p;y++)
                {
                for(z=1;z<=s;z++)
                    {
                   v.push_back(make_pair(x,make_pair(y,z)));
                   // printf("%d %d %d\n",x,y,z);
                }
            }
        }
      int total = 1<<v.size();
      int ma = 0;
        int ans = 0;
        int mask;
       // printf("total %d\n",total);
        for(mask=1;mask<total;mask++)
            {
          //  printf("mask %d means \n",mask);
            int valid = 1;
            int cur = 0;
             map< string ,int> co;
            for(i=0;i<v.size();i++)
                {
                if(mask & (1<<i))
                    {
                    cur++;
                    x = v[i].first;
                    y = v[i].second.first;
                    z = v[i].second.second;
                    //printf("%d %d %d\n",x,y,z);
                    sprintf(str,"%d#%d",x,y);
                    co[str]++;
                    sprintf(str,"%d@%d",x,y);
                    co[str]++;
                    sprintf(str,"%d$%d",x,y);
                    co[str]++;
                }
            }
            if(cur > ma)
                {
                map<string ,int>::iterator I;
                for(I=co.begin();I!=co.end();I++)
                    if(I->second >k){
                    valid = 0;
                   // cout<< I->first<<" is max"<<endl;
                    break;
                }
                if(valid)
                    {
                  //  cout<< "valid "<<cur<<endl;
                    ma = cur;
                    ans = mask;
                }
            }
        }
          //int ans=0;
       
        printf("Case #%d: %d\n",ca,ma);
        for(i=0;i<v.size();i++)
            if(ans&(1<<i))
            printf("%d %d %d\n",v[i].first,v[i].second.first,v[i].second.second);
        }
    }
    return 0;
}

