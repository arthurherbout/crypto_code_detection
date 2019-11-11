#include <iostream>
#include <iomanip>
#include <sstream>
#include <vector>
#include <list>
#include <algorithm>
#include <string>
#include <map>
#include <set>
#include <deque>
#include <queue>
#include <stack>
#include <cmath>

// #define DEBUG

using namespace std;

/*************************************************/
// BEGIN UTILITY CODE
typedef unsigned char byte;
typedef long long int64;

const double E5 = .00001;
const double E9 = .000000001;

void _print(string& lead, string& str) {cout<<lead<<'\"'<<str<<'\"'<<endl;} 
void _print(const char* lead, string& str) {cout<<lead<<'\"'<<str<<'\"'<<endl;} 
template<class T> string toString(T x){ostringstream oss; oss<<x; return oss.str();}

bool _isUpperCase(char c){return 'A' <= c && c <= 'Z';}
bool _isLowerCase(char c){return 'a' <= c && c <= 'z';}
bool _isAlpha(char c){return _isUpperCase(c) || _isLowerCase(c);}
bool _isNum(char c){return '0' <= c && c <= '9';}

char _toLower(char c){if(_isUpperCase(c)) return c + 'a' - 'A'; else return c;}
void _toLower(string& s){for(int i = 0; i < s.size(); i++) s[i] = _toLower(s[i]);}
char _toUpper(char c){if(_isLowerCase(c)) return c + 'A' - 'a'; else return c;}
void _toUpper(string& s){for(int i = 0; i < s.size(); i++) s[i] = _toUpper(s[i]);}
void _flush(){if(cin.peek()=='\n') cin.ignore(1,'\n');}

template<class T> void _print(const vector<T>& v){for(int i = 0; i < v.size(); i++)cout<<i<<" "<<v[i]<<endl;}


// END UTILITY CODE
/*************************************************/
int convert(char c)
{
	if(c >= '0' && c <= '9')
		return c - '0';
	
	return c - 'A' + 10;
}

int main()
{
	int T;
	cin>>T;
	
	for(int i = 0; i < T; i++)
	{
		int M,N;
		cin>>M>>N;
		
		int board[32][32];
		
		char c;
		
		for(int y = 0; y < M; y++)
		for(int x = 0; x < N; x += 4)
		{
			cin>>c;
			// each c containts 4 bits
			int data = convert(c);
			
			board[y][x+3] = data%2;
			data /= 2;
			
			board[y][x+2] = data%2;
			data /= 2;
			
			board[y][x+1] = data%2;
			data /= 2;
			
			board[y][x] = data%2;
		}

		// brute force small case
		int len = min(M,N);
		int sol[512];
		memset(sol, 0, sizeof(sol));
		int max_sol = 0;
		while(len > 1)
		{
			for(int y = 0; y < M; y++)
			for(int x = 0; x < N; x++)
			{
				int ex = x+len;
				int ey = y+len;
				if(ex > N || ey > M) break;
				bool good = true;
				for(int ty = y; ty < ey && good; ty++)
				for(int tx = x; tx < ex-1; tx++)
				{
					if(board[ty][tx] != (1 -  board[ty][tx+1]))
					{
						good = false;
						break;
					}
				}
				
				for(int tx = x; tx < ex && good; tx++)
				for(int ty = y; ty < ey-1; ty++)
				{
					if(board[ty][tx] != (1 -  board[ty+1][tx]))
					{
						good = false;
						break;
					}
				}
				
				if(good)
				{
				//	cout<<"\t\t"<<x<<" , "<<y<<" L ="<<len<<endl;
					max_sol = max(len, max_sol);
					sol[len]++;
					for(int tx = x; tx < ex; tx++)
					for(int ty = y; ty < ey; ty++)
						board[ty][tx] = 1000;
				}
				
			}
			len--;
		}
		
		int cnt = 0;
		for(int x = 0; x < 512; x++)
			if(sol[x] != 0) 
				cnt++;
		
		int tot = 0;
		for(int x = max_sol; x >= 2; x--)
			tot += x*x*sol[x];
		if(tot != M*N) cnt++;
		
		cout<<"Case #"<<i+1<<": "<<cnt<<endl;
		for(int x = max_sol; x >= 2; x--)
		{
			if(sol[x] != 0)
				cout<<x<<" "<<sol[x]<<endl;
		}
		
		if(tot != M*N)
			cout<<"1 "<<M*N - tot<<endl;
		
	}

	return 0;
}
