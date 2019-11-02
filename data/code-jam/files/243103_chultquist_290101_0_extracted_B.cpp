#include <iostream>
#include <cstdio>
#include <utility>
#include <vector>
#include <algorithm>
#include <queue>
#include <set>
#include <vector>
#include <map>

using namespace std;

#define MOD 10009

int counts[26];
vector<string> words;
long sum = 0;
vector<char> variables[5];
int termCount;

void go(int x)
{
    if (x < 0)
    {
        long value = 0;
        for (int j = 0; j <= termCount; j++)
        {
            long term = 1;
            for (unsigned int k = 0; k < variables[j].size(); k++)
                term = (term * counts[variables[j][k] - 'a']) % MOD;
            value += term;
        }
        sum = (sum + value) % MOD;
    }
    else
    {
        for (unsigned int i = 0; i < words.size(); i++)
        {
            string word = words[i];
            for (unsigned int j = 0; j < word.length(); j++)
                counts[word[j] - 'a']++;
            go(x - 1);
            for (unsigned int j = 0; j < word.length(); j++)
                counts[word[j] - 'a']--;
        }
    }
}

int main()
{
    int caseCount;
    cin >> caseCount;
    for (int caseNum = 1; caseNum <= caseCount; caseNum++)
    {
        string expr;
        cin >> expr;
        termCount = 0;
        for (int i = 0; i < 5; i++)
            variables[i].clear();
        words.clear();
        for (unsigned int i = 0; i < expr.length(); i++)
        {
            if (expr[i] == '+')
                termCount++;
            else
                variables[termCount].push_back(expr[i]);
        }

        int K;
        cin >> K;

        int n;
        cin >> n;

        for (int i = 0; i < n; i++)
        {
            string word;
            cin >> word;
            words.push_back(word);
        }

        cout << "Case #" << caseNum << ":";
        for (int i = 0; i < K; i++)
        {
            memset(counts, 0, sizeof(counts));
            sum = 0;
            go(i);
            cout << " " << sum;
        }
        cout << endl;
    }

    return 0;
}
