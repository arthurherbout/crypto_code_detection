#include <bits/stdc++.h>

using namespace std;

unordered_map<string, int> left_freq_ori, right_freq_ori;
int mark[1005];
char left_str[30], right_str[30];

int main() {
  srand(time(NULL));

  int nt; scanf("%d", &nt);
  for (int caso = 1; caso <= nt; ++caso) {
    left_freq_ori.clear();
    right_freq_ori.clear();

    vector< pair<string, string> > pairs;

    int n; scanf("%d", &n);
    for (int i = 0; i < n; ++i) {
      scanf("%s %s", left_str, right_str);
      string lstr = string(left_str);
      string rstr = string(right_str);
      
      pairs.push_back(make_pair(lstr, rstr));

      left_freq_ori[lstr]++;
      right_freq_ori[rstr]++;
    }

    int answer = 0;

    for (int k = 0; k < 2000; ++k) {
      memset(mark, 0, sizeof mark);
      random_shuffle(pairs.begin(), pairs.end());

      int aux = 0;
      unordered_map<string, int> left_freq = left_freq_ori;
      unordered_map<string, int> right_freq = right_freq_ori;
      
      while (true) {
        bool goon = false;
        for (int i = 0; i < n; ++i) if (!mark[i]) {
          if (left_freq[pairs[i].first] > 1 && right_freq[pairs[i].second] > 1) {
            goon = true;
            mark[i] = 1;
            ++aux;
            --left_freq[pairs[i].first];
            --right_freq[pairs[i].second];
            break;
          }
        }
        if (!goon) break;
      }

      answer = max(answer, aux);
    }

    printf("Case #%d: %d\n", caso, answer);
  }
  return 0;
}