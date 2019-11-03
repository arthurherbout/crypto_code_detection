#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>
#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
int gcd(int a, int b)
    {
    int t;
    while(a%b)
        {
        t=a%b;a=b;b=t;
    }
    return b;
}
int a[1005];
int b[1005];
int main() {
    /* Enter your code here. Read input from STDIN. Print output to STDOUT */   
    int t,ca;
    scanf("%d",&t);
    for(ca=1; ca<=t; ca++)
        {
       int n;
        scanf("%d",&n);
        int i;
        
        for(i=1;i<=n;i++)
            scanf("%d",&a[i]);
        int ans = 0;
        int mask;
        int total = 1<<n;
        for(mask=1;mask<total;mask++)
            {
            int tmp = 0;
            for(i=0;i<n;i++)
                {
                if(mask & (1<<i))
                    {
                    b[tmp++] = i+1;
                }
            }
            sort(b,b+tmp);
            int v = 0;
           // if(ca==3)v = 1;
           
            while(1)
                {
                 if(v)
                {
                for(i=0;i<tmp;i++)
                    printf("%d ",b[i]);
                printf("\n");
            }
                int valid = 1;
                for(i=0;i<tmp;i++)
                    {
                    int x = (i+1)%tmp;
                    int y = (i-1+tmp)%tmp;
                    if(b[x]!=a[b[i]] && b[y]!=a[b[i]])valid = 0;
                }
                if(v)
                printf("valid is %d\n",valid);
                if(valid){
                    if(tmp > ans)ans = tmp;
                }
                if(!next_permutation(b,b+tmp))break;
            }
        }
        printf("Case #%d: %d\n",ca,ans);
    }
    return 0;
}

