#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
int n,k;
double solve(vector<double> res)
    {
    double ans = 0;
       int i;
        char s[55];
        int in = 0;
        for(i=0;i<k/2;i++)
            {
            s[in++] = '0';
            s[in++] = '1';
        }
        s[in] = 0;
        sort(s,s+k);
       // printf("%s\n",s);
       while(1)
           {
           
         //  printf("%s\n",s);
           double cur = 1;
           for(i=0;i<k;i++)
               if(s[i]=='0')cur*=(1-res[i]);
               else cur*=res[i];
             ans+= cur;
           if(!next_permutation(s,s+k))break;
       }
    return ans;
}

int main() {
    /* Enter your code here. Read input from STDIN. Print output to STDOUT */   
    int t,ca;
    scanf("%d",&t);
    for(ca=1;ca<=t;ca++)
        {
       
        scanf("%d%d",&n,&k);
        vector<double> v;
        int i;
        double cc;
        for(i=0;i<n;i++)
            {
            cin>>cc;
            v.push_back(cc);
        }
        sort(v.begin(),v.end());
         vector<double> res;
        for(i=0;i<k/2;i++)
            {
            res.push_back(v[i]);
            res.push_back(v[n-1-i]);
        }
       // printf("%d\n",v.size());
        
       // ans/=(1<<k);
        double ans=0.0;
        char s[55];
        for(i=0;i<k;i++)s[i] = '1';
        for(;i<n;i++)s[i] = '0';
        s[n] = 0;
        sort(s,s+n);
        while(1)
            {
            vector<double> res;
            for(i=0;i<n;i++)if(s[i]=='1')res.push_back(v[i]);
                double cur = solve(res);
            if(cur > ans)ans = cur;
            if(!next_permutation(s,s+n))break;
        }
        printf("Case #%d: %.8lf\n",ca,ans);
    }
    return 0;
}

