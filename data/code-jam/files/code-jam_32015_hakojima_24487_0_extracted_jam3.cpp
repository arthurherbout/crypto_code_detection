
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

#define PATH "c:\\jam3\\jam3\\"

int ans;
void check(char *buf, int len, long long cal, long long prev, int pos, bool minus)
{
	if (len == pos)
	{
		if (minus)
			cal = cal - prev;
		else
			cal = cal + prev;

		if (cal == 0
			|| (cal % 2 == 0)
			|| (cal % 3 == 0)
			|| (cal % 5 == 0)
			|| (cal % 7 == 0))
		{
			//printf("ANS:%d UG\n", cal);
			ans ++;
			return;
		}
		//printf("ANS:%d\n", cal);

		return;
	}

	// 
	check(buf, len, cal, prev * 10 + buf[pos] - '0', pos + 1, minus);

	if (minus)
		cal = cal - prev;
	else
		cal = cal + prev;

	// +
	check(buf, len, cal, buf[pos] - '0', pos+1, false);

	// -
	check(buf, len, cal, buf[pos] - '0', pos+1, true);

}

int main(void)
{
    ifstream fin(PATH "s.in");
    ofstream fout(PATH "s.out");

    string line;

	char buf[128];
	int i;
    int count;
    fin >> count;
	fin.getline(buf, 100);;
    for (int t = 1; t <= count; ++t)
    {
		fin.getline(buf, 100);
		for (i =0 ; i < 100; i++)
			if (buf[i] <'0' || buf[i] >'9') buf[i] = 0;
		int len = (int)strlen(buf);
		//printf("Q:%s\n", buf);
		ans = 0;
		check(buf, len, 0, buf[0] - '0', 1, false);
		//printf("%s %d\n", buf, len);

        fout << "Case #" << t << ": " << ans << endl;
    }

    return 0;
}

