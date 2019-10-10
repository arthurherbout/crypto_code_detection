#include<cstdio>
#include<cmath>
using namespace std;

bool insideCircle(double x,double y,double r)
{
	return x*x+y*y<r*r;
}

double dist(double x,double y)
{
	return sqrt(x*x+y*y);
}

int main()
{
	int Z;
	scanf("%d",&Z);
	for(int z=1;z<=Z;++z)
	{
		printf("Case #%d: ",z);
		double f,R,t,r,g;
		scanf("%lf%lf%lf%lf%lf",&f,&R,&t,&r,&g);
		r+=f;
		g-=2.0*f;
		t+=f;
		if(g<=0)
		{
			printf("%.6f\n",1.0);
			continue;
		}
		double rad=R-t;
		int ile=1+(int)(rad/(2*r+g));
		//printf("rad=%.6f ile=%d\n",rad,ile);
		double p=0;
		for(int i=0;i<ile;++i)
			for(int j=0;j<ile;++j)
			{
				double x=i*(g+2*r)+r,x2=x+g;
				double y=j*(g+2*r)+r,y2=y+g;
				//printf("i=%d j=%d x=%.6f y=%.6f x2=%.6f y2=%.6f\n",i,j,x,y,x2,y2);
				if(insideCircle(x,y,rad))
				{
					if(insideCircle(x2,y2,rad))
					{
						p+=g*g;
						//printf("inside(%.6f)\n",g*g);
					}
					else
					{
						double ax,ay,bx,by;
						if(insideCircle(x,y2,rad))
						{
							ax=sqrt(rad*rad-y2*y2);
							ay=y2;
						}
						else
						{
							ax=x;
							ay=sqrt(rad*rad-x*x);
						}
						if(insideCircle(x2,y,rad))
						{
							bx=x2;
							by=sqrt(rad*rad-x2*x2);
						}
						else
						{
							bx=sqrt(rad*rad-y*y);
							by=y;
						}
						double square=(bx-x)*(ay-y);
						double triangle=0.5*(bx-ax)*(ay-by);
						double d=0.5*dist(ax-bx,ay-by);
						double segment=rad*rad*asin(d/rad)-sqrt(rad*rad-d*d)*d;
						//printf("ax=%.6f ay=%.6f bx=%.6f by=%.6f\n",ax,ay,bx,by);
						//printf("inter(%.6f) square=%.6f triangle=%.6f d=%.6f segment=%.6f\n",square-triangle+segment,square,triangle,d,segment);
						p+=square-triangle+segment;
					}
				}
			}
		//printf("p=%.6f area=%.6f\n",p,R*R*M_PI);
		p*=4;
		printf("%.6f\n",1-p/(R*R*M_PI));
	}
}
