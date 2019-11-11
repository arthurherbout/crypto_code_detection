#include <fstream>
#include <vector>

using namespace std;

int main()
{
	ifstream in("D-small-0.in");
	ofstream out("D-small-0.out");
	out.precision(6);
	int T, N;
	vector<int> a;
	double ans;
	in >> T;
	for (int t = 1; t < T + 1; ++t)
	{
		in >> N;
		a.resize(N);
		for (int i = 0; i < N; i++)
		{
			in >> a[i];
			--a[i];
		}

		ans = 0.0;

		for (int i = 0; i < N; ++i)
		for (int j = 0; j < N; ++j)
		{
			if (a[j] == j) continue;
			swap(a[j], a[a[j]]);
			ans += 2.0;
		}

		out << "Case #" << t << ": " << ans << '\n';
	}
}
