#include <bits/stdc++.h>
using namespace std;

const int INF = 1000000007;

bitset<2005> tag[205];
char s[20005];
map<string,int> idx;
int cnt[4005],jmp[4005];

int getidx(const string& s){
    return idx.insert(make_pair(s,idx.size())).first->second;
}

void reduce(){
    int m=idx.size(),n=0;
    for(int i=0;i<m;i++) if(cnt[i]==1){
        jmp[i]=-1;
    }else{
        cnt[n]=cnt[i];
        jmp[i]=n++;
    }
}

int main(){
    int cs,n;
    scanf("%d",&cs);
    for(int no=1;no<=cs;no++){
        scanf("%d",&n);
        gets(s);
        idx.clear();
        vector<int> u[205];
        for(int i=0;i<n;i++){
            gets(s);
            for(char* o=strtok(s," ");o;o=strtok(0," ")){
                int x=getidx(o);
                cnt[x]++;
                u[i].push_back(x);
            }
        }
        reduce();
        for(int i=0;i<n;i++){
            for(size_t j=0;j<u[i].size();j++){
                int x=u[i][j];
                if(jmp[x]<0){
                    swap(u[i][j--],u[i].back());
                    u[i].pop_back();
                }else{
                    u[i][j]=jmp[x];
                }
            }
            tag[i].reset();
            for(int x:u[i]) tag[i].set(x);
        }
        int mask=(1<<n)-1,ans=INF;
        for(int s=1;s<=mask;s+=4){
            bitset<2005> en,fr;
            for(int i=0;i<n;i++){
                if((s>>i)&1) en|=tag[i];
                        else fr|=tag[i];
            }
            ans=min(ans,int((en&fr).count()));
        }
        printf("Case #%d: %d\n",no,ans);
    }
}
