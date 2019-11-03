#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

int find_matches(string remainder, vector<string> tokens);

int main()
{
	// fstream inf("a.in");
	ifstream inf("A-small-attempt1.in");
	ofstream outf("out.txt");
	
	int l;
	int d;
	int n;
	vector<string> words;
	string s;
	vector<string> tokens;

	inf >> l >> d >> n;

	for (int i = 0; i < d; i++)
	{
		inf >> s;
		words.push_back(s);
	}

	for (int i = 0; i < n; i++)
	{
		tokens.clear();

		inf >> s;

		string token;
		bool inside_token = false;
		
		for (int j = 0; j < s.size(); j++)
		{
			if (s[j] == '(')
			{
				inside_token = true;
				token.clear();
			}
			else if (s[j] == ')')
			{
				inside_token = false;
				tokens.push_back(token);
			}
			else if (inside_token)
			{
				token += s[j];
			}
			else
			{
				string t;
				t += s[j];

				tokens.push_back(t);
				token.clear();
			}
		}

		int matches = 0;

		for (int j = 0; j < words.size(); j++)
		{
			string remainder = words[j];

			matches += find_matches(remainder, tokens);
		}

		outf << "Case #" << i + 1 << ": " << matches << "\n";
	}
}

int find_matches(string remainder, vector<string> tokens)
{
	if (remainder.empty())
	{
		return 1;
	}
	else
	{
		char current = remainder[0];
		string token = tokens.front();

		int matches = 0;

		for (int i = 0; i < token.size(); i++)
		{
			if (current == token[i])
			{
				vector<string> tokens2 = tokens;
				tokens2.erase(tokens2.begin());

				return matches + find_matches(remainder.substr(1), tokens2);
			}
		}
	}
}