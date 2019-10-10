#include <iostream>
#include <cstdio>
#include <vector>
#include <algorithm>

using namespace std;

#define REP(i,N) for (int i=0; i<N; i++)

int c[99], s[99], t[99];
int turns, score, deck;
bool used[99];
int main ()
{
    //freopen ("in.txt", "r", stdin);
    freopen ("C-small-attempt2.in", "r", stdin);
    //freopen ("A-large.in", "r", stdin);
    freopen ("out.txt", "w", stdout);

    int T;
    cin >> T;
    REP (cas, T)
    {
        int N, M;
        cin >> N;
        REP (i, N) cin >> c[i] >> s[i] >> t[i];
        cin >> M;
        REP (i, M) cin >> c[N+i] >> s[N+i] >> t[N+i];

        int ans = 0;
        REP (lim, N+M+4)
        {
            REP (i, N+M) used[i] = false;
            turns = 1;
            score = 0;
            deck = N;

            int cnt = 0;

            while (true)
            {
                REP (i, deck) if (!used[i] && t[i] > 0)
                {
                    used[i] = true;
                    score += s[i];
                    deck = min (N+M, deck + c[i]);
                    turns += t[i] - 1;
                }

                if (cnt >= lim) break;
                cnt++;
//cout << deck << " " << score << " " << turns << endl;
     //           cout << lim << " " << score << endl;

                int bigdeck = -1, bigs = 0, besti = -1;
                REP (i, deck) if (!used[i])
                {
                    int tryc = min (N+M, deck + c[i]);
  //                  cout << i << " " << deck << " " << tryc << endl;
 //                   cout << tryc << " " << bigdeck << "asdf" << endl;
                    if (tryc > bigdeck || (tryc == bigdeck && bigs < s[i]))
                    {
   //                    cout << tryc << " " << s[i] << " " << bigs << endl;
                        bigdeck = tryc;
                        bigs = s[i];
                        besti = i;
                    }
                }
                if (bigdeck == -1) break;

                deck = min (N+M, deck + c[besti]);
                score += bigs;
                used[besti] = true;
                turns--;
//cout << "aaa   " << lim << " " << deck << " " << turns << " " << score << endl;
                if (turns == 0) break;
            }
//cout << deck << " "<< turns << endl;
            vector<int> v;
            REP (i, deck) if (!used[i]) v.push_back (s[i]);
            sort (v.begin(), v.end());
            reverse (v.begin(), v.end());
     //       REP (i, v.size()) cout << v[i] << endl;
            REP (i, min ((int)v.size(), turns)) score += v[i];
//cout << lim << " " << score << endl;
            ans = max (ans, score);
        }
        cout << "Case #" << (cas + 1) << ": " << ans << endl;
    }
    return 0;
}
