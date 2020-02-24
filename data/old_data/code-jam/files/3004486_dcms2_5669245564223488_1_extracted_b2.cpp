#include <bits/stdc++.h>
#define fr(i,a,b) for (int i = (a), __ = (b); i < __; ++i)	
#define st first
#define nd second
#define dbg(x) cout << #x << " " << x << endl
using namespace std;

const double eps = 1e-7;
const int inf = 0x3f3f3f3f;
typedef pair<int,int> ii;
typedef long long ll;
typedef vector<int> vi;
const ll mod = 1000000009;

string str[150];
int mark[26], passo = 0, cnt[26], frente[26], tras[26];
int rep[150];
ll fat[150];

int fset(int a) {return a == rep[a] ? a : rep[a] = fset(rep[a]);}
void unir(int a, int b) {a = fset(a); b = fset(b); rep[a] = b;}
bool sameSet(int a, int b) {return fset(a) == fset(b);}
/*
bool ok(int conj) {
    string s = "";
    int i = 0;
    for (; i < n; ++i) if (rep[i] == conj) {
        s = str[i]; break;
    }
    for (; i < n; ++i) if (rep[i] == conj) {
        if (str[i][0] == s[s.size()-1]) s += str[i];
        else s = str[i] + s;
    }
    
}*/

int main() {
    fat[0] = fat[1] = 1;
    fr(i,2,150) fat[i] = (i * fat[i-1])%mod;
    int nt; scanf("%d", &nt); ++nt;
    fr(_,1,nt) {
        int n; scanf("%d", &n);
        bool ok = true;
        memset(cnt, 0, sizeof cnt);
        memset(frente, 0, sizeof frente);
        memset(tras, 0, sizeof tras);
        fr(i,0,n) {
            rep[i] = i;
            cin >> str[i];
            char prev = '.'; ++passo;
            if (!ok) continue;
            fr(j,0,str[i].size()) {
                char c = str[i][j] - 'a';
                if (prev != c) {
                    prev = c;
                    if (mark[c] == passo) ok = false;
                    mark[c] = passo;
                }
            }
            if (!ok) continue;
            int len = unique(str[i].begin(), str[i].end())-str[i].begin();
            str[i].resize(len);

            if (len > 1) {
                char c = str[i][0]-'a';
                cnt[c]++;
                ++frente[c];

                c = str[i][len-1]-'a';
                cnt[c]++;
                ++tras[c];
                
                fr(j,1,len-1) {
                    char c = str[i][j]-'a';
                    ++cnt[c];
                    ok &= cnt[c] == 1;
                }
            }
            
            fr(j,0,26) ok &= cnt[j] <= 2 && frente[j] <= 1 && tras[j] <= 1;
                       
        }
        if (n == 1 && ok) {
            printf("Case #%d: 1\n", _); continue;
        }
        bool okkk = false;
        fr(j,0,26) okkk |= (frente[j] == 1 && tras[j] == 0) || (frente[j] == 0 && tras[j] == 1);
        bool allum = true;
        fr(i,0,n) allum &= (int)str[i].size() == 1;
        okkk |= allum;
        ok &= okkk;
        ll ans = 0;
        set<int> auxset;
        if (!ok) goto fim;
        
        fr(i,0,n) fr(j,i+1,n) {
            if (str[i][str[i].size()-1] == str[j][0]) {
                unir(i,j);
            } else if (str[i][0] == str[j][str[j].size()-1]) {
                unir(i,j);
            }
        }
        
        fr(i,0,n) rep[i] = fset(i);
        fr(i,0,n) auxset.insert(rep[i]);
        ans = fat[auxset.size()];
        
        //fr(i,0,n) if (ok(rep[i])) = (ans*2)%mod;
        
        memset(cnt, 0, sizeof cnt);
        
        fr(i,0,n) if (str[i].size() == 1) cnt[str[i][0]-'a']++;
        
        fr(i,0,26) {
            ans = (ans * fat[cnt[i]])%mod;
        }
        
        fim:;
        printf("Case #%d: %lld\n", _, ans);
    }
    return 0;
}
