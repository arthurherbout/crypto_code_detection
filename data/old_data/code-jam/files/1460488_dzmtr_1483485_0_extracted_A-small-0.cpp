#include <iostream>
#include <string>
#include <tr1/unordered_map>

using namespace std;
using namespace std::tr1;

int main()
{
	ios_base::sync_with_stdio(false);
	cin.tie(0);
	unordered_map<char, char> f;
	f['y'] = 'a';
	f['n'] = 'b';
	f['f'] = 'c';
	f['i'] = 'd';
	f['c'] = 'e';
	f['w'] = 'f';
	f['l'] = 'g';
	f['b'] = 'h';
	f['k'] = 'i';
	f['u'] = 'j';
	f['o'] = 'k';
	f['m'] = 'l';
	f['x'] = 'm';
	f['s'] = 'n';
	f['e'] = 'o';
	f['v'] = 'p';
	f['z'] = 'q';
	f['p'] = 'r';
	f['d'] = 's';
	f['r'] = 't';
	f['j'] = 'u';
	f['g'] = 'v';
	f['t'] = 'w';
	f['h'] = 'x';
	f['a'] = 'y';
	f['q'] = 'z';
	f[' '] = ' ';
	int T;
	cin >> T;
	cin.get();
	for (int t = 1; t <= T; ++t)
	{
		string in, out;
		getline(cin, in);
		for (string::const_iterator i = in.begin(); i != in.end(); ++i)
			out.push_back(f[*i]);
		cout << "Case #" << t << ": " << out << '\n';
	}
}
