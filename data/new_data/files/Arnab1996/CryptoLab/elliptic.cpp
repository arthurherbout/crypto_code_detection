#include<bits/stdc++.h>
using namespace std;
int modInverse(int a, int m)
{
    int m0 = m, t, q;
    int x0 = 0, x1 = 1;
    if (m == 1)
      return 0;
    while (a > 1)
    {
        q = a / m;
        t = m;
        m = a % m, a = t;
        t = x0;
        x0 = x1 - q * x0;
        x1 = t;
    }
    if (x1 < 0)
       x1 += m0;
    return x1;
}
int main()
{
    int a,b,px,py,m;
    cout<<"Enter the Elliptical Domain values: ->a ->b and Modulo ->m  :\n";
    cin>>a>>b>>m;
    cout<<"Enter the points' X-Values :\n";
    cin>>px>>py;
    int mm = (((3*px*px+a)%m)*(modInverse(2*py,m)))%m;
    int x3= (mm*mm -px-px+100*m)%m;
    int y3 = (mm*(px-x3)-py+100*m)%m;
    cout<<"The Initial point P is (X, Y) : "<<x3<<" "<<y3<<"\n";
    int x1,x2,y1,y2;
    x1=px;
    y1=py;
    x2=x3;
    y2=y3;
    cout<<"The other scalar multiplication points are (X, Y) :\n";
    for(int i=0;i<4;i++)
    {
        int mm = (((y2-y1)%m)*(modInverse(x2-x1,m)))%m;
        int x3= (mm*mm -x1-x2+m)%m;
        int y3 = (mm*(x1-x3)-y1+m)%m;
        x2=x3;
        y2=y3;
        cout<<(i+1)<<" -> "<<x3<<" "<<y3<<"\n";
    }
}
/*
1 1 23
3 10
*/
