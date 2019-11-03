#include <iostream>
#include <fstream>

using namespace std;

double solve(double d, double v, double a)
{
	return (-v + sqrt(v*v - 2*d*a))/a;
}

int main()
{
	ifstream fi("input.txt");
	ofstream fo("output.txt");
	
	int t;
	fi>>t;
	for (int i=0; i<t; i++)
	{
		double d;
		int n, a;
		fi>>d>>n>>a;
		double *x = new double[n];
		double *t = new double[n];
		for (int j=0; j<n; j++)
		{
			fi>>t[j]>>x[j];
		}
		fo<<"Case #"<<i+1<<":"<<endl;
		for (int j=0; j<a; j++)
		{
			double ac;
			fi>>ac;
			double rt = 0.0f;
			double p = 0.0f;
			double v = 0.0f;
			bool home = false;
			
			for (int k=1; k<n; k++)
			{
				double dt = t[k] - t[k-1];
				double max = p + v*dt + 0.5f*ac*dt*dt;
				
				if (x[k] >= max)
				{
					if (max > d)
					{
						// home
						p = d;
						rt += solve(p-d, v, ac);
						
						cout<<'('<<t[k-1]<<", "<<t[k]<<")"<<endl;
						cout<<"p = "<<p<<endl;
						cout<<"v = "<<v<<endl;
						
						home = true;
						break;
					}
					else
					{
						p = max;
						v = dt * ac;
						rt += dt;
					}
				}
				else
				{
					// intersect time
					double sl = (x[k]-x[k-1])/(t[k]-t[k-1]);
					double it = solve(p-x[k-1], v-sl, ac);
					cout<<"intersect at "<<it<<endl;
					
					if (d >= x[k])
					{
						p = x[k];
						v = sl;
						rt += dt;
					}
					else
					{
						if (d < x[k-1]+sl*it)
						{
							rt += solve(p-d, v, ac);
							p = d;
							
							cout<<'('<<t[k-1]<<", "<<t[k]<<")"<<endl;
							cout<<"p = "<<p<<endl;
							cout<<"v = "<<v<<endl;
							
							home = true;
							break;
						}
						else
						{
							rt += (d-x[k-1])/sl;
							p = d;
							
							cout<<'('<<t[k-1]<<", "<<t[k]<<")"<<endl;
							cout<<"p = "<<p<<endl;
							cout<<"v = "<<v<<endl;
							
							home = true;
							break;
						}
					}
				}
				cout<<'('<<t[k-1]<<", "<<t[k]<<")"<<endl;
				cout<<"p = "<<p<<endl;
				cout<<"v = "<<v<<endl;
			}
			if (home)
				fo<<rt<<endl;
			else
				fo<<rt+solve(p-d, v, ac)<<endl;
		}
		
		
	}
	
	return 0;
}