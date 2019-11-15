#include<iostream>
#include<conio.h>
#include <iomanip>

using namespace std;
int main()
{
    int test_cases;
    freopen ("A-large.in","r",stdin);
    freopen ("A-large.out","w",stdout);
    cin>>test_cases;
    
    for(int test = 0;test<test_cases;test++)
    {
                 unsigned int d,n;
                 cin>>d>>n;
                 double t = 0,res,k,s;
                 for(int i=0;i<n;i++)
                 {
                         cin>>k;
                         cin>>s;
                         if((d-k)/s > t)
                         {
                                    t = (d-k)/s;
                         }
                 }
                 res = d/t;
                 std::cout << std::fixed;
                 std::cout << std::setprecision(6);
                 cout<<"Case #"<<test+1<<": "<<res<<endl;
    }
    return 0;
}
                         
                 
