#include <iostream>
#include <string>
#include <sstream>
#include <map>
#include <algorithm>
#include <vector>
#include <cmath>

using namespace std;


int gcd(int a, int b)
{
	if (b==0) return a;
	else return gcd(b,a%b);
}

int main()
{
	int T;
	cin >> T;

	for(int i = 0; i < T ; ++i)
	{
		int H, W, D;
		cin>>H>>W>>D;
		vector<string> mas(H);
		double x0, y0;
		for(int i=0;i<H;i++)
		{
			cin>>mas[i];
			for(int j=0;j<W;j++)
				if (mas[i][j]=='X')
				{
					x0=i-1+0.5;
					y0=j-1+0.5;
				}
		}
		W-=2;
		H-=2;

		int ans = 0;
		double eps=1e-5;
		if (2*x0<=D+eps) ans++;
		if (2*y0<=D+eps) ans++;
		if (2*(H-x0)<=D+eps) ans++;
		if (2*(W-y0)<=D+eps) ans++;
		for(int _dx=-50;_dx<=50;_dx++)
			for(int _dy=-50;_dy<=50;_dy++)
			{
				if (_dx==0 || _dy==0) continue;
				if (gcd(abs(_dx), abs(_dy))>1) continue;
				double dx=_dx, dy=_dy;
				double x=x0, y=y0;
				double l=0;
				while(l<=D)
				{
					if (l>eps && abs((y0-y)/dy - (x0-x)/dx)<eps)
					{
						l+=sqrt(pow(x0-x,2)+pow(y0-y,2));
						if (l<=D+eps) 
							ans++;
						break;
					}
				
					double x2=0,y2=0,dl=1e9;
					double dx2, dy2;
					if (dx<0)
					{
						double _t = -x/dx;
						double _x2 = 0;
						double _y2 = y+dy*_t;
						double _dl = sqrt(pow(_x2-x, 2)+pow(_y2-y, 2));
						if (_dl<dl)
						{
							dl=_dl;
							x2=_x2;
							y2=_y2;
							dx2=-dx;
							dy2=dy;
						}
					}
					if (dy<0)
					{
						double _t = -y/dy;
						double _y2 = 0;
						double _x2 = x+dx*_t;
						double _dl = sqrt(pow(_x2-x, 2)+pow(_y2-y, 2));
						if (_dl<dl)
						{
							dl=_dl;
							x2=_x2;
							y2=_y2;
							dx2=dx;
							dy2=-dy;
						}
					}
					if (dx>0)
					{
						double _t = (H-x)/dx;
						double _x2 = H;
						double _y2 = y+dy*_t;
						double _dl = sqrt(pow(_x2-x, 2)+pow(_y2-y, 2));
						if (_dl<dl)
						{
							dl=_dl;
							x2=_x2;
							y2=_y2;
							dx2=-dx;
							dy2=dy;
						}
					}
					if (dy>0)
					{
						double _t = (W-y)/dy;
						double _y2 = W;
						double _x2 = x+dx*_t;
						double _dl = sqrt(pow(_x2-x, 2)+pow(_y2-y, 2));
						if (_dl<dl)
						{
							dl=_dl;
							x2=_x2;
							y2=_y2;
							dx2=dx;
							dy2=-dy;
						}
					}
					l+=dl;
					x=x2;
					y=y2;
					dx=dx2;
					dy=dy2;
				}
			}

		cout << "Case #" << i + 1 << ": " << ans << endl;

	}

	return 0;
}