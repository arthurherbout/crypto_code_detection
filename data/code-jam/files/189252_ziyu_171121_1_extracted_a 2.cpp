
#include <string>
#include <sstream>
#include <algorithm>
#include <vector>
#include <iostream>

using namespace std;

void solve()
{
    int case_num;
    cin>>case_num;

    int cnt=0;

    bool mark[256];
    int value[256];

    while(case_num--){
        string s;
        cin>>s;

        memset(mark,0,sizeof(mark));
        memset(value,-1,sizeof(value));

        int base = 0;

        for(int i=0;i<s.size();++i){
            if(!mark[s[i]]){
                base++;
                mark[s[i]]=true;
            }
        }

        if(base==1) base=2;

        value[s[0]]=1;

        int v = 0;
        for(int i=0;i<s.size();++i){
            if(value[s[i]]==-1){
                value[s[i]] = v;
                s[i] = v;
                v++;
                if(v==1) v=2;
            }else{
                s[i] = value[s[i]];
            }
        }

        long long res = 0;
        for(int i=0;i<s.size();++i){
            res*=base;
            res+=s[i];
        }

       if(s.size()==1) res=1;


      cout<<"Case #"<<++cnt<<": "<<res<<endl;
    }
}

int main()
{
    solve();
    return 0;
}
