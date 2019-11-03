#include <iostream>
#include <string>
#include <string.h>
#include <cstdio>
using namespace std;

char h[26];
int main()
{
    //freopen("t.in","r",stdin);
    //freopen("t.txt","w",stdout);
    int t;
    string a="ourlanguageisimpossibletounderstandtherearetwentysixfactorialpossibilitiessoitisokayifyouwanttojustgiveup";
    string b="ejpmysljylckdkxveddknmcrejsicpdrysirbcpcypcrtcsradkhwyfrepkymveddknkmkrkcddekrkdeoyakwaejtysrreujdrlkgcjv";
    string tmp,ans;
    int len=a.length();
    for(int i=0;i<len;i++)
        h[ b[i]-'a' ]=a[i];
    h['q'-'a']='z';
    h['z'-'a']='q';
    scanf("%d",&t);
    for(int i=1;i<=t;i++)
    {
        scanf("\n");
        getline(cin,tmp);
        ans="";
        for(int k=0;k<tmp.length();k++)
            if(tmp[k]==' ')
                ans+=tmp[k];
            else
                ans+=h[ tmp[k]-'a' ];
        printf("Case #%d: ",i);
        cout<<ans<<endl;
    }
    return 0;
}
