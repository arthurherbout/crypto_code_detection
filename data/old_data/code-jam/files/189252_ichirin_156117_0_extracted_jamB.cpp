#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include <cmath>
using namespace std;
int main(){
  int T,N;
  cin>>T;
  for(int cnt=1;cnt<=T;cnt++){
    vector<int> x,y,z,vx,vy,vz;
    int tx,ty,tz,tvx,tvy,tvz;
    double ans=0.0;

    cin>>N;
    for(int i=0;i<N;i++){
      scanf("%d %d %d %d %d %d",&tx,&ty,&tz,&tvx,&tvy,&tvz);
      x.push_back(tx);      y.push_back(ty);      z.push_back(tz);
      vx.push_back(tvx);      vy.push_back(tvy);      vz.push_back(tvz);
    }

    double ax,ay,az;
    ax=ay=az=0.0;
    
    for(int i=0;i<N;i++){
      ax+=(double)x[i];
      ay+=(double)y[i];
      az+=(double)z[i];
    }
    ax/=(double)N; ay/=(double)N; az/=(double)N;

    ans=ax*ax+ay*ay+az*az;
    int t=0,flag=0,ttt=-2;

    for(;;t++){
      for(int i=0;i<N;i++){//ˆÚ“®
        x[i]+=vx[i]; y[i]+=vy[i]; z[i]+=vz[i];
      }

      ax=ay=az=0.0;
      for(int i=0;i<N;i++){ //’†S
        ax+=(double)x[i];
        ay+=(double)y[i];
        az+=(double)z[i];
      }
      ax/=(double)N; ay/=(double)N; az/=(double)N;

      
      if(ans<ax*ax+ay*ay+az*az)break;
      if(ans==ax*ax+ay*ay+az*az){
        if(t-1==ttt)break;
        ttt=t;
      }
        
      //if(ans<=ax*ax+ay*ay+az*az)break;
      ans=ax*ax+ay*ay+az*az;
    }
    if(ttt!=-2)t=ttt;
    
    printf("Case #%d: %.8lf %.8lf\n",cnt,sqrt(ans),(double)t); //
  }
  return 0;
}