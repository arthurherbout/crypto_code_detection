#include <iostream>
#include <vector>
#include <cmath>
#include <cstring>
#include <set>
#include <map>

using namespace std;

vector<long> factorise(long x, long p)
{
    vector<long> factors;
    long t = x;
    if ((t & 1) == 0)
    {
        if (p == 2)
            factors.push_back(2);
        do
        {
            t  = t >> 1;
        } while ((t & 1) == 0);
    }
    long l = (long) sqrt(t);
    long f = 3;
    while (f <= l)
    {
        if ((t % f) == 0)
        {
            if (f >= p)
                factors.push_back(f);
            do
            {
                t = t / f;
            } while ((t % f) == 0);
            l = (long) sqrt(t);
        }

        f += 2;
    }
    if (t >= p)
        factors.push_back(t);

    return factors;
}

int main()
{
    int C;
    cin >> C;
    for (int c = 1; c <= C; c++)
    {
        long a, b, p;
        cin >> a >> b >> p;

        map<long, vector<long> > primes;
        vector<vector<long> > nums;
        int count = 0;
        int n = (b - a);
        for (int i = a; i <= b; i++)
        {
            vector<long> factors = factorise(i, p);
            nums.push_back(factors);
            int num = i - a;
            for (unsigned int j = 0; j < factors.size(); j++)
                primes[factors[j]].push_back(num);
        }

        bool done[1000000];
        memset(done, 0, sizeof(done));
        set<long> pdone;
        for (int i = 0; i <= n; i++)
        {
            if (!done[i])
            {
                //cerr << "* " << i << endl;
                done[i] = true;
                count++;
                vector<long> q, pq;
                q.push_back(i);
                while (!q.empty() || !pq.empty())
                {
                    if (!q.empty())
                    {
                        int x = q.back();
                        //cerr << "doing " << x << endl;
                        q.pop_back();
                        for (unsigned int p = 0; p < nums[x].size(); p++)
                        {
                            int pr = nums[x][p];
                            //cerr << "considering prime " << pr << endl;
                            if (pdone.find(pr) != pdone.end())
                                continue;
                            pq.push_back(pr);
                            pdone.insert(pr);
                        }
                        done[x] = true;
                    }
                    if (!pq.empty())
                    {
                        int y = pq.back();
                        //cerr << "doing prime " << y << endl;
                        pq.pop_back();
                        vector<long> &k = primes[y];
                        for (unsigned int j = 0; j < k.size(); j++)
                        {
                            int num = k[j];
                            if (!done[num])
                            {
                                done[num] = true;
                                q.push_back(num);
                            }
                        }
                    }
                }
            }
        }

        cout << "Case #" << c << ": " << count << endl;
    }
    return 0;
}
