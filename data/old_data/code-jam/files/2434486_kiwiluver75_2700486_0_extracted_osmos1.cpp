#include<iostream>
#include<vector>
#include<algorithm>
#include<stack>
#include<queue>
#include<cstring>
#include<cstdio>
#include<cmath>
#include<set>
#include<map>


using namespace std;

#define s(a) scanf("%d",&(a))
#define nl printf("\n")

int main(){
    int cases,casenum=1,mote,n,c,curr;
    freopen("osmos.in","r",stdin);
    freopen("osmos.out","w",stdout);
    s(cases);
    while(cases--){
        c=0;
        s(mote);
        s(n);
        curr=mote;
        int arr[n];
        for(int i=0;i<n;i++)
            scanf("%d",a+i);
        sort(a,a+n);
        for(int i=0;i<n;i++)
        {
            if(curr <= arr[i])
            {
                if(curr+curr-1 > arr[i])
                    curr += curr-1+arr[i];
                c++;
            }
            else
                curr+=arr[i];
        }
        printf("Case #%d: %d\n",casenum++,c);
    }
}

