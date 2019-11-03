#include <cstdio>
#include <vector>
#include <map>
#include <unordered_map>
#include <algorithm>
#include <math.h>

using namespace std ;

const int Tm = 2000+100 ;
double home; 
int nbPoints,nbAcc ;
double timE[Tm] ;
double dist[Tm] ;

double evalDelay( double a)
{
  double timETo , delay=0 ;
  for( int i = 0 ; i < nbPoints ; i++ )
    
    {
      // in timETo seconds we are at dist
      timETo = sqrt(dist[i]/(0.5*a));

      // we arrive  timE[i]-timETo  before the other car
      delay = max(timE[i]-timETo,delay) ;
    }
  return delay;
}

double timEToHome(double a)
{
  return sqrt(home/(0.5*a));
}

void algo()
{
  scanf("%lf %d %d",&home,&nbPoints,&nbAcc);
  for( int i = 0 ; i < nbPoints ; i++ )
    scanf("%lf %lf",timE+i,dist+i);
  while( nbPoints && dist[nbPoints-1] > home )
    nbPoints--;
  if( nbPoints > 0 && dist[nbPoints]>home )
    {
      timE[nbPoints] = (timE[nbPoints]-timE[nbPoints-1])*(home-dist[nbPoints-1])/(dist[nbPoints]-dist[nbPoints-1]) ;
      dist[nbPoints++] = home ;
    }
  for( int a = 0 ; a < nbAcc ; a++ )
    {
      double Acc ;
      scanf("%lf",&Acc);
      printf("%.9lf\n",evalDelay(Acc)+timEToHome(Acc));
    }
}

int main()
{
  int t;
  scanf("%d",&t);
  for(int i = 1 ; i <= t ; i++ )
    {
      printf("Case #%d:\n",i);
      algo();
    }
  return 0;
}
