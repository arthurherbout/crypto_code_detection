#include<bits/stdc++.h>
using namespace std;

#define REP(i,a,b) for(i=a;i<b;i++)
#define rep(i,n) REP(i,0,n)

#define INF 1000000000
#define MINF -103871688

int T, N;
char mode[1200]; int ID[1200];

int fs[2222], ls[2222];
int dd[2222];

int main(){
  int i, j, k, dame, last[2222];
  int res, cnt = 0;
  char now[2222];

  scanf("%d",&T);
  while(T--){
    //fprintf(stderr,"rest %d\n",T);
    printf("Case #%d: ", ++cnt);

    scanf("%d",&N);
    rep(i,N) scanf("%s%d",mode+i,ID+i);

    dame = 0;
    rep(k,2010) now[k] = '?';
    rep(k,2010) last[k] = -1;
    rep(i,N) if(!dame){
      if(ID[i]==0) continue;
      if(now[ID[i]] != mode[i]){
        last[ID[i]] = i;
        now[ID[i]] = mode[i];
        continue;
      }
      
      if(mode[i] == 'E'){
        for(j=last[ID[i]];j<=i;j++) if(ID[j]==0 && mode[j]=='L'){
          ID[j] = ID[i];
          break;
        }
        if(j>i) dame = 1;
        last[ID[i]] = i;
        continue;
      }

      if(mode[i] == 'L'){
        last[ID[i]] = i;
        continue;
      }
    }
    
    rep(k,2010) now[k] = '?';
    rep(k,2010) last[k] = -1;
    for(i=N-1;i>=0;i--) if(!dame){
      if(ID[i]==0) continue;
      if(now[ID[i]] != mode[i]){
        last[ID[i]] = i;
        now[ID[i]] = mode[i];
        continue;
      }
      
      if(mode[i] == 'E'){
        last[ID[i]] = i;
        assert(0 && "mode E");
        continue;
      }
      
      if(mode[i] == 'L'){
        for(j=last[ID[i]];j>=i;j--) if(ID[j]==0 && mode[j]=='E'){
          ID[j] = ID[i];
          break;
        }
        if(j<i) dame = 1;
        last[ID[i]] = i;
        continue;
      }
    }
    if(dame){ puts("CRIME TIME"); continue; }

    rep(k,2100) fs[k] = 1000000000, ls[k] = -1000000000;
    rep(i,N) fs[ID[i]] = min(fs[ID[i]], i);
    rep(i,N) ls[ID[i]] = max(ls[ID[i]], i);

    for(i=N-1;i>=0;i--){
      if((ID[i]==0 || (ID[i]>0 && ls[ID[i]]==i)) && mode[i]=='E'){
        REP(j,i,N){
          if(ID[j]==-1) continue;
          if(ID[i]==0 && ID[j]>0 && fs[ID[j]]==j && mode[j]=='L'){
            ID[i] = ID[j];
            fs[ID[j]] = i;
            break;
          }
        }
      }
    }
    for(i=N-1;i>=0;i--){
      if((ID[i]==0 || (ID[i]>0 && ls[ID[i]]==i)) && mode[i]=='E'){
        REP(j,i,N){
          if(ID[j]==-1) continue;
          if(ID[j]==0 && mode[j]=='L'){
            if(ID[i]==0) ID[i] = ID[j] = -1;
            else         ID[j] = ID[i], ls[ID[j]] = j;
            break;
          }
        }
      }
    }

    
    rep(i,N) if(ID[i]==0 && mode[i]=='E'){
      int ind = -1, mn = 100000;
      rep(k,2020) dd[k] = 0;

      for(j=i-1;j>=0;j--){
        if(ID[j] <= 0) continue;
        if(dd[ID[j]]) continue;
        dd[ID[j]] = 1;

        if(ls[ID[j]] < i) continue;
        if(mode[j]=='L') continue;

        ind = ID[j];
        mn = j;
      }
      if(ind != -1){
        REP(j,mn,i) if(ID[j]==0 && mode[j]=='L'){
          ID[i] = ID[j] = ind;
          break;
        }
      }
    }

/*    puts("");
    rep(i,N){
      printf("ID %d mode %c",ID[i],mode[i]);
      if(ID[i] && ls[ID[i]]==i) printf(" LAST");
      puts("");
    }*/

    res = 0;
    rep(i,N){
      if(ID[i]==0 && mode[i]=='E') res++;
      if(ID[i]>0 && ls[ID[i]]==i && mode[i]=='E') res++;
    }
    
    printf("%d\n",res);
  }

  return 0;
}
