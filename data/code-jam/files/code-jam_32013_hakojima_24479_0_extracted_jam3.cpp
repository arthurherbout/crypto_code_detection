
#include <algorithm>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <queue>
#include <set>
#include <map>
#include <cstdio>
#include <cstdlib>
#include <cctype>
#include <cmath>
#include <list>

using namespace std;

#define PATH "c:\\jam3\\debug\\"

int main(void)
{
    ifstream fin(PATH "s.in");
    ofstream fout(PATH "s.out");

    char buf[1024];

    int count;
    fin >> count;
    for (int x = 1; x <= count; ++x)
    {
        double f,R, t, r, g;
        fin >> f >> R >> t >> r >> g;

        //printf("\nf:%lf R:%lf t:%lf r:%lf g:%lf\n", f, R, t, r, g);

        double ans = 0;

        if (g <= 2 * f
            || r + f >= R - (t + f))
        {
            ans = 1.0F;
        }
        else
        {
            double newR = R - (t + f); // => 1.0
            double r1 = (r + f) / newR;
            double g1 = (g - 2 * f) / newR;

            double SA = 0;
            for (int i = 0; i < 1000; ++i)
            {
                double S = 0;
                double st, en;

                // vertical string
                switch (i)
                {
                case 0:
                    st = 0;
                    break;
                case 1:
                    st = r1 + g1;
                    break;
                default:
                    st = r1 + g1 * i + 2 * r1 * (i-1);
                    break;
                }
                if (st >= 1.0)
                    break;

                if (i == 0)
                    en = st + r1;
                else
                    en = st + 2 * r1;
                if (en > 1.0F)
                    en = 1.0F;

                S += (en*sqrt(1 - en * en) + asin(en))/2 + (-(st*sqrt(1 - st*st)) - asin(st))/2;

                //printf("%d: st(%lf,%lf)=%lf ", i, st, en, S);

                // gap & horizontal string
                st = en;
                if (st >= 1.0)
                {
                    SA += S;
                    break;
                }
                en = st + g1;
                if (en > 1.0) 
                    en = 1.0;
                //printf("(%lf,%lf)", st, en);

                for (int j = 0; j < 1000; j++)
                {
                    double st2, en2;
                    switch (j)
                    {
                    case 0:
                        st2 = 0;
                        break;
                    case 1:
                        st2 = r1 + g1;
                        break;
                    default:
                        st2 = r1 + g1 * j + 2 * r1 * (j - 1);
                        break;
                    }
                    if (st2*st2 + st*st >= 1.0)
                        break;

                    if (j == 0)
                        en2 = st2 + r1;
                    else
                        en2 = st2 + 2 * r1;

                    //printf("h:%d:(%lf,%lf)\n",j, st2,en2);
                    if (en2*en2 + en*en < 1.0) // not over
                    {
                        //printf("not over\n");
                        S += (en - st) * (en2 - st2);
                    }
                    else // over
                    {
                        if (st2*st2 + en*en > 1.0) // startline is over at EN
                        {
                            en = sqrt(1 - st2*st2);
                        }

                        {
                            if (en2*en2 + st*st < 1.0) // endline is not over at ST
                            {
                                double point = sqrt(1 - en2*en2);
                                S += (point - st) * (en2 - st2);
                                //printf("point:%lf %lf\n", point, S);
                                {
                                    double s = point;
                                    double e = en;
                                    S += (e*sqrt(1 - e * e) + asin(e))/2 + (-(s*sqrt(1 - s*s)) - asin(s))/2;
                                    //printf("c:%lf\n", (e*sqrt(1 - e * e) + asin(e))/2 + (-(s*sqrt(1 - s*s)) - asin(s))/2);
                                    //printf("c2:%lf\n", S);
                                    S -= (e - s) * st2;
                                }
                            }
                            else // endline is over at ST
                            {
                                //printf("point2\n");
                                {
                                    double s = st;
                                    double e = en;
                                    S += (e*sqrt(1 - e * e) + asin(e))/2 + (-(s*sqrt(1 - s*s)) - asin(s))/2;
                                    S -= (e - s) * st2;
                                }
                            }
                        }
                    }

                }
                //printf("PS:%lf\n", S);
                SA += S;
            }
            SA /= 3.14159265358979323846264338 / 4;
            double innerS;
            innerS = R - (t + f);
            innerS = innerS * innerS;
            ans = 1 - ((1 - SA) * innerS / (R*R));
            //printf("S:%lf ans:%lf\n", SA, ans);
        }
{
        char buf[1024];
sprintf(buf, "%6.6lf", ans);
        fout << "Case #" << x << ": " << buf << "\n";
}
    }

    return 0;
}

