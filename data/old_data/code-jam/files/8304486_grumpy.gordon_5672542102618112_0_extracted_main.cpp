/*
████─████─███─████──███─███─█──█─████─█──█─███
──██─█──█─█───█──██──█──█───█──█─█──█─█──█─█
─██──████─███─████───█──███─████─█────████─███
██───█──█─█───█──██──█────█─█──█─█──█─█──█─█
████─█──█─███─████──███─███─█──█─████─█──█─███

         o###########oo
      o##"          ""##o
    o#"                "##
  o#"                    "#o
 #"  ##              ##   "##
#"                          ##
#  ###################       #
#                            #
#                            #
#                            #
#                            #
#                            #
#                            #
#o                           #
"#o                         ##
 "#o                       ##
  "#o                    o#"
   "#o                  ##
     "#o              o#"
       "#ooo      ooo#######oo
        ###############   "######o
     o###""        "###o      # ###
   o###o     oooo    ###    oo####"
 o###**#     #**#   ############"
 ""##""""""""""###########    #
    # oooooooo#"#**     ##    #
    # #       # # **    ##    #
    #o#       #o#  *****###ooo#
                        ##
                        ##   o###o
                        ## o##***##
               o########## #***#**##o
             o##"   ""###  #***##***#
 o#######o  ###   oo####   ##**####*#
o##"  ""#############""     ##****###
##"         ##              ##*##*###
##          ###              ##### ##
##           ###              # ##  #
##            ##                 #
##             ##
##             ###
##              ###oo
###              ""###
 ###
  ###
*/
#include <bits/stdc++.h>

using namespace std;

typedef long long ll;

typedef long double ld;

ll AR = 19, BR = 13, CR = 23, XR = 228, YR = 322, MOD = 1e9 + 993;

ll myrand(){
    ll ZR = (XR * AR + YR * BR + CR) % MOD;
    XR = YR;
    YR = ZR;
    return ZR;
}

ll sqr(ll x){
    return x * x;
}

const ll llinf = 2e18;

const ld EPS = 1e-6;

const ld PI = atan2(0, -1);

const int maxn = 2e5 + 100, inf = 1e9 + 100, sq = 300;

int t;

set<int> q;

int answer;

int len;

void go(int n);

void make(int val, int il, vector<int> &a){
    if (il == len){
        if (q.count(val) == 0)
            go(val);
        return;
    }
    for (int i = 0; i <= len; i++)
    if (a[i] > 0){
        a[i]--;
        make(val * 10 + i, il + 1, a);
        a[i]++;
    }
}

void go(int n){
    answer++;
    q.insert(n);
    int x = n;
    int sum = 0;
    vector<int> a(len + 1, 0);
    for (int i = 1; i <= len; i++)
        a[i] = x % 10, x /= 10, sum += a[i];
    reverse(a.begin() + 1, a.begin() + len + 1);
    if (len < sum)
        return;
    a[0] = len - sum;
    make(0, 0, a);
}

int main()
{
    #ifdef ONPC
    ifstream cin("a.in");
    ofstream cout("a.out");
    //freopen("a.in", "r", stdin);
    //freopen("a.out", "w", stdout);
    #else
    //ifstream cin("a.in");
    //ofstream cout("a.out");
    //freopen("trap.in", "r", stdin);
    //freopen("trap.out", "w", stdout);
    #endif // ONPC
    ios::sync_with_stdio(0);
    int t;
    cin >> t;
    for (int iter = 0; iter < t; iter++){
        int n = 0;
        string s;
        cin >> s;
        len = s.length();
        for (int i = 0; i < len; i++)
            n = n * 10 + s[i] - 48;
        q.clear();
        answer = 0;
        go(n);
        cout << "Case #" << iter + 1 << ": " << answer << '\n';
    }
}
