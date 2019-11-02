
#include <string>
#include <sstream>
#include <algorithm>
#include <vector>
#include <iostream>

using namespace std;

int prison[100];
int q[100];
bool mark[100];
int pno,qno;
int cnt;
int res;

void backtracing(int depth)
{
    if(qno==depth){
        res = min(res,cnt);
        return;
    }

    for(int i=0;i<qno;++i){
        if(!mark[i]){
            mark[i] = true;
            int t=0;
            prison[q[i]]=1;
            for(int j=q[i]-1;j>=1;--j)
                if(prison[j]==0)
                    t++;
                else
                    break;
            for(int j=q[i]+1;j<=pno;++j)
                if(prison[j]==0)
                    t++;
                else
                    break;
            cnt+=t;

            backtracing(depth+1);
            cnt-=t;
           mark[i] = false;
           prison[q[i]]=0;
        }
    }
}

void solve()
{
    int case_num;
    cin>>case_num;

    int casecnt=0;

    while(case_num--){
        cin>>pno>>qno;

        for(int i=0;i<qno;++i)
            cin>>q[i];

       memset(mark,0,sizeof(mark));
       memset(prison,0,sizeof(prison));

       res = INT_MAX;
       cnt=0;
       backtracing(0);
      cout<<"Case #"<<++casecnt<<": "<<res<<endl;
    }
}

int main()
{
    solve();
    return 0;
}
