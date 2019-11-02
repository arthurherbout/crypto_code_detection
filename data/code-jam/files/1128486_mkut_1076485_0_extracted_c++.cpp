#include <iostream>
#include <vector>
using namespace std;

bool ok(vector<int>& freq, int f)
{
	for(int i = 0; i < freq.size(); i++)
		if(freq[i] % f != 0 && f % freq[i] != 0)
			return false;
	return true;
}

int main()
{
	int T; cin >> T;
	for(int No = 1; No <= T; No++)
	{
		int N, L, H; cin >> N >> L >> H;
		vector<int> freq(N);
		for(int i  = 0; i < N; i++)
			cin >> freq[i];
		
		int ans = -1;
		for(int i = L; i <= H; i++)
		{
			if(ok(freq, i))
			{
				ans = i;
				break;
			}
		}
		
		cout << "Case #" << No << ": ";
		if(ans == -1)
			cout << "NO" << endl;
		else
			cout << ans << endl;
	}
}
