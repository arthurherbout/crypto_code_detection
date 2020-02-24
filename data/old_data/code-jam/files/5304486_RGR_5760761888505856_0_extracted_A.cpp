#include <bits/stdc++.h>
using namespace std;
using ll = long long;
using vl = vector<ll>;
using vvl=vector<vl>;
using vb=vector<bool>;
using vs=vector<string>;
using pll=pair<ll,ll>;
const ll oo = 0x3f3f3f3f3f3f3f3fLL;
const double eps = 1e-9;
#define sz(c) ll((c).size())
#define all(c) begin(c),end(c)
#define mp make_pair
#define mt make_tuple
#define pb push_back
#define eb emplace_back
#define xx first
#define yy second
#define FR(i,a,b) for(ll i = (a); i < (b); i++)
#define FRD(i,a,b) for(ll i = ll(b)-1;i>=(a);i--)
#define TR(X) ({if(1) cerr << "TR: " << (#X) << " = " << (X) << endl; })

int main() {
	ios_base::sync_with_stdio(false);
	int tc;
	cin >> tc;
	FR(a,0,tc) {
		int rows, cols;
		cin >> rows >> cols;
		cin.ignore();
		vs grid(rows);
		map<char, bool> done;
		FR(b,0,rows) {
			getline(cin, grid[b]);
		}
		FR(b,0,rows) {
			FR(c,0,cols) {
				if(grid[b][c] != '?' && !done[grid[b][c]]) {
					done[grid[b][c]] = true;
					int left, right;
					left = right = c;
					for(int d = c-1; d >= 0; d--) {
						if(grid[b][d] == '?') {
							grid[b][d] = grid[b][c];
							left = d;
						} else {
							break;
						}
					}
					for(int d = c+1; d < cols; d++) {
						if(grid[b][d] == '?') {
							grid[b][d] = grid[b][c];
							right = d;
						} else {
							break;
						}
					}
					for(int e = b-1; e >= 0; e--) {
						if(grid[e].find_first_not_of("?", left) > right) {
							grid[e].replace(left, right-left+1, right-left+1, grid[b][c]);
						} else {
							break;
						}
					}
					for(int e = b+1; e < rows; e++) {
						if(grid[e].find_first_not_of("?", left) > right) {
							grid[e].replace(left, right-left+1, right-left+1, grid[b][c]);
						} else {
							break;
						}
					}
				}
			}
		}
		cout << "Case #" << a+1 << ":\n";
		FR(b,0,rows) {
			cout << grid[b] << '\n';
		}
	}
}
