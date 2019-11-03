#include<cstdio>
#include<cstring>
#include<set>
#include<string>
#include<cmath>
#include<iostream>
#include<vector>
#include<algorithm>
using namespace std;
int find(vector<string> s){
    int n = s.size();
    set<string> se;
    se.clear();
    int i,j;
    for(i=0;i<n;i++){
        string t = s[i];
        for(j=0;j<=t.size();j++){
            se.insert(t.substr(0,j));
        }
    }
    set<string>::iterator it;
    /*for(it = se.begin();it!=se.end();it++)
        cout<<*it<<" ";
    cout<<endl;*/
    return se.size();
}
int main()
{
    int t, n, m, i, j, k;
    vector<string> s;
    int bel[100];
    freopen("D-small-attempt0.in","r",stdin);
    freopen("D-small-attempt0.out","w",stdout);
    scanf("%d",&t);
    for(int cnt = 1;cnt<=t; cnt++){
        scanf("%d %d",&m,&n);
        s.clear();
        for(i=0;i<m;i++){
            string ss;
            cin >> ss;
            //cout<<ss<<endl;
            s.push_back(ss);
        }
        int ub = 1;
        int ma = 0, ret = 1;
        for(i=0;i<m;i++)ub*=n;
        for(i=0;i<ub;i++){
            int tt = i;
            for(j=0;j<m;j++){
                bel[j] = tt%n;
                tt/=n;
            }
            int sum = 0;
            for(k=0;k<n;k++){
                vector<string> now;
                now.clear();
                for(j=0;j<m;j++)if(bel[j] == k)
                    now.push_back(s[j]);
                sum += find(now);

            }//printf("%d\n",sum);
            if(sum > ma){
                ma = sum;
                ret = 1;
            }else if(sum == ma){
                ret ++;
            }
        }
        printf("Case #%d: %d %d\n",cnt,ma,ret);

    }
    return 0;
}
