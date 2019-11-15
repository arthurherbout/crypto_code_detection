#include<bits/stdc++.h>
using namespace std;
string b[] = {"ZERO", "ONE", "TWO", "THREE", "FOUR", "FIVE", "SIX", "SEVEN", "EIGHT", "NINE"};
char a[100005];
int co[26];
int vis[] = {0,2,4,6,1,5,7,9,8,3};
char ans[100005];
int main()
{
    int t;
    scanf("%d",&t);
    int ca;
    map<int,int> cn[10];
    int i;
    int j;
    for(i=0;i<10;i++)
    {
        int l = b[i].length();
        for(j=0;j<l;j++)
        cn[i][b[i][j]-'A']++;
    }
    for(ca=1;ca<=t;ca++)
    {
        int in = 0;
        scanf("%s",&a);
        int ma = 0;
        printf("Case #%d: ",ca);
        
        for(i=0;i<26;i++)co[i] = 0;
        for(i=0;a[i];i++)co[a[i]-'A']++;
        int ii;
        for(ii=0;ii<10;ii++)
        {
            i = vis[ii];
          //  printf("check for %d\n",i);
            int ma = 3005;
            map<int,int>::iterator I ;
            for(I=cn[i].begin();I!=cn[i].end();I++)
            {
               // ma = min(ma,co[])
               int in = I->first;
               int cur = co[in];
               int mak = cur/(I->second);
               ma = min(ma,mak);
            }
            //printf("can make %d\n",ma);
           
            for(j=0;j<ma;j++) ans[in++] = i+'0';
             for(I=cn[i].begin();I!=cn[i].end();I++)
            {
               // ma = min(ma,co[])
               int in = I->first;
               int cur = co[in];
               co[in]-= ma*(I->second);
            }
        }
        ans[in] = 0;
        sort(ans,ans+in);
        printf("%s\n",ans);
        
    }
}
