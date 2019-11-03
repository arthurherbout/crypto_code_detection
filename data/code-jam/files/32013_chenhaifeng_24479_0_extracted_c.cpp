#include <stdio.h>
#include <math.h>
#define eps 0.0000001
#define PI acos(-1.0)
double cal(double lx, double ly, double rx, double ry, double R)
{ 
	double tmpAng = acos(rx / sqrt(rx * rx + ry * ry)) 
		- acos(lx / sqrt(lx * lx + ly * ly)); 
	//printf("ang 1 %lf \n",  ry); 
	double area = R * R * tmpAng / 2; 
	double a = sqrt(rx * rx + ry * ry), b = sqrt(lx * lx + ly * ly), 
		c = sqrt((rx - lx) * (rx - lx) + (ry - ly) * (ry - ly)); 
	double p = (a + b + c) / 2; 
	//if (area - sqrt(p * (p - a) * (p - b) * (p - c)) > 999999)
		//printf("%lf \n", area - sqrt(p * (p - a) * (p - b) * (p - c))); 
	return area - sqrt(p * (p - a) * (p - b) * (p - c));
}
int main()
{ 
	freopen("C-small-attempt0.in", "r", stdin); 
	freopen("outcchf.txt", "w", stdout); 
	int T, tcnt = 0; 
	scanf("%d", &T); 
	double f, R, t, r, g; 
	while (T--) 
	{ 
		scanf("%lf%lf%lf%lf%lf", &f, &R, &t, &r, &g); 
		if (g - 2 * f <= eps) 
		{
			printf("Case #%d: 1.000000\n", ++tcnt); 
			continue; 
		}
		double len = (g - 2 * f); 
		double area = 0; 
		double x = 0, y = 0; 
		//for (x = 0; x < 9000; x += g + 2 * r);
		for (x = 0; x < R - t - r - eps; x += g + 2 * r)
		{ 
			//for (y = 0; y < 4000; y += g + 2 * r);
			for (y = 0; (x + r) * (x + r) + (y + r) * (y + r) < (R - t) * (R - t) - eps; 
				y += g + 2 * r)
			{ 
				//printf("%lf %lf     ", x, y); 
				double lx = x + f + r, ly = y + f + r, 
					rx = x + g + r - f, ry = y + g + r - f; 
				double nR = R - f - t, tarea = len * len; 
				if (lx * lx + ly * ly >= nR * nR) 
					continue; 
				if (ly * ly + rx * rx <= nR * nR && sqrt(nR * nR - rx * rx) >= ly && sqrt(nR * nR - rx * rx) <= ry)
				{ 
					if (lx * lx + ry * ry <= nR * nR && sqrt(nR * nR - ry * ry) >= lx && sqrt(nR * nR - ry * ry) <= rx)
					{ 
						double cy = sqrt(nR * nR - rx * rx), cx = sqrt(nR * nR - ry * ry); 
						tarea = len * (cx - lx) + (cy - ly + len) * (rx - cx) / 2; 
						tarea += cal(rx, cy, cx, ry, nR); 
					}
					else
						if (lx * lx + ly * ly <= nR * nR && sqrt(nR * nR - lx * lx) >= ly 
							&& sqrt(nR * nR - lx * lx) <= ry)
						{
							double cy1 = sqrt(nR * nR - lx * lx), cy2 = sqrt(nR * nR - rx * rx); 
							tarea = (cy1 - ly + cy2 - ly) * len / 2; 
							tarea += cal(rx, cy2, lx, cy1, nR);
						}
				}
				else
					if (lx * lx + ly * ly <= nR * nR && sqrt(nR * nR - ly * ly) >= lx
						&& sqrt(nR * nR - ly * ly) <= rx)
					{
						if (lx * lx + ry * ry <= nR * nR && sqrt(nR * nR - ry * ry) >= lx 
							&& sqrt(nR * nR - ry * ry) <= rx)
						{ 
							double cx1 = sqrt(nR * nR - ly * ly), 
								cx2 = sqrt(nR * nR - ry * ry); 
							tarea = (cx1 - lx + cx2 - lx) * len / 2; 
							tarea += cal(cx1, ly, cx2, ry, nR); 
						}
						else
							if (lx * lx + ly * ly <= nR * nR && sqrt(nR * nR - lx * lx) >= ly 
								&& sqrt(nR * nR - lx * lx) <= ry)
							{
								double cx = sqrt(nR * nR - ly * ly), 
									cy = sqrt(nR * nR - lx * lx); 
								tarea = (cx - lx) * (cy - ly) / 2; 
								tarea += cal(cx, ly, lx, cy, nR);
							}
					}
				//if (tarea > (g - 2 * r - 2 * f) * (g - 2 * r - 2 * f) + eps)
					//printf("%lf %lf %lf %lf\n", x, y, tarea, (g - 2 * r - 2 * f) * (g - 2 * r - 2 * f)); 
				area += tarea; 
			}
			//printf(" * %lf\n", area); 
		}
		//printf("%lf %lf\n", PI * R * R / 4, area); 
		printf("Case #%d: %.6lf\n", ++tcnt, (PI * R * R / 4 - area) / (PI * R * R / 4));
	}
}
