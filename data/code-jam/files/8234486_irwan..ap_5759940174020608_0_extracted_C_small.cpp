#include <iostream>
#include <unordered_set>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

vector<string> splitWord(string s) {
    vector<string> words;
    istringstream iss(s);
    string word;
    while (iss >> word) {
        words.push_back(word);
    }
    return words;
}

int checkBruteForce(unordered_set<string> english, unordered_set<string> french, const vector<vector<string>>& unknownWords, int pattern) {

    int n = unknownWords.size();
    for (int i = 0; i < n; i++) {
        for (string w : unknownWords[i]) {
            if (pattern & (1<<i)) {
                english.insert(w);
            } else {
                french.insert(w);
            }
        }
    }

    int ans = 0;
    for (string s : english) {
        if (french.find(s) != french.end()) {
            ans++;
        }
    }
    return ans;
}

int solve(const vector<string>& englishWords, const vector<string>& frenchWords, const vector<vector<string>>& unknownWords) {

    int n = unknownWords.size();
    int maxPattern = (1<<n);

    unordered_set<string> english;
    unordered_set<string> french;

    for (string w : englishWords) {
        english.insert(w);
    }
    for (string w : frenchWords) {
        french.insert(w);
    }

    int ans = 100000000;
    for (int pattern = 0; pattern < maxPattern; pattern++) {
        ans = min(ans, checkBruteForce(english, french, unknownWords, pattern));
    }

    return ans;
}

int main() {

    int T;
    cin >> T;
    string dummy;

    for (int t = 1; t <= T; t++) {

        int N;
        cin >> N;
        N -= 2;

        getline(cin, dummy);

        string english;
        getline(cin, english);

        string french;
        getline(cin, french);

        vector<string> unknown(N);

        for (int i = 0; i < N; i++) {
            getline(cin, unknown[i]);
        }

        vector<string> englishWords = splitWord(english);
        vector<string> frenchWords = splitWord(french);
        vector<vector<string>> unknownWords(N);
        for (int i = 0; i < N; i++) {
            unknownWords[i] = splitWord(unknown[i]);
        }

        cout << "Case #" << t << ": ";
        cout << solve(englishWords, frenchWords, unknownWords) << endl;
    }

    return 0;
}

