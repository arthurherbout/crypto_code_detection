#include <cstdio>
#include <string>
using namespace std;

int n_rp, n_ps, n_sr;
string fin;

bool det(int n, int r, int p, int s){
    if(n == 0){
        if(r) fin = "R";
        if(p) fin = "P";
        if(s) fin = "S";
        return true;
    }
    int rp, ps, sr, d;
    if(r >= p && r >= s){ // r
        d = (p + s - r) / 2;
        if(d < 0) return false;
        rp = p - d;
        sr = s - d;
        ps = d;
    }
    else if(p >= r && p >= s){ // p
        d = (r + s - p) / 2;
        if(d < 0) return false;
        rp = r - d;
        ps = s - d;
        sr = d;
    }
    else{ // s
        d = (r + p - s) / 2;
        if(d < 0) return false;
        ps = p - d;
        sr = r - d;
        rp = d;
    }
    bool b = det(n-1, sr, rp, ps);
    n_rp = rp;
    n_sr = sr;
    n_ps = ps;
    return b;
}

string mutate(string s, int n){
    if(n == 0) return s;
    string t;
    for(int i=0;i<s.size();i++){
        if(s[i] == 'R') t += "RS";
        if(s[i] == 'P') t += "PR";
        if(s[i] == 'S') t += "PS";
    }
    return mutate(t, n-1);
}

string rearrange(string s, int n){
    if(n == 0) return s;
    int o = 1 << (n-1);
    string s1 = rearrange(s.substr(0, o), n-1);
    string s2 = rearrange(s.substr(o, o), n-1);
    if(s1 < s2) return s1 + s2;
    else return s2 + s1;
}

int main(){
    int T; scanf("%d", &T);
    for(int ti=1;ti<=T;ti++){
        fprintf(stderr, "start %d\n", ti);
        int n, r, p, s; scanf("%d%d%d%d", &n, &r, &p, &s);
        if(det(n, r, p, s)){
            printf("Case #%d: ", ti);
            printf("%s\n", rearrange(mutate(fin, n), n).c_str());
        }
        else{
            printf("Case #%d: IMPOSSIBLE\n", ti);
        }
        fprintf(stderr, "end %d\n", ti);
    }
    return 0;
}
