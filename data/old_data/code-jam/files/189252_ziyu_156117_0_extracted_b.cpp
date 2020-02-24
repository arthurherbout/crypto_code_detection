
#include <string>
#include <sstream>
#include <algorithm>
#include <vector>
#include <iostream>
#include <cmath>

using namespace std;

struct firefly{
    double x,y,z,vx,vy,vz;
    firefly(){
        x=y=z=vx=vy=vz=0;
    }
};

#define eps 1e-8

firefly flys[600];

void solve()
{
    int case_num;
    cin>>case_num;

    int cnt=0;

    while(case_num--){
        int flyno;
        cin>>flyno;
        firefly center;
        for(int i=0;i<flyno;++i){
            cin>>flys[i].x>>flys[i].y>>flys[i].z>>flys[i].vx>>flys[i].vy>>flys[i].vz;
            center.x+=flys[i].x;
            center.y+=flys[i].y;
            center.z+=flys[i].z;
            center.vx+=flys[i].vx;
            center.vy+=flys[i].vy;
            center.vz+=flys[i].vz;
        }

        center.x/=flyno;
        center.y/=flyno;
        center.z/=flyno;
        center.vx/=flyno;
        center.vy/=flyno;
        center.vz/=flyno;
        

        double a = center.vx*center.vx+center.vy*center.vy+center.vz*center.vz;
        double b = 2*(center.x*center.vx+center.y*center.vy+center.z*center.vz);
        double c = (center.x*center.x+center.y*center.y+center.z*center.z);

        double t,d;

        if(fabs(a)<eps){
            d = c;
            t = 0;
        }
        else
        if( b*b-4*a*c < eps ){
            t = (-b)/(2*a);
            if(t<eps){
                t = 0;
                d = c;
            }else{
                d = c-(b*b)/(4*a);
            }

        }else{
            double t1,t2;
            t1 = (-b-sqrt(b*b-4*a*c))/(2*a);
            t2 = (-b+sqrt(b*b-4*a*c))/(2*a);

            if(t1<eps)
                t = t2;
            else 
                t = t1;

            if(t<0){
                t = 0;
                d = c;
            }else{
                d = 0;
            }
        }

        if(d<eps) d=0;

        d=sqrt(d);

        cout.setf(ios::fixed,ios::floatfield);
       cout.precision(8);
      cout<<"Case #"<<++cnt<<": "<<d<<" "<<t<<endl;
    }
}

int main()
{
    solve();
    return 0;
}
