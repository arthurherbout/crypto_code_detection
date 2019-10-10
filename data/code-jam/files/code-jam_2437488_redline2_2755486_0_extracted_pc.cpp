#include<stdio.h>
#include<set>
#include<algorithm>
#include<string.h>
#include<vector>

using namespace std;

int T,N,d,n,w,e,s,dd,dp,ds,ans;
int wall[1000],tmp[1000];
struct XD{
    int d,n,w,e,s,dd,dp,ds;
};

vector<XD> v;

bool comp(const XD &a,const XD &b)
{
    return a.d>b.d;
}


int main()
{
    freopen("C-small-attempt0.in","r",stdin);
    freopen("pc.out","w",stdout);

    scanf(" %d",&T);
    for( int cas = 1; cas <= T; cas++ ){
        v.clear();
        scanf(" %d",&N);
        fill(wall,wall+1000,0);
        fill(tmp,tmp+1000,0);
        for( int i = 0; i < N; i++ ){
            scanf("%d %d %d %d %d %d %d %d",&d,&n,&w,&e,&s,&dd,&dp,&ds);
            v.push_back((XD){d,n,w+500,e+500,s,dd,dp,ds});
        }
        make_heap(v.begin(),v.end(),comp);

        ans = 0;
        XD now;
        int yn;
        while(!v.empty()){
            N = v.front().d;
            while( v.front().d == N && !v.empty() ){
                now = v.front();
                pop_heap(v.begin(),v.end(),comp);
                v.pop_back();
                yn = 0;
                for( int i = now.w; i< now.e; i++ ){
                    if( wall[i] < now.s )   yn = 1;
                    tmp[i] = max(tmp[i],max(wall[i],now.s));
                }
                if( yn == 1)ans++;
                if( now.n > 1 ){
                    v.push_back((XD){now.d+now.dd,now.n-1,now.w+now.dp,now.e+now.dp,now.s+now.ds,now.dd,now.dp,now.ds});
                    push_heap(v.begin(),v.end(),comp);
                }
            }
            for( int i = 0; i < 1000; i++ )
                wall[i] = max(wall[i],tmp[i]);
        }
        printf("Case #%d: %d\n",cas,ans);
    }

    return 0;
}

