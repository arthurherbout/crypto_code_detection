#include <iostream>
#include <vector>
#include <algorithm>
#include <set>
#include <map>
#include <string>
#include <cmath>
using namespace std;

int main ()
{
	freopen("input.txt","r",stdin);
	freopen("output.txt","w",stdout);
	
	int t;
	cin >> t;	
	
	for (int k=0; k<t; k++)
	{
		int n;
		
		long double x, y, z, vx, vy, vz;
		
		cin >> n;
		
		x = 0;
		y = 0;
		z = 0;
		vx = 0;
		vy = 0;
		vz = 0;
		
		for (int i = 0; i<n; i++)
		{		
			int qx, qy, qz, qvx, qvy, qvz;
			cin >> qx >> qy >>  qz >> qvx >> qvy >> qvz;
		
			x += qx;
			y += qy;
			z += qz;
			vx += qvx;
			vy += qvy;
			vz += qvz;
		}
	
		x /= n;
		y /= n;
		z /= n;
		vx /= n;
		vy /= n;
		vz /= n;
		
		long double T = 0;
		
		if (vx * vx + vy * vy + vz * vz != 0)
		{
			T = -(x * vx + y * vy + z * vz) / (vx * vx + vy * vy + vz * vz);
		}
		
		cout.setf(ios::fixed);
		cout.precision(8);
		
		if (T<0) T = 0;
		
		if (sqrt((x + vx * T) * (x + vx * T) + (y + vy * T) * (y + vy * T) + (z + vz * T) * (z + vz * T))>
			sqrt((x + vx * 0) * (x + vx * 0) + (y + vy * 0) * (y + vy * 0) + (z + vz * 0) * (z + vz * 0)))
		{
			cout << "Case #" << k + 1 << ": " << sqrt((x + vx * 0) * (x + vx * 0) + (y + vy * 0) * (y + vy * 0) + (z + vz * 0) * (z + vz * 0)) << " " << 0 << endl;
		}
		else
		{
			cout << "Case #" << k + 1 << ": " << sqrt((x + vx * T) * (x + vx * T) + (y + vy * T) * (y + vy * T) + (z + vz * T) * (z + vz * T)) << " " << T << endl;
		}
	}
	
    return 0;
}
