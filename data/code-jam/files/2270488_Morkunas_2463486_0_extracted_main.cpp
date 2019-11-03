#include <iostream>
#include <cstring>
#include <cmath>
#include <fstream>

using namespace std;



bool apvertimas(long long int a);

int main()
{
    ifstream input ("C-small-attempt0.in");
    ofstream out ("C-small-attempt0.out");

    int n;
    input >> n;

    long long int m,e;
    e=10000000;
    int d=0;
    long long int mas[50];
    mas[0]=9;
    int q=0;


  for (long long int i=1;i <= e; i++)
    {
        if ((i*i%10==1) || (i*i%10==4))
        {

        if ((apvertimas(i)==true)&&(apvertimas(i*i)==true))
        {
        q=q+1;
        mas[q]=i*i;
                d++;
        }
        }


    }
    for(int p = 0; p<n;p++)
    {

    long long int a,b;
    input >> a >> b;
    int suma=0;
    for (int i = a; i <=b; i++)
    {
        if ((i%10==1) || (i%10==4) || (i==9))
            {
                for (int y = 0; y <=q+1; y++)
                {
                    if(mas[y]==i)
                    {
                        suma++;
                    }
                }
            }
    }
    out <<"Case #"<< p+1 <<": " << suma << endl;
    }
    input.close();
    out.close();

    return 0;
}


bool apvertimas(long long int a)
{   long long int b=0;
    long long int c=0,s = 1;
    long long int d=a,e;
    int long long r=a;
    long long int f;
    while(a!=0)
    {
        a=a/10;
        c++;
    }
    f=c;

    for (long long int i = 1; i<=c;i++)
    {
        e=d%10;
        for(long long int y = f; y > 1; y=y-1)
        {
            s=s*10;
        }
        b=b+s*e;
        d=d/10;
        s=1;
        f=f-1;
    }
    if(r==b)
    {
    return true;
    }
    else{
    return false;
    }

}
