#include <cassert>
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
#include <cmath>

using namespace std;
const double pi = 3.14159265358979324;
const double eps = 1e-7;

int main()
{
    int t;

    cin >> t;
    for (int ct = 0; ct < t; ++ct)
    {
        int n;
        vector<long long> x, y, z;
        cin >> n;
        for (int i = 0; i < n; ++i)
        {
            int xx, yy, zz;
            cin >> xx >> yy >> zz;
            x.push_back(xx);
            y.push_back(yy);
            z.push_back(zz);
        }
        bool ok = true;
        for (int i = 0; ok && i < n; ++i)
        {
            vector<double> angle;
            long long bx, by, bz, tx, ty, tz;
            if (x[i] != 0 || y[i] != 0)
            {
                bx = y[i];
                by = -x[i];
                bz = 0;
            }
            else if (x[i] != 0 || z[i] != 0)
            {
                bx = z[i];
                by = 0;
                bz = -x[i];
            }
            else
            {
                bx = 0;
                by = z[i];
                bz = -y[i];
            }
            tx = y[i] * bz - z[i] * by;
            ty = z[i] * bx - x[i] * bz;
            tz = x[i] * by - y[i] * bx;
            assert(x[i] * bx + y[i] * by + z[i] * bz == 0);
            assert(x[i] * tx + y[i] * ty + z[i] * tz == 0);
            double b = sqrt(bx * (double)bx + by * (double)by + bz * (double)bz);
            double t = sqrt(tx * (double)tx + ty * (double)ty + tz * (double)tz);
            bool antipode = false;
            for (int j = 0; j < n; ++j)
                if (j != i)
                {
                    if (x[i] * y[j] == x[j] * y[i] &&
                        x[i] * z[j] == x[j] * z[i] &&
                        y[i] * z[j] == y[j] * z[i])
                    {
                        if (x[i] * x[j] >= 0 && z[i] * z[j] >= 0 && z[i] * z[j] >= 0)
                        {
                            // Same point
                        }
                        else
                        {
                            // Antipode
                            antipode = true;
                            break;
                        }
                    }
                    double c = (x[j] * (double)bx + y[j] * (double)by + z[j] * (double)bz) / b;
                    double s = (x[j] * (double)tx + y[j] * (double)ty + z[j] * (double)tz) / t;
                    double a = atan2(c, s);
                    angle.push_back(a);
                }
            if (antipode)
            {
                continue;
            }
            sort(angle.begin(), angle.end());
            for (int i = 0; i + 1 < angle.size(); ++i)
                if (angle[i] + pi + eps < angle[i + 1])
                {
                    ok = false;
                }
            if (angle[angle.size() - 1] + eps < angle[0] + pi)
            {
                ok = false;
            }
        }

        cout << "Case #" << (ct + 1) << ": " << (ok? "YES" : "NO") << endl;
    }

    return 0;
}
