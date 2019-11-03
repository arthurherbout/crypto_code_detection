#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
int a[29];
int main() {
    int t,ca;
    scanf("%d",&t);
    for(ca=1;ca<=t;ca++)
        {
        int n;
        scanf("%d",&n);
        set< pair<int,int> > s;
        set<pair<int,int> >::iterator I;
        int i,x,y;
        int total = 0;
        for(i=0;i<n;i++)
            {
            scanf("%d",&x);
            a[i] = x;
            total += x;
        }
        printf("Case #%d:",ca);
       while(total)
           {
           int ma = 0;
           int in = -1;
           for(i=0;i<26;i++)
               if(a[i] > ma){
               ma = a[i];
               in = i;
           }
           printf(" %c",in+'a');
           a[in]--;
           total--;
           for(i=0;i<26;i++)
               {
               if(2*a[i] > total){
                   printf("%c",i+'a');
                   a[i]--;
                   total--;
                   break;
               }
           }
       }
        printf("\n");
    }
    return 0;
}

