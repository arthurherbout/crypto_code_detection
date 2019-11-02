#include<iostream>
#include<cstdio>
#include<string>
#include<sstream>
#include<iomanip>
#include<map>
#include<vector>
#include<queue>
#include<set>
#include<algorithm>
#include<memory.h>
#include<iomanip>
using namespace std;

typedef pair<int,int> ii;

const int size = 105;
int N;
int S[size], P[size];
bool pol[size];

double eps = 1e-5;

int main()
{
	int T;
	cin>>T;
	for(int test=1;test<=T;test++)
	{
		cin>>N;
		for(int i=0;i<N;i++)
		{
			string s;
			cin>>s;
			pol[i]=(s=="R");
			cin>>S[i]>>P[i];
		}

		double T = 1e100;
		for(int i=0;i<N;i++)
			for(int j=0;j<N;j++)
			{
				if (i==j) continue;
				if (S[i]==S[j]) continue;
				if (S[i]>S[j])
				{
					double dS = S[i]-S[j];
					double t1 = (P[j]-P[i]-5)/dS;
					if (t1>=eps && (pol[i]==pol[j]))
					{
						double xi = P[i]+t1*S[i];
						double xj = P[j]+t1*S[j];
						bool cani=true, canj = true;

						for(int k=0;k<N;k++)
						{
							if (k==i || k==j) continue;
							double xk = P[k]+t1*S[k];
							if (xk>=xi-5+eps && xk <= xj-eps)
								cani=false;
							else if (abs(xk-xi+5)<eps && S[k]>S[i])
								cani=false;
							else if (abs(xk-xj)<eps && S[k]<S[i])
								cani=false;

							if (xk>=xi+eps && xk <= xj+5-eps)
								canj=false;
							else if (abs(xk-xi)<eps && S[k]>S[j])
								canj=false;
							else if (abs(xk-xj-5)<eps && S[k]<S[j])
								canj=false;
						}

						if (!cani & !canj)
							T = min(T, t1);
					}

				}

			}


		if (T>1e90)
			printf("Case #%d: Possible\n", test);
		else
			printf("Case #%d: %.6lf\n", test, T);

	}
	
	return 0;
}