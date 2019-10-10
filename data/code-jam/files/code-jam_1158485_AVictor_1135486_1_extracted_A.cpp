#include<iostream>
#include<cstdio>
#include<vector>
#include<set>
#include<algorithm>
using namespace std;

typedef pair<int,int> ii;
struct gr
{
       int x;
       double y1,y2;
       gr() {};
       gr(int x, double y1, double y2): x(x), y1(y1), y2(y2) {};
};

int main()
{
    int test_count;
    cin>>test_count;
    for(int test_num=0;test_num<test_count;test_num++)
    {
            int W,L,U,G;
            cin>>W>>L>>U>>G;
            vector<ii> lo(L), up(U);
            for(int i=0;i<L;i++)
                    scanf("%d%d",&lo[i].first,&lo[i].second);
            for(int i=0;i<U;i++)
                    scanf("%d%d",&up[i].first,&up[i].second);
                    
            vector<gr> mas;
            mas.push_back(gr(0,lo[0].second,up[0].second));
            int p1 =1;
            int p2 = 1;
            for(int x=1;x<=W;x++)
            {
                    if (lo[p1].first!=x && up[p2].first!=x) continue;
                    if (lo[p1].first==x && up[p2].first==x)
                    {
                                        mas.push_back(gr(x,lo[p1].second,up[p2].second));
                                        p1++;
                                        p2++;
                                        continue;                 
                    }
                    if (lo[p1].first==x)
                    {
                                        double y = double(up[p2].second*(x-up[p2-1].first) + up[p2-1].second*(up[p2].first-x))/(up[p2].first-up[p2-1].first);
                                        mas.push_back(gr(x,lo[p1].second,y));
                                        p1++;
                                        continue;                 
                    }
                    if (up[p2].first==x)
                    {
                                        double y = double(lo[p1].second*(x-lo[p1-1].first) + lo[p1-1].second*(lo[p1].first-x))/(lo[p1].first-lo[p1-1].first);
                                        mas.push_back(gr(x,y,up[p2].second));
                                        p2++;
                                        continue;                 
                    }    
            }
            double S = 0;
            int N = mas.size();
            for(int i=1;i<N;i++)
            {
                    double h1 = mas[i].y2-mas[i].y1;
                    double h2 = mas[i-1].y2-mas[i-1].y1;
                    double dx = mas[i].x-mas[i-1].x;
                    S+=dx*(h1+h2)/2;
            }
                
            printf("Case #%d:\n",test_num+1);
            int p=0;
            double Snow=0;
            for(int i=1;i<G;i++)
            {
                    double Sx = i*S/G;
                    double h1,h2,dx,S1;
                    
            lab:
                    h1 = mas[p+1].y2-mas[p+1].y1;
                    h2 = mas[p].y2-mas[p].y1;
                    dx = mas[p+1].x-mas[p].x;
                    S1 = dx*(h1+h2)/2;
                    if (Snow+S1<=Sx)
                    {
                                    Snow+=S1;
                                    p++;
                                    goto lab;
                    }
                    
                    double A = mas[p].x;
                    double B = mas[p+1].x;
                    double eps = 1e-7;
                    while(B>A+eps)
                    {
                                  double C = (A+B)/2;
                                  double hx = (h1*(C-mas[p].x)+h2*(mas[p+1].x-C))/(mas[p+1].x-mas[p].x);
                                  double Sxx = (C-mas[p].x)*(hx+h2)/2;
                                  if (Snow+Sxx>Sx)
                                    B=C;
                                  else
                                    A=C;          
                    }
                    printf("%.7lf\n",A);                 
                    
            }
    }
    
    
    return 0;
}
