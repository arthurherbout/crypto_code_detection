#include <bits/stdc++.h>

#define MAXOPT 100000

using namespace std;

typedef pair<int, int> ii;

int T, C=1;
char s[256];
char boa[16][64];
int tamboa[16];
int pre[256];
bool cursei[256];
int ncool[16], total, n, m;
int F[16][64];
char comeco[256];

int grau[256], grauinv[256];
bool ehopcao[256];
int suf[256], folhas[256], nfolhas;
int opcoes[256], nopcoes;
vector<int> adj[256];

bool kmp(char *s, int n, char *pat, int m, int ib) {
    int t = 0;
    for (int i=0;i<n;i++) {
        tenta2:;
        if (s[i] == pat[t]) {
            t++;
            if (t==m) {
                return true;
                t = F[ib][t-1];
            }
            continue;
        }
        if (t==0)
            continue;
        t = F[ib][t-1];
        goto tenta2;
    }
    return false;
}

bool vai(int u) {
    if (u==n) {
        total++;
        for (int i=0;i<m;i++)
            if (kmp(s, n, boa[i], tamboa[i], i))
                ncool[i]++;
        return true;
    }

    for (int i=0;i<nopcoes;i++) {
        if (total >= MAXOPT) return;
        if (rand()%100 > 100) continue;

        int c = opcoes[i];

        if (!cursei[c] and (pre[c] == -1 or cursei[pre[c]])) {

            s[u] = comeco[c];
            cursei[c] = true;

            for (int i=0;i<(int)adj[c].size();i++) {
                grau[adj[c][i]]--;
                if (grau[adj[c][i]]==0 and !ehopcao[adj[c][i]]) {
                    opcoes[nopcoes++] = adj[c][i];
                    ehopcao[adj[c][i]]=true;
                }
            }

            if (vai(u+1)) return true;

            for (int i=0;i<(int)adj[c].size();i++) {
                grau[adj[c][i]]++;
            }
            cursei[c]=false;
        }
    }
    return false;
}

void fazF(int u, char *s) {
    int n = strlen(s);
    F[u][0] = 0;
    for (int i=1;i<n;i++) {
        int j = i-1;
        tenta:;
        if (j<0) {
            F[u][i]=0;
            continue;
        }
        if (s[i]==s[F[u][j]])
            F[u][i] = F[u][j]+1;
        else {
            j = F[u][j]-1;
            goto tenta;
        }
    }
}


int main() {

    srand(time(NULL));
    for(scanf("%d",&T);T--;) {
        printf("Case #%d:",C++);
        scanf("%d",&n);
        for (int i=0;i<n;i++) {
            adj[i].clear();
            grauinv[i]=0;
        }
        nfolhas=0;
        for (int i=0;i<n;i++) {
            scanf("%d",pre+i);
            pre[i]--;
            if (pre[i]==-1)
                folhas[nfolhas++] = i;
            else {
                adj[ pre[i] ].push_back(i);
                grauinv[ i ]++;
            }
        }
        scanf("%s",comeco);
        scanf("%d",&m);
        for (int i=0;i<m;i++) {
            scanf("%s",boa[i]);
            fazF(i, boa[i]);
            tamboa[i] = strlen(boa[i]);
        }

        memset(ncool,0,m*sizeof(int));
        total = 0;

        while (total < MAXOPT) {
            memset(cursei,false,n*sizeof(bool));

            memcpy(grau,grauinv,n*sizeof(int));

            memset(ehopcao,false,n*sizeof(bool));
            for (int i=0;i<nfolhas;i++) suf[i] = i;
            for (int i=0;i<nfolhas;i++) {
                int tt = rand()%(nfolhas-i);
                swap(suf[i],suf[i+tt]);
                opcoes[i] = folhas[suf[i]];
                ehopcao[opcoes[i]]=true;
            }
            nopcoes = nfolhas;

            vai(0);
        }

        for (int i=0;i<m;i++)
            printf(" %.12lf",(double)ncool[i]/(double)total);
        printf("\n");
    }

    return 0;
}
